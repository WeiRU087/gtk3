#ifndef GDK_IPC_H
#define GDK_IPC_H

#include "broadway-protocol.h"



void Gtk_Broadway_Send_Flush_Sync_QueryMouse(const BroadwayRequestFlush msg, BroadwayRequestType type);

void Gtk_Broadway_Send_New_Window(const BroadwayRequestNewWindow msg, BroadwayRequestType type);

void Gtk_Broadway_Send_Destroy_Show_Hide_Focus_Window(const BroadwayRequestDestroyWindow msg, BroadwayRequestType type);

void Gtk_Broadway_Send_Set_Transient_For(const BroadwayRequestSetTransientFor msg, BroadwayRequestType type);

void Gtk_Broadway_Send_Window_Update(const BroadwayRequestUpdate msg, BroadwayRequestType type);

void Gtk_Broadway_Send_Move_Resize(const BroadwayRequestMoveResize msg, BroadwayRequestType type);

int Gtk_Broadway_Send_Grab_Pointer(const BroadwayRequestGrabPointer msg, BroadwayRequestType type);

int Gtk_Broadway_Send_Ungrab_Pointer(const BroadwayRequestUngrabPointer msg, BroadwayRequestType type);

void Gtk_Broadway_Send_Set_Show_Keyboard(const BroadwayRequestSetShowKeyboard msg, BroadwayRequestType type);

gboolean Gdk_Broadway_Peek_Reply(BroadwayReply* reply);



#endif //GDK_IPC_H