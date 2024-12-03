//
// Created by david on 12/3/24.
//

#ifndef UNIX_WAKEUP_HANDLER_H
#define UNIX_WAKEUP_HANDLER_H
#include "../common/wakeup_handler.h"


class unix_wakeup_handler: public wakeup_handler{
public:
    unix_wakeup_handler();
    ~unix_wakeup_handler();
    void wakeup() final;
    void wakeup(wakeup_operation* operation, void* data,size_t length) final;
private:
    void handle_wakeup() final;
    //_wakeup_fd[0]是读取端
    //_wakeup_fd[1]是写入端
    fd_t _wakeup_fd[2];
};



#endif //UNIX_WAKEUP_HANDLER_H
