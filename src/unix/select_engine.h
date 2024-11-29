//
// Created by david on 11/28/24.
//

#ifndef CZSOCKET_SELECT_ENGINE_H
#define CZSOCKET_SELECT_ENGINE_H
#include <sys/select.h>
#include "unix_base_engine.h"

class io_handler;

class select_engine : public unix_base_engine
{
public:
    select_engine() = default;
    ~select_engine();
    void poll(uint64_t millisecond) override;

protected:
    bool can_add(const std::shared_ptr<io_handler>& handler) override;

private:
    fd_set _rfds;
    fd_set _wfds;
    struct timeval _tv;
};


#endif //CZSOCKET_SELECT_ENGINE_H
