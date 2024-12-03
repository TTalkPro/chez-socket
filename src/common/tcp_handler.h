//
// Created by david on 12/3/24.
//

#ifndef TCP_HANDLER_H
#define TCP_HANDLER_H
#include <sys/socket.h>
#include "io_handler.h"
#include "timer_handler.h"

class tcp_handler:public io_handler, public timer_handler
{
public:
    tcp_handler(int socket, bool connected = false);
    tcp_handler() = default;
    ~tcp_handler();
    void do_connect(const struct sockaddr* addr, unsigned int addr_len);
protected:
    void handle_events(int events) final;
    void handle_timeout() override
    {
    };

private:
    void make_socket();

private:
    bool _connected = false;
    bool _bind = false;
    int _domain = AF_UNSPEC;
};

#endif // TCP_HANDLER_H
