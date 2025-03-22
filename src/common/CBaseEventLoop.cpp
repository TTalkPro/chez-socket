//
// Created by david on 3/22/25.
//
#include "CBaseEventLoop.h"
#include "CBaseReactor.h"
#include "CBaseTask.h"
#include "CTimerTask.h"

#include <exception>
#include <iostream>
#include <memory>
#include <ostream>

CBaseEventLoop::CBaseEventLoop(const std::shared_ptr<CBaseReactor>& Reactor) {
  this->Reactor = Reactor;
}

CBaseEventLoop::~CBaseEventLoop() {
  if (Reactor != nullptr) {
    Reactor = nullptr;
  }
}


bool CBaseEventLoop::AddHandler(
    const std::shared_ptr<CBaseHandler> &Handler) {
  std::cout << "CBaseEventLoop::AddHandler" << std::endl;
  if (nullptr == Reactor) {
    return false;
  }
  return Reactor->AddHandler(Handler);
}

void CBaseEventLoop::RemoveHandler(
    const std::shared_ptr<CBaseHandler> &Handler) {
  std::cout << "CBaseEventLoop::RemoveHandler" << std::endl;
  if (nullptr != Reactor) {
    Reactor->RemoveHandler(Handler);
  }
}
void  CBaseEventLoop::AddTask(const std::shared_ptr<CBaseTask>& Task) {
  const std::shared_ptr<CBaseTask> TimerTask = std::dynamic_pointer_cast<CBaseTask>(Task);
  TimerManager.AddTask(Task,Task->GetMillisecond(),Task->GetCycled());
}
void CBaseEventLoop::RemoveTask(const std::shared_ptr<CBaseTask>& Task) {

}

uint64_t CBaseEventLoop::RunTask() {
  TimerItem Item;
  Item.RemainMillisecond = 0;
  while (TimerManager.HasTimerItem()) {
    TimerManager.GetLatestItem(Item);
    if (Item.RemainMillisecond > 0) {
      break;
    }
    TimerManager.RemoveFirst();
    const std::shared_ptr<CBaseTask> Task = Item.Task;
    Task->RunTask();
  }
  return Item.RemainMillisecond;
}