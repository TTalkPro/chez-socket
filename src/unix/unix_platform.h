//
// Created by david on 11/29/24.
//

#include "../chez_socket.h"

int mark_nonblock_ioctl(int fd, int set);
int mark_nonblock_fcntl(int fd, int set);
int make_pipe(fd_t fds[2], int read_flags, int write_flags);

#ifndef UNIX_PLATFORM_H
#define UNIX_PLATFORM_H
#if defined(__APPLE__) || \
    defined(__DragonFly__) || \
    defined(__FreeBSD__) || \
    defined(__linux__) || \
    defined(__OpenBSD__) || \
    defined(__NetBSD__)
#define mark_nonblock mark_nonblock_ioctl
#else
#define mark_nonblock mark_nonblock_fcntl
#endif
#endif //UNIX_PLATFORM_H
