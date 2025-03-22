//
// Created by david on 3/22/25.
//

#include "CThreadedEventLoop.h"

#include "CBaseReactor.h"
#include "CBaseTask.h"
#include "CTimerTask.h"

#include <future>
#include <iostream>
#include <ostream>

class CHandlerTask : virtual public CBaseTask {
public:
  CHandlerTask(const std::shared_ptr<std::promise<bool>> &Promise,
               const std::shared_ptr<CBaseHandler> &Handler,
               const std::shared_ptr<CBaseEventLoop> &EventLoop)
      : CBaseTask() {
    this->Promise = Promise;
    this->Handler = Handler;
    this->EventLoop = EventLoop;
  }
  ~CHandlerTask() override {
    std::cout << "CHandlerTask::~CHandlerTask()" << std::endl;
    if (Promise != nullptr) {
      Promise = nullptr;
    }
    if (Handler != nullptr) {
      Handler = nullptr;
    }
    if (EventLoop != nullptr) {
      EventLoop = nullptr;
    }
  }

protected:
  std::shared_ptr<std::promise<bool>> Promise;
  std::shared_ptr<CBaseHandler> Handler;
  std::shared_ptr<CBaseEventLoop> EventLoop;
};

class CAddHandlerTask final : virtual public CHandlerTask {
public:
  CAddHandlerTask(const std::shared_ptr<std::promise<bool>> &Promise,
                  const std::shared_ptr<CBaseHandler> &Handler,
                  const std::shared_ptr<CBaseEventLoop> &EventLoop)
      : CHandlerTask(Promise, Handler, EventLoop) {};
  ~CAddHandlerTask() override = default;
  void RunTask() override {
    bool ret = EventLoop->AddHandler(Handler);
    Promise->set_value(ret);
  };
};

class CRemoveHandlerTask final : virtual public CHandlerTask {
public:
  CRemoveHandlerTask(const std::shared_ptr<std::promise<bool>> &Promise,
                     const std::shared_ptr<CBaseHandler> &Handler,
                     const std::shared_ptr<CBaseEventLoop> &EventLoop)
      : CHandlerTask(Promise, Handler, EventLoop) {};
  ~CRemoveHandlerTask() override = default;
  void RunTask() override {
    EventLoop->RemoveHandler(Handler);
    Promise->set_value(true);
  };
};

CThreadedEventLoop::~CThreadedEventLoop() {
  if (Thread != nullptr) {
    if (Thread->joinable()) {
      Thread->detach();
    }
    Thread = nullptr;
  }
}

bool CThreadedEventLoop::Start() {
  if (Running.load()) {
    return false;
  }
  try {
    Thread =
        std::make_unique<std::thread>(&CThreadedEventLoop::ThreadEntry, this);
  } catch (...) {
    throw "[CEventBase::Start] thread start error";
  }
  return true;
}

void CThreadedEventLoop::Stop() {
  bool oldVal = Running;
  if (oldVal == false)
    return;
  while (!Running.compare_exchange_strong(oldVal, false)) {
    if (Running.load() == false) {
      break;
    }
  }
  if (Thread != nullptr) {
    if (Thread->joinable()) {
      Thread->detach();
    }
    Thread = nullptr;
  }
}

void CThreadedEventLoop::ThreadEntry() {
  bool oldValue = Running;
  if (!Running.compare_exchange_strong(oldValue, true)) {
    return;
  }
  try {
    Run();
  } catch (std::exception &ex) {
    Running = false;
    throw ex;
  } catch (...) {
    Running = false;
    throw;
  }
  Running = false;
}

void CThreadedEventLoop::Run() {
  uint64_t Deadline = 0;

  while (Running.load()) {
    {
      std::lock_guard<std::mutex> lock(TaskMutex);
      Deadline = RunTask();
    }
    if (nullptr != Reactor) {
      Reactor->Poll(Deadline);
    }
  }
}

bool CThreadedEventLoop::InEventLoop()  {
  if (nullptr == Thread) {
    return false;
  }
  return (Thread->get_id() == std::this_thread::get_id());
}

void CThreadedEventLoop::Poll() {
  Start();
  Join();
}

bool CThreadedEventLoop::AddHandler(
    const std::shared_ptr<CBaseHandler> &Handler)  {
  if (InEventLoop()) {
    std::cout << "CThreadedEventLoop::AddHandler InEventLoop" << std::endl;
    return CBaseEventLoop::AddHandler(Handler);
  } else {
    std::cout << "CThreadedEventLoop::AddHandler Task" << std::endl;
    std::shared_ptr<std::promise<bool>> ActionResult =
        std::make_shared<std::promise<bool>>();
    std::future<bool> Ret = ActionResult->get_future();
    std::shared_ptr<CAddHandlerTask> Task = std::make_shared<CAddHandlerTask>(
        ActionResult, Handler, shared_from_this());
    AddTask(Task);
    return Ret.get();
  }
};
void CThreadedEventLoop::RemoveHandler(
    const std::shared_ptr<CBaseHandler> &Handler)  {
  if (InEventLoop()) {
    std::cout << "CThreadedEventLoop::RemoveHandler InEventLoop" << std::endl;
    CBaseEventLoop::RemoveHandler(Handler);
  } else {
    std::cout << "CThreadedEventLoop::RemoveHandler Task" << std::endl;
    std::shared_ptr<std::promise<bool>> ActionResult =
    std::make_shared<std::promise<bool>>();
    std::future<bool> Ret = ActionResult->get_future();
    std::shared_ptr<CRemoveHandlerTask> Task = std::make_shared<CRemoveHandlerTask>(
        ActionResult, Handler, shared_from_this());
    AddTask(Task);
    Ret.get();
  }
};

void CThreadedEventLoop::AddTask(
    const std::shared_ptr<CBaseTask> &Task)  {
  std::lock_guard<std::mutex> guard(TaskMutex);
  CBaseEventLoop::AddTask(Task);
}
void CThreadedEventLoop::RemoveTask(
    const std::shared_ptr<CBaseTask> &Task)  {
  std::lock_guard<std::mutex> guard(TaskMutex);
  CBaseEventLoop::RemoveTask(Task);
}