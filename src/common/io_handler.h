//
// Created by david on 11/28/24.
//

#ifndef CZSOCKET_IO_HANDLER_H
#define CZSOCKET_IO_HANDLER_H
#include <iostream>
#include <memory>
#include "base_handler.h"

class io_handler :virtual public base_handler
{
public:
    io_handler() = default;
    virtual ~io_handler() = default;

    void watch(int events);
    void unwatch(int events);
    int events() const { return _events; };
    int pending_events() const { return _pending_events; };
    void apply_pending_events() { _events = _pending_events; };
    int sockfd() { return _sockfd; };
    void set_sockfd(int fd) { _sockfd = fd; };
    void handle_io(int events);

protected:
    virtual void handle_events(int events) = 0;

private:
    int _sockfd = -1;
    int _pending_events = 0;
    int _events = 0;
};

#endif //CZSOCKET_IO_HANDLER_H
