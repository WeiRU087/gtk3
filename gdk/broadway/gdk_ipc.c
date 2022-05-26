#include "gdk_ipc.h"
#include <dlfcn.h>
#include <assert.h>



// begin load function from libugp_sdk.so

typedef int (*Gtk_Broadway_Send_Flush_Sync_QueryMouse_Ptr)(const BroadwayRequestFlush *msg, BroadwayRequestType type);
typedef int (*Gtk_Broadway_Send_New_Window_Ptr)(const BroadwayRequestNewWindow *msg, BroadwayRequestType type);
typedef void (*Gtk_Broadway_Send_Destroy_Show_Hide_Focus_Window_Ptr)(const BroadwayRequestDestroyWindow *msg, BroadwayRequestType type);
typedef void (*Gtk_Broadway_Send_Set_Transient_For_Ptr)(const BroadwayRequestSetTransientFor *msg, BroadwayRequestType type);
typedef void (*Gtk_Broadway_Send_Window_Update_Ptr)(const BroadwayRequestUpdate *msg, BroadwayRequestType type);
typedef void (*Gtk_Broadway_Send_Move_Resize_Ptr)(const BroadwayRequestMoveResize *msg, BroadwayRequestType type);
typedef int (*Gtk_Broadway_Send_Grab_Pointer_Ptr)(const BroadwayRequestGrabPointer *msg, BroadwayRequestType type);
typedef int (*Gtk_Broadway_Send_Ungrab_Pointer_Ptr)(const BroadwayRequestUngrabPointer *msg, BroadwayRequestType type);
typedef void (*Gtk_Broadway_Send_Set_Show_Keyboard_Ptr)(const BroadwayRequestSetShowKeyboard *msg, BroadwayRequestType type);


struct UgpFunctions {
   Gtk_Broadway_Send_Flush_Sync_QueryMouse_Ptr pGtk_Broadway_Send_Flush_Sync_QueryMouse;
   Gtk_Broadway_Send_New_Window_Ptr pGtk_Broadway_Send_New_Window;
   Gtk_Broadway_Send_Destroy_Show_Hide_Focus_Window_Ptr pGtk_Broadway_Send_Destroy_Show_Hide_Focus_Window;
   Gtk_Broadway_Send_Set_Transient_For_Ptr pGtk_Broadway_Send_Set_Transient_For;
   Gtk_Broadway_Send_Window_Update_Ptr pGtk_Broadway_Send_Window_Update;
   Gtk_Broadway_Send_Move_Resize_Ptr pGtk_Broadway_Send_Move_Resize;
   Gtk_Broadway_Send_Grab_Pointer_Ptr pGtk_Broadway_Send_Grab_Pointer;
   Gtk_Broadway_Send_Ungrab_Pointer_Ptr pGtk_Broadway_Send_Ungrab_Pointer;
   Gtk_Broadway_Send_Set_Show_Keyboard_Ptr pGtk_Broadway_Send_Set_Show_Keyboard;
   void* lib_handle;
} g_ugp_functions = {0}; 

#define LINK_METHOD(METHOD) \
  { \
    g_ugp_functions.p##METHOD = (METHOD##_Ptr)dlsym(g_ugp_functions.lib_handle, #METHOD); \
    if (!g_ugp_functions.p##METHOD) { \
      assert(0 && "Fail to Load function"); \
    } \
  } \

void LoadFunctionsIfNeeded() {
 if (g_ugp_functions.lib_handle) {
   return;
  }
  g_ugp_functions.lib_handle = dlopen("libugp_sdk.so", RTLD_LAZY);
  if (!g_ugp_functions.lib_handle) {
      assert(0 && "Fail to Load library");
  }

  LINK_METHOD(Gtk_Broadway_Send_Flush_Sync_QueryMouse);
  LINK_METHOD(Gtk_Broadway_Send_New_Window);
  LINK_METHOD(Gtk_Broadway_Send_Destroy_Show_Hide_Focus_Window);
  LINK_METHOD(Gtk_Broadway_Send_Set_Transient_For);
  LINK_METHOD(Gtk_Broadway_Send_Window_Update);
  LINK_METHOD(Gtk_Broadway_Send_Move_Resize);
  LINK_METHOD(Gtk_Broadway_Send_Grab_Pointer);
  LINK_METHOD(Gtk_Broadway_Send_Ungrab_Pointer);
  LINK_METHOD(Gtk_Broadway_Send_Set_Show_Keyboard);
}
// end load function from libugp_sdk.so

static int isInitialized = 0;

int Gtk_Broadway_Send_Flush_Sync_QueryMouse(const BroadwayRequestFlush msg, BroadwayRequestType type) {
    if (!isInitialized){
      isInitialized = TRUE;
      LoadFunctionsIfNeeded();
    }

    g_ugp_functions.pGtk_Broadway_Send_Flush_Sync_QueryMouse(&msg, type);
    return 0;
}

int Gtk_Broadway_Send_New_Window(const BroadwayRequestNewWindow msg, BroadwayRequestType type) {
    if (!isInitialized){
      isInitialized = TRUE;
      LoadFunctionsIfNeeded();
    }

    g_ugp_functions.pGtk_Broadway_Send_New_Window(&msg, type);
    return 0;
}

void Gtk_Broadway_Send_Destroy_Show_Hide_Focus_Window(const BroadwayRequestDestroyWindow msg, BroadwayRequestType type) {
    if (!isInitialized){
      isInitialized = TRUE;
      LoadFunctionsIfNeeded();
    }

    g_ugp_functions.pGtk_Broadway_Send_Destroy_Show_Hide_Focus_Window(&msg, type);
}

void Gtk_Broadway_Send_Set_Transient_For(const BroadwayRequestSetTransientFor msg, BroadwayRequestType type) {
    if (!isInitialized){
      isInitialized = TRUE;
      LoadFunctionsIfNeeded();
    }

    g_ugp_functions.pGtk_Broadway_Send_Set_Transient_For(&msg, type);
}

void Gtk_Broadway_Send_Window_Update(const BroadwayRequestUpdate msg, BroadwayRequestType type) {
    if (!isInitialized){
      isInitialized = TRUE;
      LoadFunctionsIfNeeded();
    }

    g_ugp_functions.pGtk_Broadway_Send_Window_Update(&msg, type);
}

void Gtk_Broadway_Send_Move_Resize(const BroadwayRequestMoveResize msg, BroadwayRequestType type) {
    if (!isInitialized){
      isInitialized = TRUE;
      LoadFunctionsIfNeeded();
    }

    g_ugp_functions.pGtk_Broadway_Send_Move_Resize(&msg, type);
}

int Gtk_Broadway_Send_Grab_Pointer(const BroadwayRequestGrabPointer msg, BroadwayRequestType type) {
    if (!isInitialized){
      isInitialized = TRUE;
      LoadFunctionsIfNeeded();
    }

    g_ugp_functions.pGtk_Broadway_Send_Grab_Pointer(&msg, type);
    return 0;
}

int Gtk_Broadway_Send_Ungrab_Pointer(const BroadwayRequestUngrabPointer msg, BroadwayRequestType type) {
    if (!isInitialized){
      isInitialized = TRUE;
      LoadFunctionsIfNeeded();
    }

    g_ugp_functions.pGtk_Broadway_Send_Ungrab_Pointer(&msg, type);
    return 0;
}

void Gtk_Broadway_Send_Set_Show_Keyboard(const BroadwayRequestSetShowKeyboard msg, BroadwayRequestType type) {
    if (!isInitialized){
      isInitialized = TRUE;
      LoadFunctionsIfNeeded();
    }

    g_ugp_functions.pGtk_Broadway_Send_Set_Show_Keyboard(&msg, type);
}
