//
// Created by david on 11/28/24.
//
#include <typeinfo>
#include "base_engine.h"

#include "chez_socket.h"
#include "io_handler.h"

bool base_engine::add_handler(const std::shared_ptr<io_handler>& handler)
{
    bool ret = false;
    if (handler->fd() != -1)
    {
        int fd = handler->fd();
        maybe_resize(fd);
        if (handler->events() & EV_IO == 0)
        {
            //说明是个全新的
            if (can_add(handler))
            {
                _pendings.insert(handler);
                ret = true;
            }
        }
        else
        {
            _pendings.insert(handler);
            ret = true;
        }
    }
    return ret;
}

void base_engine::remove_handler(const std::shared_ptr<io_handler>& handler)
{
    if (handler->fd() != -1)
    {
        int fd = handler->fd();
        _handlers[fd].reset();
        if (fd == _max_fd)
        {
            _max_fd = -1;
        }
    }
}

void base_engine::maybe_resize(int max_fd)
{
    if (_handlers.capacity() < max_fd + 1)
    {
        _handlers.resize(max_fd + 1);
    }
}
