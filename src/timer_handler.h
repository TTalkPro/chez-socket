//
// Created by david on 11/28/24.
//

#ifndef CHEZ_SOCKET_TIMER_HANDLER_H
#define CHEZ_SOCKET_TIMER_HANDLER_H
#include <string>
#include <iostream>


class timer_handler {
public:
    timer_handler() = default;
    virtual ~timer_handler();
    virtual void handle_timeout();
};


#endif //CHEZ_SOCKET_TIMER_HANDLER_H
