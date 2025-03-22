//
// Created by david on 3/22/25.
//
#include "CEventLoopBase.h"
#include "CHandlerBase.h"
#include <exception>
#include <memory>

CEventLoopBase::~CEventLoopBase() {
  if (Thread != nullptr) {
    if (Thread->joinable()) {
      Thread->detach();
    }
    delete Thread;
    Thread = nullptr;
  }
}

void CEventLoopBase::MaybeResize(int Size) {
  if (Handlers.capacity() < Size + 1) {
    Handlers.resize(Size + 1);
  }
}

bool CEventLoopBase::AddHandler(
    const std::shared_ptr<CHandlerBase> &Handler) {
  bool ret = false;
  int RawHandler = Handler->GetRawHandler();
  if (RawHandler != -1) {
    if (CanAddHandler(Handler)) {
      MaybeResize(RawHandler);
      PendingHandlers.insert(Handler);
      ret = true;
    }
  }
  return ret;
}

void CEventLoopBase::RemoveHandler(
    const std::shared_ptr<CHandlerBase> &Handler) {
  int RawHandler = Handler->GetRawHandler();
  if (RawHandler != -1) {
    if (RawHandler < Handlers.size()) {
      // 说明已经在Handlers中了
      Handlers[RawHandler].reset();
    } else {
      // 在pendings的状态下就被移除了
      // 正常情况下是不应该发生的
      std::set<std::shared_ptr<CHandlerBase>,
               std::owner_less<>>::iterator it = PendingHandlers.find(Handler);
      if (it != PendingHandlers.end()) {
        PendingHandlers.erase(it);
      }
    }
  }
}

bool CEventLoopBase::Start() {
  if (Running.load()) {
    return false;
  }
  try {
    Thread = std::make_unique<std::thread>(&CEventLoopBase::ThreadEntry, this);
  } catch (...) {
    throw "[CEventBase::Start] thread start error";
  }
  return true;
}
void CEventLoopBase::ThreadEntry() {
  if (!Running.compare_exchange_strong(&false,&true,
    std::memory_order_release,
    std::memory_order_relaxed)) {
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
bool CEventLoopBase::InEventLoop() {
  return (Thread->get_id() == std::this_thread::get_id());
}