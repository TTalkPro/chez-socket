
#include "timer_handler.h"

timer_handler::~timer_handler()
{
    detach_reactor();
}
