#include "config.h"

#include "broadway-server-local.h"

#include "broadway-output.h"

#include <glib.h>
#include <glib/gprintf.h>
#include "gdktypes.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#elif defined (G_OS_WIN32)
#include <io.h>
#endif
#ifdef HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#ifdef G_OS_UNIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif
#ifdef HAVE_GIO_UNIX
#include <gio/gunixsocketaddress.h>
#endif
#ifdef G_OS_WIN32
#include <windows.h>
#include <string.h>
#endif

BroadwayServer *server;

// server = broadway_server_new();


void
broadway_server_query_mouse (guint32            *toplevel,
			     gint32             *root_x,
			     gint32             *root_y,
			     guint32            *mask)
{
  if (server->output)
    {
      broadway_server_consume_all_input (server);
      if (root_x)
	*root_x = server->future_root_x;
      if (root_y)
	*root_y = server->future_root_y;
      if (mask)
	*mask = server->future_state;
      if (toplevel)
	*toplevel = server->future_mouse_in_toplevel;
      return;
    }

  /* Fallback when unconnected */
  if (root_x)
    *root_x = server->last_x;
  if (root_y)
    *root_y = server->last_y;
  if (mask)
    *mask = server->last_state;
  if (toplevel)
    *toplevel = server->mouse_in_toplevel_id;
}

guint32             
broadway_server_new_window(
        int               x,
        int               y,
        int               width,
        int               height,
        gboolean          is_temp)
{
    BroadwayWindow *window;

    window = g_new0 (BroadwayWindow, 1);
    window->id = server->id_counter++;
    window->x = x;
    window->y = y;
    if (x == 0 && y == 0 && !is_temp)
        {
        /* TODO: Better way to know if we should pick default pos */
        window->x = 100;
        window->y = 100;
        }
    window->width = width;
    window->height = height;
    window->is_temp = is_temp;

    g_hash_table_insert (server->id_ht,
                GINT_TO_POINTER (window->id),
                window);

    server->toplevels = g_list_append (server->toplevels, window);

    if (server->output)
        broadway_output_new_surface (server->output,
                    window->id,
                    window->x,
                    window->y,
                    window->width,
                    window->height,
                    window->is_temp);
    else
        fake_configure_notify (server, window);

    return window->id;
}

void                
broadway_server_destroy_window(gint id)
{
    BroadwayWindow *window;

    if (server->mouse_in_toplevel_id == id)
        {
        /* TODO: Send leave + enter event, update cursors, etc */
        server->mouse_in_toplevel_id = 0;
        }

    if (server->pointer_grab_window_id == id)
        server->pointer_grab_window_id = -1;

    if (server->output)
        broadway_output_destroy_surface (server->output,
                        id);

    window = g_hash_table_lookup (server->id_ht,
                    GINT_TO_POINTER (id));
    if (window != NULL)
        {
        server->toplevels = g_list_remove (server->toplevels, window);
        g_hash_table_remove (server->id_ht,
                GINT_TO_POINTER (id));

        g_free (window->cached_surface_name);
        if (window->cached_surface != NULL)
        cairo_surface_destroy (window->cached_surface);

        g_free (window);
        }
}

gboolean
broadway_server_window_show (gint id)
{
  BroadwayWindow *window;
  gboolean sent = FALSE;

  window = g_hash_table_lookup (server->id_ht,
				GINT_TO_POINTER (id));
  if (window == NULL)
    return FALSE;

  window->visible = TRUE;

  if (server->output)
    {
      broadway_output_show_surface (server->output, window->id);
      sent = TRUE;
    }

  return sent;
}

gboolean
broadway_server_window_hide (gint id)
{
  BroadwayWindow *window;
  gboolean sent = FALSE;

  window = g_hash_table_lookup (server->id_ht,
				GINT_TO_POINTER (id));
  if (window == NULL)
    return FALSE;

  window->visible = FALSE;

  if (server->mouse_in_toplevel_id == id)
    {
      /* TODO: Send leave + enter event, update cursors, etc */
      server->mouse_in_toplevel_id = 0;
    }

  if (server->pointer_grab_window_id == id)
    server->pointer_grab_window_id = -1;

  if (server->output)
    {
      broadway_output_hide_surface (server->output, window->id);
      sent = TRUE;
    }
  return sent;
}

void
broadway_server_focus_window (gint new_focused_window)
{
  BroadwayInputMsg focus_msg;

  if (server->focused_window_id == new_focused_window)
    return;

  memset (&focus_msg, 0, sizeof (focus_msg));
  focus_msg.base.type = BROADWAY_EVENT_FOCUS;
  focus_msg.base.time = broadway_server_get_last_seen_time (server);
  focus_msg.focus.old_id = server->focused_window_id;
  focus_msg.focus.new_id = new_focused_window;

  broadway_events_got_input (&focus_msg, -1);

  /* Keep track of the new focused window */
  server->focused_window_id = new_focused_window;
}

void
broadway_server_flush ()
{
  if (server->output &&
      !broadway_output_flush (server->output))
    {
      server->saved_serial = broadway_output_get_next_serial (server->output);
      broadway_output_free (server->output);
      server->output = NULL;
    }
}

void
broadway_server_window_set_transient_for (gint id, gint parent)
{
  BroadwayWindow *window;

  window = g_hash_table_lookup (server->id_ht,
				GINT_TO_POINTER (id));
  if (window == NULL)
    return;

  window->transient_for = parent;

  if (server->output)
    {
      broadway_output_set_transient_for (server->output, window->id, window->transient_for);
      broadway_server_flush (server);
    }
}

void
broadway_server_window_update (gint id,
			       cairo_surface_t *surface)
{
  BroadwayWindow *window;
  BroadwayBuffer *buffer;

  if (surface == NULL)
    return;

  window = g_hash_table_lookup (server->id_ht,
				GINT_TO_POINTER (id));
  if (window == NULL)
    return;

  g_assert (window->width == cairo_image_surface_get_width (surface));
  g_assert (window->height == cairo_image_surface_get_height (surface));

  buffer = broadway_buffer_create (window->width, window->height,
                                   cairo_image_surface_get_data (surface),
                                   cairo_image_surface_get_stride (surface));

  if (server->output != NULL)
    {
      window->buffer_synced = TRUE;
      broadway_output_put_buffer (server->output, window->id,
                                  window->buffer, buffer);
    }

  if (window->buffer)
    broadway_buffer_destroy (window->buffer);

  window->buffer = buffer;
}

gboolean
broadway_server_window_move_resize ( gint id,
				    gboolean with_move,
				    int x,
				    int y,
				    int width,
				    int height)
{
  BroadwayWindow *window;
  gboolean with_resize;
  gboolean sent = FALSE;

  window = g_hash_table_lookup (server->id_ht,
				GINT_TO_POINTER (id));
  if (window == NULL)
    return FALSE;

  with_resize = width != window->width || height != window->height;
  window->width = width;
  window->height = height;

  if (server->output != NULL)
    {
      broadway_output_move_resize_surface (server->output,
					   window->id,
					   with_move, x, y,
					   with_resize, window->width, window->height);
      sent = TRUE;
    }
  else
    {
      if (with_move)
	{
	  window->x = x;
	  window->y = y;
	}

      fake_configure_notify (server, window);
    }

  return sent;
}

guint32
broadway_server_grab_pointer (gint client_id,
			      gint id,
			      gboolean owner_events,
			      guint32 event_mask,
			      guint32 time_)
{
  if (server->pointer_grab_window_id != -1 &&
      time_ != 0 && server->pointer_grab_time > time_)
    return GDK_GRAB_ALREADY_GRABBED;

  if (time_ == 0)
    time_ = server->last_seen_time;

  server->pointer_grab_window_id = id;
  server->pointer_grab_client_id = client_id;
  server->pointer_grab_owner_events = owner_events;
  server->pointer_grab_time = time_;

  if (server->output)
    {
      broadway_output_grab_pointer (server->output,
				    id,
				    owner_events);
      broadway_server_flush (server);
    }

  /* TODO: What about toplevel grab events if we're not connected? */

  return GDK_GRAB_SUCCESS;
}

guint32
broadway_server_ungrab_pointer (guint32    time_)
{
  guint32 serial;

  if (server->pointer_grab_window_id != -1 &&
      time_ != 0 && server->pointer_grab_time > time_)
    return 0;

  /* TODO: What about toplevel grab events if we're not connected? */

  if (server->output)
    {
      serial = broadway_output_ungrab_pointer (server->output);
      broadway_server_flush (server);
    }
  else
    {
      serial = server->saved_serial;
    }

  server->pointer_grab_window_id = -1;

  return serial;
}

void
broadway_server_set_show_keyboard (gboolean show)
{
  server->show_keyboard = show;

  if (server->output)
    {
      broadway_output_set_show_keyboard (server->output, server->show_keyboard);
      broadway_server_flush (server);
   }
}