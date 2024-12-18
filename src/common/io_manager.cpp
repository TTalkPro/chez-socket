//
// Created by david on 11/28/24.
//
#include <typeinfo>
#include "io_manager.h"

#include "../chez_socket.h"
#include "io_handler.h"

bool io_manager::add_handler(const std::shared_ptr<io_handler> &handler) {
  bool ret = false;
  if (handler->sockfd() != -1) {
    int fd = handler->sockfd();
    std::cout << "adding handler " << fd  << std::endl;
    maybe_resize(fd);
    if (handler->events() & EV_IO == 0) {
      //说明是个全新的
      if (can_add(handler)) {
        _pendings.insert(handler);
        ret = true;
      }
    } else {
      _pendings.insert(handler);
      ret = true;
    }
  }
  return ret;
}

void io_manager::remove_handler(const std::shared_ptr<io_handler> &handler) {
  int fd = handler->sockfd();
  if (fd != -1) {
    if ( fd < _handlers.size()) {
      //说明已经在_handlers中了
      _handlers[fd].reset();
    }else {
      // 在pendings的状态下就被移除了
      // 正常情况下是不应该发生的
      pending_set::iterator it = _pendings.find(handler);
      if (it != _pendings.end()) {
        _pendings.erase(it);
      }
    }
  }
}

void io_manager::maybe_resize(int max_fd) {
  if (_handlers.capacity() < max_fd + 1) {
    _handlers.resize(max_fd + 1);
  }
}