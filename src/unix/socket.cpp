//
// Created by david on 12/3/24.
//
#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include "unix_platform.h"

int unix_make_socket(int& sockfd, int domain, int type, int protocol) {
    int err;

#if defined(SOCK_NONBLOCK) && defined(SOCK_CLOEXEC)
    sockfd = socket(domain, type | SOCK_NONBLOCK | SOCK_CLOEXEC, protocol);
    if (sockfd != -1)
    {
        return 0;
    }

    if (errno != EINVAL)
    {
        return errno;
    }
#endif

    sockfd = socket(domain, type, protocol);
    if (sockfd == -1)
    {
        return errno;
    }

    err = unix_nonblock(sockfd, 1);
    if (err == 0)
        err = unix_cloexec(sockfd, 1);

    if (err) {
        close(sockfd);
        return err;
    }

#if defined(SO_NOSIGPIPE)
    {
        int on = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_NOSIGPIPE, &on, sizeof(on));
    }
#endif
    return 0;
}
