//
// Created by david on 11/28/24.
//

#ifndef CHEZ_SOCKET_TIMER_HANDLER_H
#define CHEZ_SOCKET_TIMER_HANDLER_H
#include <string>
#include <iostream>
#include "chez_socket.h"
#include "base_handler.h"

class timer_handler: virtual public base_handler{
public:
    timer_handler(uint64_t millisecond,bool cycled):base_handler(), _millisecond(millisecond),_cycled(cycled){
        mark_events(EV_TIMEOUT);
    };
    explicit timer_handler(uint64_t millisecond): timer_handler(millisecond, false){};
    virtual ~timer_handler();
    uint64_t millisecond(){return _millisecond;};
    bool is_cycled(){return  _cycled;};
private:
    uint64_t _millisecond;
    bool _cycled;
};


#endif //CHEZ_SOCKET_TIMER_HANDLER_H
