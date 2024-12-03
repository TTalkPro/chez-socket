//
// Created by david on 11/29/24.
//

#ifndef WAKEUP_HANDLER_H
#define WAKEUP_HANDLER_H
#include <memory>
#include "io_handler.h"
class wakeup_operation;
struct wakeup_msg
{
    wakeup_operation* operation;
    size_t length;
    void* data;
};

class wakeup_handler: protected io_handler {
public:
    wakeup_handler() = default;
    virtual ~wakeup_handler() = default;
    virtual void wakeup() = 0;
    virtual void wakeup(wakeup_operation* operation, void* data,size_t length) = 0;
protected:
    virtual void handle_wakeup() = 0;
    void handle_reactor_attached() final;
    void handle_io(int events) final;
    void handle_timeout() final;
};



#endif //WAKEUP_HANDLER_H
