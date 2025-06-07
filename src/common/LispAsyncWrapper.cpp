//
// Created by david on 6/7/25.
//
#include <memory>
#include <iostream>
#include <ostream>
#include "CEventLoop.h"
#include "LispAsyncWrapper.h"
#if defined(__linux__) || \
    defined(__FreeBSD__) || defined(__OpenBSD__) ||      \
    defined(__DragonFly__) || defined(__NetBSD__)

  #include "../unix/CWakeupEvent.h"
  #include "../unix/CSelectReactor.h"
#endif

#ifndef CWAKEUP_EVENT_H
class CWakeupEvent final: public CBaseEvent {
public:
  void HandleEvent(int Events) override{};
  void Wakeup() {};
};
#endif //CWAKEUP_EVENT_H


thread_local std::shared_ptr<CEventLoop> _loop = nullptr;

void* lisp_async_create_loop() {
  std::shared_ptr<CWakeupEvent> WakeupEvent = std::make_shared<CWakeupEvent>();
  std::shared_ptr<CSelectReactor> Reactor = std::make_shared<CSelectReactor>();
  _loop = std::make_shared<CEventLoop>(Reactor,WakeupEvent);
  _loop->Prepare();
  std::cout<<"lisp_async_create_loop: " << &_loop <<std::endl;
  return &_loop;
}

void lisp_async_destroy_loop(void* loop) {
  std::shared_ptr<CEventLoop> *l = (std::shared_ptr<CEventLoop> *)loop;
  std::cout<<"lisp_async_destroy_loop: " << l <<std::endl;
  l->reset();
}