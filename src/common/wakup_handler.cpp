//
// Created by david on 12/3/24.
//

#include <cstring>

#include "wakeup_handler.h"
#include "../reactor.h"

void wakeup_handler::handle_timeout()
{
    io_handler::handle_timeout();
}


void wakeup_handler::handle_io(int events)
{
    if (events & EV_READ)
    {
        handle_wakeup();
    }
}
void wakeup_handler::handle_reactor_attached()
{
    attached_reactor()->add_io_handler(std::dynamic_pointer_cast<io_handler>(shared_from_this()),EV_READ);
}
