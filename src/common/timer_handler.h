//
// Created by david on 11/28/24.
//

#ifndef CHEZ_SOCKET_TIMER_HANDLER_H
#define CHEZ_SOCKET_TIMER_HANDLER_H

#include <memory>
#include "../chez_socket.h"
class reactor;

class timer_handler: public  std::enable_shared_from_this<timer_handler>
{
    friend class reactor;
public:
    explicit timer_handler()=default;
    virtual ~timer_handler();
    virtual void handle_timeout() = 0 ;
protected:
    virtual void handle_reactor_attached() =0;
    std::shared_ptr<reactor> attached_reactor() const { return _reactor; };
    void attach_reactor(const std::shared_ptr<reactor>& r)
    {
        _reactor = r;
        handle_reactor_attached();
    };
    void detach_reactor() { _reactor = nullptr; };
private:
    std::shared_ptr<reactor> _reactor;
};


#endif //CHEZ_SOCKET_TIMER_HANDLER_H
