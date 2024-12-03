//
// Created by david on 12/3/24.
//

#ifndef WAKEUP_OPERATION_H
#define WAKEUP_OPERATION_H
#include <memory>
#include<unistd.h>

class wakeup_operation: public std::enable_shared_from_this<wakeup_operation>{
public:
    wakeup_operation() = default;
    ~wakeup_operation() = default;
    virtual void handle_wakeup(void* data, size_t size);
};

#endif //WAKEUP_OPERATION_H
