//
// Created by david on 11/28/24.
//
#include "chez_socket.h"
#include "common/io_handler.h"
#include "common/io_manager.h"
#include "common/timer_handler.h"
#include "common/timer_manager.h"
#include "common/wakeup_operation.h"
#include <memory>

#if defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) ||      \
    defined(__DragonFly__) || defined(__NetBSD__)
#include "unix/select_io_manager.h"
#include "unix/unix_wakeup_handler.h"
#endif

#include "reactor.h"


thread_local std::shared_ptr<reactor> reactor::_instance = nullptr;
thread_local std::mutex reactor::_instance_guard;

std::shared_ptr<reactor> &reactor::instance() {
  if (_instance == nullptr) {
    std::unique_lock<std::mutex> lock(_instance_guard);
    if (_instance == nullptr) {
      _instance = std::shared_ptr<reactor>(new reactor());
      _instance->_thread_id = std::this_thread::get_id();
      _instance->initialize();
    }
  }
  return _instance;
}

bool reactor::in_thread() {
  return (_thread_id == std::this_thread::get_id());
}

reactor::reactor() {
  _io_manager = std::make_unique<select_io_manager>();
  _timer_manager = std::make_unique<timer_manager>();
  _wakeup_handler = std::make_shared<unix_wakeup_handler>();
  _loop.store(true);
}

reactor::~reactor() {
  _io_manager = nullptr;
  _timer_manager = nullptr;
}

void reactor::initialize() {
  if (_wakeup_handler) {
    _wakeup_handler->intialize(shared_from_this());
  }
}
void reactor::attach_handler(const std::shared_ptr<base_handler>& handler) {
  handler->attach(shared_from_this());
}
void reactor::wakeup() {
  if (_wakeup_handler) {
    _wakeup_handler->wakeup();
  }
}

void reactor::wakeup(wakeup_operation *operation, void *data, size_t size) {
  if (_wakeup_handler) {
    _wakeup_handler->wakeup(operation, data, size);
  }
}


bool reactor::add_timer_handler(const std::shared_ptr<base_handler> &handler,
                                uint64_t millisecond, bool is_cycled) {
  bool ret = _timer_manager->add_handler(
      std::dynamic_pointer_cast<timer_handler>(handler), millisecond,
      is_cycled);
  return ret;
}

bool reactor::add_io_handler(const std::shared_ptr<base_handler> &handler,
                             int events) {
  std::shared_ptr<io_handler> h = std::dynamic_pointer_cast<
    io_handler>(handler);
  int _events = events & EV_IO;
  int ret = _io_manager->add_handler(h);
  if (ret) {
    h->watch(_events);
  }
  return ret;
}

void reactor::remove_timer_handler(
    const std::shared_ptr<base_handler> &handler) {
  _timer_manager->remove_handler(
      std::dynamic_pointer_cast<timer_handler>(handler));
}

void reactor::remove_io_handler(const std::shared_ptr<base_handler> &handler,
                                int events) {
  std::shared_ptr<io_handler> h = std::dynamic_pointer_cast<
    io_handler>(handler);
  int _events = events & EV_IO;
  h->unwatch(_events);
  if (h->pending_events() == 0) {
    _io_manager->remove_handler(h);
  }
}

uint64_t reactor::run_timer_task() {
  timer_item _item;
  _item.remain_millisecond = 0;
  while (_timer_manager->has_timer_item()) {
    _timer_manager->get_latest_item(_item);
    if (_item.remain_millisecond > 0) {
      break;
    }
    _timer_manager->remove_first();
    const std::shared_ptr<timer_handler> _timer_handler = _item.handler.lock();
    if (_timer_handler == nullptr) {
      continue;
    }
    _timer_handler->handle_timer();
  }
  return _item.remain_millisecond;
}

void reactor::run() {
  uint64_t _waiting = 0;
  while (_loop.load()) {
    _waiting = run_timer_task();
    if (_loop.load()) {
      _io_manager->poll(_waiting);
    }
  }
}