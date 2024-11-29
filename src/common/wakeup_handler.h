//
// Created by david on 11/29/24.
//

#ifndef WAKEUP_HANDLER_H
#define WAKEUP_HANDLER_H
#include <memory>
#include "base_handler.h"
class wakeup_handler;
struct wakeup_msg
{
    wakeup_handler* handler;
    size_t length;
    void* data;
};

class wakeup_handler: virtual public base_handler {
    friend class reactor;
public:
  wakeup_handler() = default;
  virtual ~wakeup_handler() = default;
  virtual void handle_wakeup(const wakeup_msg& msg) {

  };

};



#endif //WAKEUP_HANDLER_H
