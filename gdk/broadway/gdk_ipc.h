#ifndef GDK_IPC_H
#define GDK_IPC_H

#include <cassert>
#include <cstdlib>
#include <string>


namespace ugp {

struct BroadwayRequestBase
{
    uint32_t size;
    uint32_t serial;
    uint32_t type;
};


class GdkIpcChannel
{
public: 
    static GdkIpcChannel *Get();

    void Gtk_Broadway_Send_Message(const BroadwayRequestBase &request);

private:
    GdkIpcChannel();
    void Init();
    static GdkIpcChannel *instance_;
};

}

#endif //GDK_IPC_H