//
// Created by david on 12/3/24.
//

#include "unix_platform.h"
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int unix_make_socket(int &sockfd, int domain, int type, int protocol) {
  int err;

#if defined(SOCK_NONBLOCK) && defined(SOCK_CLOEXEC)
  sockfd = socket(domain, type | SOCK_NONBLOCK | SOCK_CLOEXEC, protocol);
  if (sockfd != -1) {
    return 0;
  }

  if (errno != EINVAL) {
    return errno;
  }
#endif

  sockfd = socket(domain, type, protocol);
  if (sockfd == -1) {
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

int unix_is_ipv6_link_local(const struct sockaddr *addr) {
  const struct sockaddr_in6 *a6;
  uint8_t b[2];

  if (addr->sa_family != AF_INET6)
    return 0;

  a6 = (const struct sockaddr_in6 *)addr;
  memcpy(b, &a6->sin6_addr, sizeof(b));

  return b[0] == 0xFE && b[1] == 0x80;
}
int unix_ipv6_link_local_scope_id() {
  int scop_id = 0;
  struct sockaddr_in6 *a6;
  struct ifaddrs *ifa;
  struct ifaddrs *p;

  if (getifaddrs(&ifa)) {
    return 0;
  }
  for (p = ifa; p != NULL; p = p->ifa_next) {
    if (p->ifa_addr != NULL) {
      if (unix_is_ipv6_link_local(p->ifa_addr)) {
        break;
      }
    }
  }

  if (p != NULL) {
    a6 = (struct sockaddr_in6 *)p->ifa_addr;
    scop_id = a6->sin6_scope_id;
  }

  freeifaddrs(ifa);

  return scop_id;
}
