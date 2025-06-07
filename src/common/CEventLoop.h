//
// Created by david on 3/22/25.
//

#ifndef CEVENTLOOP_H
#define CEVENTLOOP_H
#include <memory>
#include <thread>
#include <mutex>

#include "../LispAsync.h"
#include "CTaskManager.h"
#include "CBaseReactor.h"
#include "CBaseTask.h"
#include "CBaseEvent.h"
// 整体上EventLoop要处理的只有2件事情
//一件事情是IO事件，另外一个就是异步任务
//异步任务分会时间任务和一般任务
// IO任务交给Reactor进行处理
// 时间任务交给EventLoop本身处理
class CWakeupEvent;


class CEventLoop final : public std::enable_shared_from_this<CEventLoop> {
public:
  explicit CEventLoop(const std::shared_ptr<CBaseReactor>& Reactor,
    const std::shared_ptr<CWakeupEvent>& WakeupEvent = nullptr);
  ~CEventLoop();
  bool AddEvent(const std::shared_ptr<CBaseEvent> &Event,int Events = LISP_ASYNC_EVENT_NONE);
  bool RemoveEvent(const std::shared_ptr<CBaseEvent> &Event,int Events = LISP_ASYNC_EVENT_NONE);
  bool AddTask(const std::shared_ptr<CBaseTask>& Task);
  bool RemoveTask(const std::shared_ptr<CBaseTask>& Task);
  bool InEventLoop();
  void Prepare();
  void Poll();
  void Wakeup ();
protected:
  uint64_t RunTasks();
private:
  std::thread::id ThreadId = std::this_thread::get_id();
  std::mutex TaskMutex;
  //我们可以人为一般任务是时间戳为0的Task
  CTaskManager TaskManager;

  std::shared_ptr<CBaseReactor> Reactor = nullptr;
  std::shared_ptr<CWakeupEvent> WakeupEvent = nullptr;

};

#endif // CEVENTLOOP_H
