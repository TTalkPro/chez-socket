//
// Created by david on 11/28/24.
//

#include "timer_handler.h"
timer_handler::~timer_handler() {
}

void timer_handler::handle_timeout() {
    std::cout << "the timer_handler handle timeout" << std::endl;
}
