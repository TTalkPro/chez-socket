//
// Created by david on 12/3/24.
//
#include <unistd.h>

#include <sys/socket.h>
#include "unix_platform.h"
#include "../common/tcp_handler.h"


tcp_handler::tcp_handler(int socket)
{
    int err = -1;
    err = unix_cloexec(socket,1);
    if (err)
    {
        throw std::runtime_error("unix cloexec error");
    }
    err = unix_nonblock(socket,1);
    if (err)
    {
        throw std::runtime_error("unix nonblock error");
    }
    set_fd(socket);
}

tcp_handler::~tcp_handler()
{
    if (fd() > 2)
    {
        close(fd());
        set_fd(-1);
    }
}

void tcp_handler::make_socket()
{
    int fd = -1;
    if (_domain != AF_INET && _domain != AF_INET6 && _domain != AF_UNSPEC)
    {
        throw std::runtime_error("Invalid domain");
    }
    if (_domain != AF_UNSPEC) {
        if (0 != unix_make_socket(fd,_domain,SOCK_STREAM, 0))
        {
            throw std::runtime_error("Failed to make socket");
        }
        set_fd(fd);
    }

}
