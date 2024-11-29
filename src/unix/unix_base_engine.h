//
// Created by david on 11/29/24.
//

#ifndef UNIX_BASE_ENGINE_H
#define UNIX_BASE_ENGINE_H
#include "../chez_socket.h"
#include "../common//base_engine.h"
#include "unix_platform.h"


class unix_base_engine: virtual  public  base_engine{
public:
    unix_base_engine() ;
    ~unix_base_engine() override ;
    void wakeup()  final;
    void wakeup(wakeup_handler* handler, void* data,size_t length) final;
protected:
    void handle_wakeup();
protected:
    //_wakeup_fd[0]是读取端
    //_wakeup_fd[1]是写入端
    fd_t _wakeup_fd[2];
};



#endif //UNIX_BASE_ENGINE_H
