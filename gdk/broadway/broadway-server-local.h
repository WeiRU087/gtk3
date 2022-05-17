#ifndef BROADWAY_SERVER_LOCAL
#define BROADWAY_SERVER_LOCAL
#include "broadway-protocol.h"
#include <glib-object.h>
#include <cairo.h>

void broadway_events_got_input (BroadwayInputMsg *message,
				gint32 client_id);

typedef struct _BroadwayServer BroadwayServer;
typedef struct _BroadwayServerClass BroadwayServerClass;

#define BROADWAY_TYPE_SERVER              (broadway_server_get_type())
#define BROADWAY_SERVER(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), BROADWAY_TYPE_SERVER, BroadwayServer))
#define BROADWAY_SERVER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BROADWAY_TYPE_SERVER, BroadwayServerClass))
#define BROADWAY_IS_SERVER(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), BROADWAY_TYPE_SERVER))
#define BROADWAY_IS_SERVER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BROADWAY_TYPE_SERVER))
#define BROADWAY_SERVER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BROADWAY_TYPE_SERVER, BroadwayServerClass))


BroadwayServer     *broadway_server_new                      (char             *address,
							      int               port,
                                                              const char       *ssl_cert,
                                                              const char       *ssl_key,
							      GError          **error);
BroadwayServer     *broadway_server_on_unix_socket_new       (char             *address,
							      GError          **error);
gboolean            broadway_server_has_client               ();
void                broadway_server_flush                    ();
void                broadway_server_sync                     ();
void                broadway_server_get_screen_size          (,
							      guint32          *width,
							      guint32          *height);
guint32             broadway_server_get_next_serial          ();
guint32             broadway_server_get_last_seen_time       ();
gboolean            broadway_server_lookahead_event          (
							      const char       *types);
void                broadway_server_query_mouse              (
							      guint32          *toplevel,
							      gint32           *root_x,
							      gint32           *root_y,
							      guint32          *mask);
guint32             broadway_server_grab_pointer             (
							      gint              client_id,
							      gint              id,
							      gboolean          owner_events,
							      guint32           event_mask,
							      guint32           time_);
guint32             broadway_server_ungrab_pointer           (
							      guint32           time_);
gint32              broadway_server_get_mouse_toplevel       ();
void                broadway_server_set_show_keyboard        (
                                                              gboolean          show);
guint32             broadway_server_new_window               (
							      int               x,
							      int               y,
							      int               width,
							      int               height,
							      gboolean          is_temp);
void                broadway_server_destroy_window           (
							      gint              id);
gboolean            broadway_server_window_show              (
							      gint              id);
gboolean            broadway_server_window_hide              (
							      gint              id);
void                broadway_server_window_raise             (
							      gint              id);
void                broadway_server_window_lower             (
							      gint              id);
void                broadway_server_window_set_transient_for (
							      gint              id,
							      gint              parent);
gboolean            broadway_server_window_translate         (
							      gint              id,
							      cairo_region_t   *area,
							      gint              dx,
							      gint              dy);
cairo_surface_t   * broadway_server_create_surface           (int               width,
							      int               height);
void                broadway_server_window_update            (
							      gint              id,
							      cairo_surface_t  *surface);
gboolean            broadway_server_window_move_resize       (
							      gint              id,
							      gboolean          with_move,
							      int               x,
							      int               y,
							      int               width,
							      int               height);
void                broadway_server_focus_window             (
                                                              gint              new_focused_window);
cairo_surface_t * broadway_server_open_surface (
						guint32 id,
						char *name,
						int width,
						int height);

#endif /* BROADWAY_SERVER_LOCAL */
