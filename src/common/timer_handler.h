//
// Created by david on 11/28/24.
//

#ifndef CHEZ_SOCKET_TIMER_HANDLER_H
#define CHEZ_SOCKET_TIMER_HANDLER_H
#include <string>
#include <iostream>
#include "../chez_socket.h"
#include "base_handler.h"

class timer_handler : virtual public base_handler
{
public:
    explicit timer_handler()=default;
    virtual ~timer_handler();
    virtual void handle_timer(){};
};


#endif //CHEZ_SOCKET_TIMER_HANDLER_H
