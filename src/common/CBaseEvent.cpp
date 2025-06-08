//
// Created by david on 3/22/25.
//

#include "CBaseEvent.h"

#include "CEventLoop.h"

#include <iostream>
#include <ostream>
CBaseEvent::~CBaseEvent() {
  std::cout << "CBaseEvent::~CBaseEvent()" << std::endl;
  Detach();
}

void CBaseEvent::Attach(
    const std::shared_ptr<CEventLoop> &EventLoop) {
  const std::shared_ptr<CEventLoop> CurrentEventLoop = this->EventLoop.lock();
  if (CurrentEventLoop != nullptr) {
    if (EventLoop == CurrentEventLoop) {
      return;
    }
    CurrentEventLoop->RemoveEvent(shared_from_this());
    CurrentEventLoop->Unregister(shared_from_this());
  }
  this->EventLoop = EventLoop;
  EventLoop->Register(shared_from_this());
}

void CBaseEvent::Detach() {
  const std::shared_ptr<CEventLoop> CurrentEventLoop = this->EventLoop.lock();
  if (CurrentEventLoop != nullptr) {
    CurrentEventLoop->RemoveEvent(shared_from_this());
    CurrentEventLoop->Unregister(shared_from_this());
    EventLoop.reset();
  }
}
