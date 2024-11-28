//
// Created by david on 11/28/24.
//
#include <memory>
#include "chez_socket.h"
#include "base_handler.h"
#include "io_handler.h"
#include "timer_handler.h"
#include "select_engine.h"
#include "timer_manager.h"
#include "reactor.h"


reactor::reactor(){
    _engine = std::make_shared<select_engine>();
    _timer_manager = std::make_shared<timer_manager>();
    _loop.store(true);
}
reactor::~reactor() {
    _engine = nullptr;
    _timer_manager = nullptr;
}

bool reactor::add_handler(const std::shared_ptr<base_handler>& handler) {
    int _events = handler->events();
    std::cout << "add handler with events: " << _events << std::endl;
    int result = true;
    if(_events & EV_TIMEOUT ){
        result =  _timer_manager->add_handler(std::dynamic_pointer_cast<timer_handler>(handler));
    }
    if((_events & EV_READ)
        || (_events & EV_WRITE)){
        result =  (result && _engine->add_handler(std::dynamic_pointer_cast<io_handler>(handler)));
    }
    if(result){
        handler->attach(this);
    }
    return result;
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
        _timer_handler->handle_events(EV_TIMEOUT);
    }
    return _item.remain_millisecond;
}
void reactor::run() {
    uint64_t _waiting = 0;
    while(_loop.load()) {
        _waiting = run_timer_task();
        _engine->poll(_waiting);
        run_timer_task();
    }
}
