#include "timer_handler.h"

timer_handler::~timer_handler() {
}
void timer_handler::handle_timer() {
  handle_timeout();
}
