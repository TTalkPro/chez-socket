//
// Created by david on 11/28/24.
//

#ifndef CZSOCKET_BASE_ENGINE_H
#define CZSOCKET_BASE_ENGINE_H
#include <sys/socket.h>

#include <map>
#include <set>
#include <vector>
#include <memory>

class io_handler;

class base_engine {
public:
    base_engine() = default;
    virtual ~base_engine() = default;
    bool add_handler(const std::shared_ptr<io_handler>& handler);
    void remove_handler(const std::shared_ptr<io_handler>& handler);
    virtual void wakeup() = 0;
    virtual void poll(uint64_t millisecond ) = 0;
protected:
    virtual bool can_add(const std::shared_ptr<io_handler>& handler) = 0;
    typedef std::set<std::shared_ptr<io_handler>,std::owner_less<>> changes_set;
    changes_set _changes;
    typedef std::vector<std::weak_ptr<io_handler>> handlers_vector;
    handlers_vector _handlers;
    void maybe_resize(int max_fd);
};


#endif //CZSOCKET_BASE_ENGINE_H
