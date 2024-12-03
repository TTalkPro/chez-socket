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
    tcp_handler(int domain):io_handler(),_domain(domain){};
    tcp_handler() = default;
    ~tcp_handler();
private:
    void make_socket();

private:
    bool _bind = false;
    int _port = -1;
    int _domain = AF_UNSPEC;

};



#endif //TCP_HANDLER_H
