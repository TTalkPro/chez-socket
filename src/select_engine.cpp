//
// Created by david on 11/28/24.
//
#include "chez_socket.h"
#include "io_handler.h"
#include "select_engine.h"

select_engine::~select_engine() noexcept {

}

void select_engine::wakeup()  {

}

bool select_engine::can_add(const std::shared_ptr<io_handler>& handler) {
    if(handler->fd() < FD_SETSIZE){
        return true;
    }
    return false;
}
void select_engine::poll(uint64_t millisecond) {
    int _res = 0;
    int _events = 0;
    int _nfds = 0;
    FD_ZERO(&_rfds);
    FD_ZERO(&_wfds);
    _changes.clear();
    std::shared_ptr<io_handler> _handler;
    for(int i = 0; i < _handlers.capacity(); i++){
        _handler = _handlers[i].lock();
        if(_handler != nullptr){
            _events = _handler->events();
            if(_events & EV_READ){
                FD_SET(i,&_rfds);
            }
            if(_events & EV_WRITE){
                FD_SET(i,&_wfds);
            }
            _nfds = i;
        }
    }
    if(millisecond > 0){
        _tv.tv_usec = millisecond * 1000;
        _res = select(_nfds,&_rfds,&_wfds,NULL,&_tv);
    }else {
        _res = select(_nfds,&_rfds,&_wfds,NULL,NULL);
    }
    if(_res > 0 ) {
        for (int i = 0; i <= _nfds; i++) {
            _events = 0;
            if(FD_ISSET(i, &_rfds)){
                _events |= EV_READ;
            }
            if(FD_ISSET(i,&_wfds)){
                _events |= EV_WRITE;
            }
            if(_events != 0){
                _handler = _handlers[i].lock();
                if(_handler != nullptr){
                    _handler->handle_events(_events);
                }
            }
        }
    }

}