//
// Created by david on 11/28/24.
//

#ifndef CZSOCKET_SELECT_ENGINE_H
#define CZSOCKET_SELECT_ENGINE_H
#include <sys/select.h>
#include "../common/io_manager.h"

class io_handler;

class select_io_manager : public io_manager
{
public:
    select_io_manager() = default;
    ~select_io_manager();
    void poll(uint64_t millisecond) override;

protected:
    bool can_add(const std::shared_ptr<io_handler>& handler) override;

private:
    fd_set _rfds;
    fd_set _wfds;
    struct timeval _tv;
};


#endif //CZSOCKET_SELECT_ENGINE_H
