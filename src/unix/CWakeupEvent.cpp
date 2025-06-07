//
// Created by david on 6/7/25.
//
#include "./CWakeupEvent.h"
#include "./UnixPlatform.h"
#include "../common/CEventLoop.h"
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>

const char WAKEUP_MESSAGE = 'w';
CWakeupEvent::CWakeupEvent() {
  if (0 != unix_make_pipe(WakeupHandles, O_NONBLOCK, O_NONBLOCK)) {
    throw std::runtime_error("Failed to create pipe for CWakeupEvent");
  }
  SetHandle(WakeupHandles[0]);
}
CWakeupEvent::~CWakeupEvent() {
  if (WakeupHandles[0] > 0) {
    close(WakeupHandles[0]);
  }
  if (WakeupHandles[1] > 0) {
    close(WakeupHandles[1]);
  }
}

void CWakeupEvent::HandleEvent(int Events) {
  int Ret = 0;
  char w[1024];
  do {
    Ret = read(WakeupHandles[0], &w, 1024);
  } while (Ret < 0 && errno == EINTR);
}

void CWakeupEvent::Wakeup() {
  std::shared_ptr<CEventLoop> EventLoop = GetEventLoop().lock();
  if (nullptr == EventLoop) {
    return;
  }
  if (EventLoop->InEventLoop()) {
    return;
  }
  int Ret = 0;
  do {
    Ret = write(WakeupHandles[1], &WAKEUP_MESSAGE, 1);
  } while (Ret == -1 && errno == EINTR);
}
