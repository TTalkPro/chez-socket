//
// Created by david on 3/22/25.
//

#ifndef CBASEEVENTLOOP_H
#define CBASEEVENTLOOP_H
#include <queue>
#include <memory>

#include "CTimerManager.h"

//整体上EventLoop要处理的只有2件事情
//一件事情是IO事件，另外一个就是异步任务
//异步任务分会时间任务和一般任务
class CBaseHandler;
class CBaseTask;
// IO任务交给Reactor进行处理
// 时间任务交给EventLoop本身处理
class CBaseReactor;

enum EVENTLOOP_TYPE {
  EVENTLOOP_TYPE_NONE = 0,
  EVENTLOOP_TYPE_THREADED,
  EVENTLOOP_TYPE_CURRENT_THREAD
};

class CBaseEventLoop : public std::enable_shared_from_this<CBaseEventLoop> {
public:
  CBaseEventLoop(const std::shared_ptr<CBaseReactor>& reactor);
  virtual ~CBaseEventLoop();

  virtual bool AddHandler(const std::shared_ptr<CBaseHandler>& Handler);
  virtual void RemoveHandler(const std::shared_ptr<CBaseHandler>& Handler);
  virtual void AddTask(const std::shared_ptr<CBaseTask>& Task);
  virtual void RemoveTask(const std::shared_ptr<CBaseTask>& Task);
  virtual bool InEventLoop() = 0;
  virtual void Poll()  = 0;

  EVENTLOOP_TYPE GetType() {return Type;};
protected:
  uint64_t RunTask();
  std::shared_ptr<CBaseReactor> Reactor = nullptr;
  //目前我们只有一种TimerManager
  CTimerManager TimerManager;
  EVENTLOOP_TYPE Type = EVENTLOOP_TYPE_NONE;
};

#endif // CBASEEVENTLOOP_H
