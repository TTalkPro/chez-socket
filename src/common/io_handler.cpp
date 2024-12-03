//
// Created by david on 11/28/24.
//
#include <iostream>

#include "io_handler.h"

void io_handler::watch(int events) {
  _pending_events |= events;
};

void io_handler::unwatch(int events) {
  _pending_events &= ~events;
};

void io_handler::handle_io(int events) {
  std::cout << "io_handler::handle_io:" << events << std::endl;
  handle_events(events);
}
