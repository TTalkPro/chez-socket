//
// Created by david on 11/28/24.
//

#ifndef CZSOCKET_BASE_HANDLER_H
#define CZSOCKET_BASE_HANDLER_H

#include <memory>
#include <iostream>
class reactor;

class base_handler {
    friend class reactor;
public:
    base_handler() = default;
    void watch(int events){_events = _events | events;}
    void unwatch(int events){_events = _events ^ events;}
    virtual ~base_handler();
    virtual void handle_events(int events) {}
    int events() const { return _events; }
protected:
    reactor* attached_reactor() const {return _reactor;}
    void attach_reactor(reactor* r) {_reactor = r;}
    void detach_reactor() {_reactor = nullptr;}
private:
    reactor* _reactor;
    int _events;
};

#endif //CZSOCKET_BASE_HANDLER_H
