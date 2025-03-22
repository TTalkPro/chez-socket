//
// Created by david on 3/22/25.
//

#include "CBaseHandler.h"

#include "CBaseEventLoop.h"

#include <iostream>
#include <ostream>
CBaseHandler::~CBaseHandler() {
  std::cout << "CBaseHandler::~CBaseHandler()" << std::endl;
  if (Registered) {
    UnregisterOnEventLoop();
  }
  if (EventLoop != nullptr) {
    EventLoop = nullptr;
  }
}

bool CBaseHandler::RegisterOnEventLoop(
    const std::shared_ptr<CBaseEventLoop> &EventLoop) {
  if (Registered) {
    return (EventLoop == this->EventLoop); // 如果在相同的EventLoop代表成功
  }
  Registered = EventLoop->AddHandler(shared_from_this());
  std::cout << "CBaseHandler::RegisterOnEventLoop(): " << Registered << std::endl;
  if (Registered) {
    this->EventLoop = EventLoop;
  }
  return Registered;
}

void CBaseHandler::UnregisterOnEventLoop() {
  if (!Registered) {
    return;
  }
  if (EventLoop == nullptr) {
    return;
  }
  EventLoop->RemoveHandler(shared_from_this());
  Registered = false;
}