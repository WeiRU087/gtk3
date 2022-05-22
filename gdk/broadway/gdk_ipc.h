#ifndef GDK_IPC_H
#define GDK_IPC_H

#include <cassert>
#include <cstdlib>
#include <string>



typedef struct {
  uint32_t size;
  uint32_t serial;
  uint32_t type;
} BroadwayRequestBase, BroadwayRequestFlush, BroadwayRequestSync, BroadwayRequestQueryMouse;

typedef struct {
  BroadwayRequestBase base;
  uint32_t id;
} BroadwayRequestDestroyWindow, BroadwayRequestShowWindow, BroadwayRequestHideWindow, BroadwayRequestFocusWindow;

typedef struct {
  BroadwayRequestBase base;
  uint32_t id;
  uint32_t parent;
} BroadwayRequestSetTransientFor;

typedef struct {
  BroadwayRequestBase base;
  uint32_t id;
  char name[36];
  uint32_t width;
  uint32_t height;
} BroadwayRequestUpdate;

typedef struct {
  BroadwayRequestBase base;
  uint32_t id;
  uint32_t owner_events;
  uint32_t event_mask;
  uint32_t time_;
} BroadwayRequestGrabPointer;

typedef struct {
  BroadwayRequestBase base;
  uint32_t time_;
} BroadwayRequestUngrabPointer;

typedef struct {
  BroadwayRequestBase base;
  int32_t x;
  int32_t y;
  uint32_t width;
  uint32_t height;
  uint32_t is_temp;
} BroadwayRequestNewWindow;

typedef struct {
  BroadwayRequestBase base;
  uint32_t id;
  uint32_t with_move;
  int32_t x;
  int32_t y;
  uint32_t width;
  uint32_t height;
} BroadwayRequestMoveResize;

typedef struct {
  BroadwayRequestBase base;
  uint32_t show_keyboard;
} BroadwayRequestSetShowKeyboard;

typedef enum {
  BROADWAY_REQUEST_NEW_WINDOW,
  BROADWAY_REQUEST_FLUSH,
  BROADWAY_REQUEST_SYNC,
  BROADWAY_REQUEST_QUERY_MOUSE,
  BROADWAY_REQUEST_DESTROY_WINDOW,
  BROADWAY_REQUEST_SHOW_WINDOW,
  BROADWAY_REQUEST_HIDE_WINDOW,
  BROADWAY_REQUEST_SET_TRANSIENT_FOR,
  BROADWAY_REQUEST_UPDATE,
  BROADWAY_REQUEST_MOVE_RESIZE,
  BROADWAY_REQUEST_GRAB_POINTER,
  BROADWAY_REQUEST_UNGRAB_POINTER,
  BROADWAY_REQUEST_FOCUS_WINDOW,
  BROADWAY_REQUEST_SET_SHOW_KEYBOARD
} BroadwayRequestType;



int Gtk_Broadway_Send_Flush_Sync_QueryMouse(const BroadwayRequestFlush &msg, BroadwayRequestType type);

int Gtk_Broadway_Send_New_Window(const BroadwayRequestNewWindow& msg, BroadwayRequestType type);

void Gtk_Broadway_Send_Destroy_Show_Hide_Focus_Window(const BroadwayRequestDestroyWindow &msg, BroadwayRequestType type);

void Gtk_Broadway_Send_Set_Transient_For(const BroadwayRequestSetTransientFor &msg, BroadwayRequestType type);

void Gtk_Broadway_Send_Window_Update(const BroadwayRequestUpdate &msg, BroadwayRequestType type);

void Gtk_Broadway_Send_Move_Resize(const BroadwayRequestMoveResize &msg, BroadwayRequestType type);

int Gtk_Broadway_Send_Grab_Pointer(const BroadwayRequestGrabPointer &msg, BroadwayRequestType type);

int Gtk_Broadway_Send_Ungrab_Pointer(const BroadwayRequestUngrabPointer &msg, BroadwayRequestType type);

void Gtk_Broadway_Send_Set_Show_Keyboard(const BroadwayRequestSetShowKeyboard, BroadwayRequestType type);




#endif //GDK_IPC_H