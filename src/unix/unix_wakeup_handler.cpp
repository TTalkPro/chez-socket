//
// Created by david on 12/3/24.
//
#include "unix_wakeup_handler.h"
#include "../common/wakeup_operation.h"
#include "unix_platform.h"
#include "../reactor.h"

#include <cstring>
#include <fcntl.h>
#include <unistd.h>

unix_wakeup_handler::unix_wakeup_handler() {
  if (0 != unix_make_pipe(_wakeup_fd, O_NONBLOCK, O_NONBLOCK)) {
    throw std::runtime_error("Failed to create pipe for unix_wakeup_handler");
  }
  set_sockfd(_wakeup_fd[0]);
}

unix_wakeup_handler::~unix_wakeup_handler() {
  if (_wakeup_fd[0] > 0) {
    close(_wakeup_fd[0]);
  }
  if (_wakeup_fd[1] > 0) {
    close(_wakeup_fd[1]);
  }
}

void unix_wakeup_handler::wakeup() { wakeup(NULL, NULL, 0); }

void unix_wakeup_handler::wakeup(wakeup_operation *operation, void *data,
                                 size_t length) {
  int r = 0;
  wakeup_msg msg;
  memset(&msg, 0, sizeof(wakeup_msg));
  msg.data = data;
  msg.length = length;
  msg.operation = operation;
  do {
    r = write(_wakeup_fd[1], &msg, sizeof(msg));
    std::cout << "write wakeup message " << r << std::endl;
  } while (r == -1 && errno == EINTR);
}

void unix_wakeup_handler::intialize(const std::shared_ptr<reactor> &r) {
  wakeup_handler::intialize(r);
  r->add_io_handler(std::dynamic_pointer_cast<io_handler>(shared_from_this()),
                    EV_READ);
}

void unix_wakeup_handler::handle_wakeup() {
  int r;
  wakeup_msg msg;
  memset(&msg, 0, sizeof(wakeup_msg));
  do {
    r = read(_wakeup_fd[0], &msg, sizeof(msg));
  } while (r < 0 && errno == EINTR);
  if (msg.operation != NULL) {
    msg.operation->handle_wakeup(msg.data, msg.length);
  }
}