//
// Created by david on 11/28/24.
//
#include "select_io_manager.h"
#include "../chez_socket.h"
#include "../common/io_handler.h"

bool select_io_manager::can_add(const std::shared_ptr<io_handler> &handler) {
  if (handler->sockfd() < FD_SETSIZE) {
    return true;
  }
  return false;
}

void select_io_manager::poll(uint64_t millisecond) {
  int res = 0;
  int events = 0;
  int nfds = -1;
  FD_ZERO(&_rfds);
  FD_ZERO(&_wfds);
  std::shared_ptr<io_handler> _handler;
  // 处理等待中的socket
  if (_pendings.size() > 0) {
    pending_set::iterator it = _pendings.begin();
    int fd = 0;
    while (it != _pendings.end()) {
      fd = (*it)->sockfd();
      if (fd == -1) {
        it++;
        continue;
      }
      _handlers[fd] = (*it);
      it++;
    }
    _pendings.clear();
  }
  for (int i = 0; i < _handlers.capacity(); i++) {
    _handler = _handlers[i].lock();
    if (_handler != nullptr) {
      _handler->apply_pending_events();
      events = _handler->events();
      std::cout << "apply_pending_events: " << events << " on: " << i << std::endl;
      if (events & EV_READ) {
        FD_SET(i, &_rfds);
      }
      if (events & EV_WRITE) {
        FD_SET(i, &_wfds);
      }
      nfds = i;
    }
  }
  if (millisecond > 0) {
    _tv.tv_usec = millisecond * 1000;
    res = select(nfds + 1, &_rfds, &_wfds, NULL, &_tv);
  } else {
    res = select(nfds + 1, &_rfds, &_wfds, NULL, NULL);
  }
  if (res > 0) {

    for (int i = 0; i <= nfds; i++) {
      events = 0;
      if (FD_ISSET(i, &_rfds)) {
        events |= EV_READ;
      }
      if (FD_ISSET(i, &_wfds)) {
        events |= EV_WRITE;
      }
      if (events != 0) {
        _handler = _handlers[i].lock();
        if (_handler != nullptr) {
          _handler->handle_io(events);
        }
      }
    }
    }
}
