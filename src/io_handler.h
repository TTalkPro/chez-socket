//
// Created by david on 11/28/24.
//

#ifndef CZSOCKET_IO_HANDLER_H
#define CZSOCKET_IO_HANDLER_H

#include <sys/socket.h>
#include <iostream>

#include "base_handler.h"

class io_handler : virtual public base_handler {
public:
    io_handler(): io_handler(-1,0){};
    io_handler(int socket, int events):base_handler(),_fd(socket){
        std::cout << "io_handler: "<< events <<std::endl;
        mark_events(events);
    };
    virtual ~io_handler();
    int fd() { return _fd; };
private:
    int _fd;
};


#endif //CZSOCKET_IO_HANDLER_H
