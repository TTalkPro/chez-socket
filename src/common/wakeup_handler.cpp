//
// Created by david on 12/3/24.
//
#include "wakeup_handler.h"
#include "../chez_socket.h"
#include "../reactor.h"

void wakeup_handler::handle_events(int events) {
  std::cout << "wakeup_handler::handle_events:" << events << std::endl;
  if (events & EV_READ) {
    handle_wakeup();
  }
}
void wakeup_handler::intialize(const std::shared_ptr<reactor>& r) {
  r->attach_handler(shared_from_this());
}
