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
class reactor;

class io_manager
{
    friend class reactor;
public:
    io_manager() = default;
    virtual ~io_manager() = default;
    bool add_handler(const std::shared_ptr<io_handler>& handler);
    void remove_handler(const std::shared_ptr<io_handler>& handler);
    virtual void poll(uint64_t millisecond) = 0;

protected:
    virtual void attach_to_reactor(const std::shared_ptr<reactor> &r) {
        _intance = r;
    };
    void maybe_resize(int max_fd);
    virtual bool can_add(const std::shared_ptr<io_handler>& handler) = 0;
    typedef std::set<std::shared_ptr<io_handler>, std::owner_less<>> pending_set;
    pending_set _pendings;
    typedef std::vector<std::weak_ptr<io_handler>> handlers_vector;
    handlers_vector _handlers;
    std::weak_ptr<reactor> _intance;
};


#endif //CZSOCKET_BASE_ENGINE_H
