#include "gdk_ipc.h"
#include <dlfcn.h>
#include <iostream>

namespace ugp {

// begin load function from libugp_sdk.so
typedef void (*Gtk_Broadway_Send_Message_Ptr)(const BroadwayRequestBase* payload);

struct UgpFunctions {
   Gtk_Broadway_Send_Message_Ptr pGtk_Broadway_Send_Message;
   void* lib_handle;
} g_ugp_functions = {0}; 

#define LINK_METHOD(METHOD) \
  { \
    g_ugp_functions.p##METHOD = (METHOD##_Ptr)dlsym(g_ugp_functions.lib_handle, #METHOD); \
    if (!g_ugp_functions.p##METHOD) { \
      std::cerr << "Fail to Load function" << #METHOD << std::endl; \
      assert(false && "Fail to Load function"); \
    } \
  } \

void LoadFunctionsIfNeeded() {
 if (g_ugp_functions.lib_handle) {
   return;
  }
  g_ugp_functions.lib_handle = dlopen("libugp_sdk.so", RTLD_LAZY);
  if (!g_ugp_functions.lib_handle) {
      std::cerr << "Fail to Load library libugp_sdk.so" << std::endl; \
      assert(false && "Fail to Load library");
  }

  LINK_METHOD(Gtk_Broadway_Send_Message);
}

// end load function from libugp_sdk.so


GdkIpcChannel *GdkIpcChannel::instance_ = nullptr;

void GdkIpcChannel::Init() {
    LoadFunctionsIfNeeded();
}

GdkIpcChannel *GdkIpcChannel::Get() {
    if (instance_) {
        return instance_;
    }

    instance_ = new GdkIpcChannel();
    instance_->Init();
    return instance_;
}

void GdkIpcChannel::Gtk_Broadway_Send_Message(const BroadwayRequestBase &request) {
    g_ugp_functions.pGtk_Broadway_Send_Message(&request);
}

GdkIpcChannel::GdkIpcChannel() {
    
}

}



