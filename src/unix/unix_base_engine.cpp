//
// Created by david on 11/29/24.
//
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include "unix_platform.h"
#include "unix_base_engine.h"

#include <cstring>
#include <sys/socket.h>

#include "../common/wakeup_handler.h"


unix_base_engine::unix_base_engine()
{
    if (0 != unix_make_pipe(_wakeup_fd,O_NONBLOCK,O_NONBLOCK))
    {
        throw std::runtime_error("Failed to create pipe for unix_base_engine");
    }
}
unix_base_engine::~unix_base_engine()
{
    if (_wakeup_fd[0] > 0)
    {
        close(_wakeup_fd[0]);
    }
    if (_wakeup_fd[1] > 0)
    {
        close(_wakeup_fd[1]);
    }
}

void unix_base_engine::wakeup()
{
    wakeup(NULL,NULL,0);
}
void unix_base_engine::wakeup(wakeup_handler *handler, void *data, size_t length)
{
    int r = 0;
    wakeup_msg msg;
    memset(&msg, 0, sizeof(wakeup_msg));
    msg.data = data;
    msg.length = length;
    msg.handler = handler;
    do {
        r = write(_wakeup_fd[1], &msg, sizeof(msg));
    } while (r == -1 && errno == EINTR);
}

void unix_base_engine::handle_wakeup()
{
    int r;
    wakeup_msg msg;
    memset(&msg, 0, sizeof(wakeup_msg));
    do {
        r = read(_wakeup_fd[0], &msg, sizeof(msg));
    } while (r < 0 && errno == EINTR);
    if (msg.handler != NULL)
    {
        msg.handler->handle_wakeup(msg);
    }
}


