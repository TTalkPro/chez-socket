//
// Created by david on 11/29/24.
//
#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

#include "../chez_socket.h"
#include "UnixPlatform.h"

int unix_make_pipe(int fds[2], int read_flags, int write_flags) {
  int temp[2];
  int err;
#if defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) ||      \
    defined(__DragonFly__) || defined(__NetBSD__)
  int flags = O_CLOEXEC;

  if ((read_flags & O_NONBLOCK) && (write_flags & O_NONBLOCK))
    flags |= O_NONBLOCK;

  if (pipe2(temp, flags))
    return errno;
  // 如果都是非阻塞的，此时可以结束了
  if (flags & O_NONBLOCK) {
    fds[0] = temp[0];
    fds[1] = temp[1];
    return 0;
  }
#else
  if (pipe(temp))
    return errno;

  if ((err = mark_cloexec(temp[0], 1)))
    goto fail;

  if ((err = mark_cloexec(temp[1], 1)))
    goto fail;
#endif

  if (read_flags & O_NONBLOCK)
    if ((err = unix_nonblock(temp[0], 1)))
      goto fail;

  if (write_flags & O_NONBLOCK)
    if ((err = unix_nonblock(temp[1], 1)))
      goto fail;

  fds[0] = temp[0];
  fds[1] = temp[1];
  return 0;

fail:
  close(temp[0]);
  close(temp[1]);
        return err;
}