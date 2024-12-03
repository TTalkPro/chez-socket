//
// Created by david on 11/29/24.
//

#ifndef WAKEUP_HANDLER_H
#define WAKEUP_HANDLER_H
#include "io_handler.h"
#include <memory>

#include "../reactor.h"
class wakeup_operation;
class reactor;
struct wakeup_msg {
  wakeup_operation *operation;
  size_t length;
  void *data;
};

class wakeup_handler: public io_handler {
  friend class reactor;
public:
  wakeup_handler() = default;
  virtual ~wakeup_handler() = default;
  virtual void wakeup() = 0;
  virtual void wakeup(wakeup_operation *operation, void *data,
                      size_t length) = 0;
protected:
  virtual void intialize(const std::shared_ptr<reactor>& r);
  void handle_events(int events) final;
  virtual void handle_wakeup() = 0;

};

#endif // WAKEUP_HANDLER_H
