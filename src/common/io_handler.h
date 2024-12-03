//
// Created by david on 11/28/24.
//

#ifndef CZSOCKET_IO_HANDLER_H
#define CZSOCKET_IO_HANDLER_H
#include <iostream>
#include <memory>
#include "timer_handler.h"
class io_manager;
class reactor;

class io_handler: public timer_handler
{
public:
    io_handler() = default;
    virtual ~io_handler() = default;
    void watch(int events)
    {
        _pending_events |= events;
    };
    void unwatch(int events)
    {
        _pending_events &= ~events;
    };
    int events() const { return _events; };
    int pending_events() const { return _pending_events; };
    void apply_pending_events() { _events = _pending_events; };
    int fd() { return _fd; };
    virtual void handle_io(int events) = 0;
protected:

    void set_fd(int fd) { _fd = fd; };

private:
    int _fd = -1;
    int _pending_events = 0;
    int _events = 0 ;
};

#endif //CZSOCKET_IO_HANDLER_H
