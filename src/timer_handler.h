//
// Created by david on 11/28/24.
//

#ifndef CHEZ_SOCKET_TIMER_HANDLER_H
#define CHEZ_SOCKET_TIMER_HANDLER_H
#include <string>
#include <iostream>
#include "chez_socket.h"
#include "base_handler.h"

class timer_handler : virtual public base_handler
{
public:
    timer_handler(uint64_t millisecond, bool cycled): base_handler(), _millisecond(millisecond), _cycled(cycled)
    {
    };

    explicit timer_handler(uint64_t millisecond): timer_handler(millisecond, false)
    {
    };

    explicit timer_handler(): timer_handler(0, false)
    {
    };
    virtual ~timer_handler();
    uint64_t millisecond() { return _millisecond; };
    bool cycled() { return _cycled; };

    bool set_cycled(bool cycled)
    {
        bool _old = _cycled;
        _cycled = cycled;
        return _old;
    };

    uint64_t set_millisecond(uint64_t millisecond)
    {
        uint64_t _old = _millisecond;
        _millisecond = millisecond;
        return _old;
    };

private:
    uint64_t _millisecond;
    bool _cycled;
};


#endif //CHEZ_SOCKET_TIMER_HANDLER_H
