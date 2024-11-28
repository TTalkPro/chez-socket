//
// Created by david on 11/28/24.
//

#ifndef CZSOCKET_IO_HANDLER_H
#define CZSOCKET_IO_HANDLER_H
#include <sys/socket.h>
#include <iostream>

class io_handler {
public:
    io_handler(int socket,int events): _fd(socket),_events(events){};
    virtual ~io_handler() = default;
    int fd(){return _fd;};
    int events(){return _events;};
    virtual void handle_events(int events);
private:
    int _fd;
    int _events;
};


#endif //CZSOCKET_IO_HANDLER_H
