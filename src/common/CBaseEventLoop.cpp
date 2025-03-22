//
// Created by david on 3/22/25.
//
#include "CBaseEventLoop.h"
#include "CBaseReactor.h"
#include "CBaseTask.h"
#include "CHandlerBase.h"
#include "CTimerTask.h"

#include <exception>
#include <memory>

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
  if (nullptr == Reactor) {
    return false;
  }
  return Reactor->AddHandler(Handler);
}

void CBaseEventLoop::RemoveHandler(
    const std::shared_ptr<CBaseHandler> &Handler) {
  if (nullptr != Reactor) {
    Reactor->RemoveHandler(Handler);
  }
}
void  CBaseEventLoop::AddTask(const std::shared_ptr<CBaseTask>& Task) {
  switch (Task->GetTaskType()) {
    case TASK_TYPE_TIMER:
      const std::shared_ptr<CTimerTask> TimerTask = std::dynamic_pointer_cast<CTimerTask>(Task);
      TimerManager.AddTask(TimerTask,TimerTask->GetMillisecond(),TimerTask->GetCycled());
    break;
    default:
      Tasks.push(Task);
  }
}
void CBaseEventLoop::RemoveTask(const std::shared_ptr<CBaseTask>& Task) {

}