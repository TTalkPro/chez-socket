//
// Created by david on 3/22/25.
//

#include "CHandlerBase.h"

#include "CEventLoopBase.h"
CHandlerBase::~CHandlerBase() {
  if (Registered) {
    UnregisterOnEventLoop();
  }
  if (EventLoop != nullptr) {
    EventLoop = nullptr;
  }
}

bool CHandlerBase::RegisterOnEventLoop(
    const std::shared_ptr<CEventLoopBase> &EventLoop) {
  if (Registered) {
    return (EventLoop == this->EventLoop); // 如果在相同的EventLoop代表成功
  }
  Registered = EventLoop->AddHandler(shared_from_this());
  if (Registered) {
    this->EventLoop = EventLoop;
  }
  return Registered;
}

void CHandlerBase::UnregisterOnEventLoop() {
  if (!Registered) {
    return;
  }
  if (EventLoop == nullptr) {
    return;
  }
  EventLoop->RemoveHandler(shared_from_this());
  Registered = false;
}