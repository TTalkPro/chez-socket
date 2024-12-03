//
// Created by david on 12/3/24.
//

#ifndef TCP_HANDLER_H
#define TCP_HANDLER_H
#include<string>
#include "io_handler.h"
#include "timer_handler.h"


class tcp_handler: public io_handler{
public:
    tcp_handler(int socket);
    tcp_handler() = default;
    ~tcp_handler();
private:
    void make_socket();

private:
    bool _bind = false;
    int _domain = AF_UNSPEC;
    struct sockaddr *_addr = nullptr;

};



#endif //TCP_HANDLER_H
