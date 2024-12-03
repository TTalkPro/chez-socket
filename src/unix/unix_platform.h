//
// Created by david on 11/29/24.
//

#include "../chez_socket.h"

int unix_cloexec(int fd,int set);
int unix_nonblock_ioctl(int fd, int set);
int unix_nonblock_fcntl(int fd, int set);
int unix_make_pipe(int fds[2], int read_flags, int write_flags);
int unix_make_socket(int& sockfd, int domain, int type, int protocol);
int unix_is_ipv6_link_local(const struct sockaddr* addr);
int unix_ipv6_link_local_scope_id();

#ifndef UNIX_PLATFORM_H
#define UNIX_PLATFORM_H
#if defined(__APPLE__) || \
    defined(__DragonFly__) || \
    defined(__FreeBSD__) || \
    defined(__linux__) || \
    defined(__OpenBSD__) || \
    defined(__NetBSD__)
#define unix_nonblock unix_nonblock_ioctl
#else
#define unix_nonblock unix_nonblock_fcntl
#endif
#endif //UNIX_PLATFORM_H
