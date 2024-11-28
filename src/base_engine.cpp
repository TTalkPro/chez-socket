//
// Created by david on 11/28/24.
//
#include <typeinfo>
#include "base_engine.h"
#include "io_handler.h"

bool base_engine::add_handler(const std::shared_ptr<io_handler>& handler) {
    if(handler->fd() != -1) {
        int fd = handler->fd();
        maybe_resize(fd);
        std::weak_ptr<io_handler> _handler = _handlers[fd];
        if(_handler.lock() == nullptr) {
            // 说明是个全新的handler
            if(can_add(handler)){
                _handlers[fd] = handler;
                _changes.insert(handler);
                return true;
            }
            return false;
        }else {
            //已经注册到完成了，所以之需要更新_changes就可以了
            _changes.insert(handler);
            return true;
        }

    }
    return false;
}

void base_engine::remove_handler(const std::shared_ptr<io_handler>& handler){
    if(handler->fd() != -1){
        int fd = handler->fd();
        _handlers[fd].reset();
        _changes.insert(handler);
    }
}

void base_engine::maybe_resize(int max_fd) {
    if(_handlers.capacity() < max_fd + 1){
        _handlers.resize(max_fd+1);
    }
}