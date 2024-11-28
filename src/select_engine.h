//
// Created by david on 11/28/24.
//

#ifndef CZSOCKET_SELECT_ENGINE_H
#define CZSOCKET_SELECT_ENGINE_H
#include "base_engine.h"
#include <sys/select.h>
#include <map>

class io_handler;

class select_engine: public base_engine {
public:
    select_engine() = default;
    ~select_engine();
    void wakeup()  override;
    void poll(uint64_t millisecond) override;
protected:
    bool can_add(const std::shared_ptr<io_handler>& handler) override;
private:
    fd_set _rfds;
    fd_set _wfds;
    struct timeval  _tv;


};


#endif //CZSOCKET_SELECT_ENGINE_H
