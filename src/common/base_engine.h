//
// Created by david on 11/28/24.
//

#ifndef CZSOCKET_BASE_ENGINE_H
#define CZSOCKET_BASE_ENGINE_H

#include <set>
#include <vector>
#include <memory>
#include "../chez_socket.h"
class io_handler;
class wakeup_handler;

class base_engine
{
public:
    base_engine() = default;
    virtual ~base_engine() = default;
    bool add_handler(const std::shared_ptr<io_handler>& handler);
    void remove_handler(const std::shared_ptr<io_handler>& handler);
    virtual void wakeup() = 0;
    virtual void wakeup(wakeup_handler* handler, void* data,size_t length) = 0;
    virtual void poll(uint64_t millisecond) = 0;
protected:
    void maybe_resize(int max_fd);
    virtual bool can_add(const std::shared_ptr<io_handler>& handler) = 0;
    typedef std::set<std::shared_ptr<io_handler>, std::owner_less<>> pending_set;
    pending_set _pendings;
    typedef std::vector<std::weak_ptr<io_handler>> handlers_vector;
    handlers_vector _handlers;
    fd_t _wakeup_fd[2];

};


#endif //CZSOCKET_BASE_ENGINE_H
