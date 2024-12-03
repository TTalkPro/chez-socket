#include <cerrno>
#include <fcntl.h>
#include <sys/ioctl.h>


#include "unix_platform.h"
int unix_cloexec(int fd,int set) {
    int r = 0;
    int flags = 0;
    do
    {
        r = fcntl(fd, F_GETFL);
    }
    while (r == -1 && errno == EINTR);
    if (!!(r & FD_CLOEXEC) == !!set)
    {
        return 0;
    }

    if (set)
    {
        flags = r | FD_CLOEXEC;
    }
    else
    {
        flags = r & ~FD_CLOEXEC;
    }
    do
    {
        r = fcntl(fd, F_SETFD, flags);
    }
    while (r == -1 && errno == EINTR);

    if (r)
    {
        return errno;
    }

    return 0;
}

int unix_nonblock_ioctl(int fd, int set)
{
    int r;

    do
    {
        r = ioctl(fd, FIONBIO, &set);
    }while (r == -1 && errno == EINTR);

    if (r)
    {
        return errno;
    }

    return 0;
}


int unix_nonblock_fcntl(int fd, int set)
{
    int flags;
    int r;

    do
    {
        r = fcntl(fd, F_GETFL);
    }
    while (r == -1 && errno == EINTR);

    if (r == -1)
    {
        return errno;
    }

    if (!!(r & O_NONBLOCK) == !!set)
    {
        return 0;
    }

    if (set)
    {
        flags = r | O_NONBLOCK;
    }
    else
    {
        flags = r & ~O_NONBLOCK;
    }

    do
        r = fcntl(fd, F_SETFL, flags);
    while (r == -1 && errno == EINTR);

    if (r)
    {
        return errno;
    }

    return 0;
}



