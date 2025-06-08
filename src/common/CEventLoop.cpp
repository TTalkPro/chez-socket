//
// Created by david on 3/22/25.
//
#include <future>
#include <iostream>
#include <memory>
#include <ostream>
#if defined(__linux__) || \
    defined(__FreeBSD__) || defined(__OpenBSD__) ||      \
    defined(__DragonFly__) || defined(__NetBSD__)
#include "../unix/CWakeupEvent.h"
#endif

#include "CEventLoop.h"


class CEventTask : virtual public CBaseTask {
public:
  CEventTask(const std::shared_ptr<std::promise<bool>> &Promise,
               const std::shared_ptr<CBaseEvent> &Event,
               const std::shared_ptr<CEventLoop> &EventLoop,
               int Events)
      : CBaseTask() {
    this->Promise = Promise;
    this->Event = Event;
    this->EventLoop = EventLoop;
    this->Events = Events;
  }
  ~CEventTask() override {
    if (Promise != nullptr) {
      Promise = nullptr;
    }
    if (Event != nullptr) {
      Event = nullptr;
    }
    if (EventLoop != nullptr) {
      EventLoop = nullptr;
    }
  }

protected:
  std::shared_ptr<std::promise<bool>> Promise;
  std::shared_ptr<CBaseEvent> Event;
  std::shared_ptr<CEventLoop> EventLoop;
  int Events = LISP_ASYNC_EVENT_NONE;
};

class CAddEventTask final : virtual public CEventTask {
public:
  CAddEventTask(const std::shared_ptr<std::promise<bool>> &Promise,
                  const std::shared_ptr<CBaseEvent> &Event,
                  const std::shared_ptr<CEventLoop> &EventLoop,
                  int Events = LISP_ASYNC_EVENT_NONE)
      : CEventTask(Promise, Event, EventLoop,Events) {};
  ~CAddEventTask() override = default;
  void RunTask(int TaskEvents) override {
    bool ret = EventLoop->AddEvent(Event,Events);
    Promise->set_value(ret);
  };
};
class CRemoveEventTask final : virtual public CEventTask {
public:
  CRemoveEventTask(const std::shared_ptr<std::promise<bool>> &Promise,
                  const std::shared_ptr<CBaseEvent> &Event,
                  const std::shared_ptr<CEventLoop> &EventLoop,
                  int Events = LISP_ASYNC_EVENT_NONE)
      : CEventTask(Promise, Event, EventLoop,Events) {};
  ~CRemoveEventTask() override = default;
  void RunTask(int TaskEvents) override {
    bool ret = false;
    if (Events > 0) {
      ret = EventLoop->RemoveEvent(Event,Events);
    }else {
      ret = EventLoop->RemoveEvent(Event);
    }
    Promise->set_value(ret);
  };
};


CEventLoop::CEventLoop(const std::shared_ptr<CBaseReactor>& Reactor,
  const std::shared_ptr<CWakeupEvent>& WakeupEvent) {
  this->Reactor = Reactor;
  this->ThreadId = std::this_thread::get_id();
  this->WakeupEvent = WakeupEvent;

}

CEventLoop::~CEventLoop() {
  std::cout << "CEventLoop::~CEventLoop()" << std::endl;
  if (Reactor != nullptr) {
    Reactor = nullptr;
  }
}

void CEventLoop::Prepare() {
  if (Reactor != nullptr) {
    Reactor->Attach(shared_from_this());
  }
  if (WakeupEvent != nullptr) {
    AddEvent(WakeupEvent);
  }
}

bool CEventLoop::AddEvent(
    const std::shared_ptr<CBaseEvent> &Event,
    int Events)  {
  if (InEventLoop()) {
    if (nullptr != Reactor) {
     return Reactor->AddEvent(Event,Events);
    }
    return false;
  }
  std::shared_ptr<std::promise<bool>> ActionResult =
        std::make_shared<std::promise<bool>>();
  std::future<bool> Ret = ActionResult->get_future();
  std::shared_ptr<CAddEventTask> Task = std::make_shared<CAddEventTask>(
      ActionResult, Event, shared_from_this(),Events);
  AddTask(Task);
  return Ret.get();
}
bool CEventLoop::RemoveEvent(
    const std::shared_ptr<CBaseEvent> &Event,
    int Events)  {
  if (InEventLoop()) {
    if (nullptr != Reactor) {
     return Reactor->RemoveEvent(Event,Events);
    }
    return false;
  }
  std::shared_ptr<std::promise<bool>> ActionResult =
    std::make_shared<std::promise<bool>>();
  std::future<bool> Ret = ActionResult->get_future();
  std::shared_ptr<CRemoveEventTask> Task = std::make_shared<CRemoveEventTask>(
      ActionResult, Event, shared_from_this(),Events);
  AddTask(Task);
  return Ret.get();
}

bool  CEventLoop::AddTask(const std::shared_ptr<CBaseTask>& Task) {
  if (!this->InEventLoop()) {
    std::lock_guard<std::mutex> guard(TaskMutex);
    return TaskManager.AddTask(Task);
  }
   return  TaskManager.AddTask(Task);

}
bool CEventLoop::RemoveTask(const std::shared_ptr<CBaseTask>& Task) {
  if (!this->InEventLoop()) {
    std::lock_guard<std::mutex> guard(TaskMutex);
   return  TaskManager.RemoveTask(Task);
  }
  return  TaskManager.RemoveTask(Task);

}

bool CEventLoop::InEventLoop()  {
  return (ThreadId == std::this_thread::get_id());
}

void CEventLoop::Poll() {
  uint64_t Deadline = RunTasks();
  if (nullptr != Reactor) {
    Reactor->Poll(Deadline);
  }
}

void CEventLoop::Wakeup() {
  if (nullptr != WakeupEvent) {
    WakeupEvent->Wakeup();
  }
}

void CEventLoop::Register(std::shared_ptr<CBaseEvent> Event) {
  EventsSet::iterator it = Events.find(Event);
  if (it != Events.end()) {
    return;
  }
  Events.insert(Event);
}

void CEventLoop::Unregister(std::shared_ptr<CBaseEvent> Event) {
  EventsSet::iterator it = Events.find(Event);
  if (it != Events.end()) {
    Events.erase(it);
  }
}


uint64_t CEventLoop::RunTasks() {
  std::lock_guard<std::mutex> lock(TaskMutex);
  return TaskManager.RunTasks();

}


