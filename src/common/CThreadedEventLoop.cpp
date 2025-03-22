//
// Created by david on 3/22/25.
//

#include "CThreadedEventLoop.h"

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
  while (Running.load()) {
  }
}

bool CThreadedEventLoop::InEventLoop() override {
  if (Thread == nullptr) {
    return false;
  }
  return (Thread->get_id() == std::this_thread::get_id());
}

void CThreadedEventLoop::Poll() {
  Start();
  Join();
}

bool CThreadedEventLoop::AddHandler(
    const std::shared_ptr<CBaseHandler> &Handler) override {
  if (InEventLoop()) {
    return CBaseEventLoop::AddHandler(Handler);
  }else {
    std::lock_guard<std::mutex> guard(HandlersMutex);
    return CBaseEventLoop::AddHandler(Handler);
  }

};
void CThreadedEventLoop::RemoveHandler(
    const std::shared_ptr<CBaseHandler> &Handler) override {
  if (InEventLoop()) {
    CBaseEventLoop::RemoveHandler(Handler);
  }else {
    std::lock_guard<std::mutex> guard(HandlersMutex);
    CBaseEventLoop::RemoveHandler(Handler);
  }
};
