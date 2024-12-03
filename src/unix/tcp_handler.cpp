//
// Created by david on 12/3/24.
//
#include "../common/tcp_handler.h"
#include "unix_platform.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>

#include "../reactor.h"

tcp_handler::tcp_handler(int socket, bool connected) {
  int err = -1;
  err = unix_cloexec(socket, 1);
  if (err) {
    throw std::runtime_error("unix cloexec error");
  }
  err = unix_nonblock(socket, 1);
  if (err) {
    throw std::runtime_error("unix nonblock error");
  }
  set_sockfd(socket);
  _connected = connected;
}

tcp_handler::~tcp_handler() {
  if (sockfd() > 2) {
    close(sockfd());
    set_sockfd(-1);
  }
}

void tcp_handler::do_connect(const struct sockaddr *addr,
                             unsigned int addr_len) {
  struct sockaddr_in6 addr6;
  void *target = (void *)addr;
  int r;
  make_socket();
  if (unix_is_ipv6_link_local(addr)) {
    memcpy(&addr6, addr, sizeof(addr6));
    if (addr6.sin6_scope_id == 0) {
      addr6.sin6_scope_id = unix_ipv6_link_local_scope_id();
      target = (void *)(&addr6);
    }
  }

  do {
    errno = 0;
    r = connect(sockfd(), (const sockaddr *)target, addr_len);
  } while (-1 == r && errno == EINTR);
  if (-1 == r && errno != 0) {
    if (errno == EINPROGRESS) {
      // attached_reactor()->add_io_handler(
      //     std::dynamic_pointer_cast<io_handler>(shared_from_this()), EV_READ);
    } else {
      throw std::runtime_error("connect error");
    }
  }
  if (0 == r) {
    _connected = true;
  }
}

void tcp_handler::handle_events(int events) {
  int err = -1;
  socklen_t err_size = sizeof(int);
  if (!_connected) {
    getsockopt(sockfd(), SOL_SOCKET, SO_ERROR, &err, &err_size);
    if (err == EINPROGRESS) {
      return;
    }
    _connected = true;
  }
}

void tcp_handler::make_socket() {
  int fd = -1;
  if (sockfd() > 0) {
    return;
  }
  // 只有没有socket的时候，才会创建socket
  // 并且需要是AF_INET或者AF_INET6
  if (_domain != AF_INET && _domain != AF_INET6) {
    throw std::runtime_error("Invalid domain");
  }
  if (sockfd() == -1) {
    if (0 != unix_make_socket(fd, _domain, SOCK_STREAM, 0)) {
      throw std::runtime_error("Failed to make socket");
    }
        set_sockfd(fd);

    }
}


