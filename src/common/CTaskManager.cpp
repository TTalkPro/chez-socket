//
// Created by david on 3/22/25.
//
#include "CTaskManager.h"
#include "CBaseTask.h"

#include "../LispAsync.h"

#include <cassert>
#include <chrono>
#include <limits>
#include <memory>
#include <sstream>

const unsigned MAX_TIMER_MILLISECOND = 86400000;

uint64_t CurrentMillisecond() {
  std::chrono::milliseconds current =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  return current.count();
}

bool operator<(const TimerItem &Left, const TimerItem &Right) {
  // 比较两个时间对象大小，左侧减右侧是负数的，那就代表是小
  return ((int64_t)(Left.NextExpiredTime - Right.NextExpiredTime) < 0);
}


bool CTaskManager::AddTask(const std::shared_ptr<CBaseTask> &Task) {
  if (Task->GetMillisecond() > 0) {
    return AddTimerTask(Task,Task->GetMillisecond(),Task->GetCycled());
  }
  if (IsDuplicatedTask(Task)) {
    return false;
  }
  Tasks.push_back(Task);
  return true;
}

bool CTaskManager::RemoveTask(const std::shared_ptr<CBaseTask> &Task) {
  bool found = RemoveTimerTask(Task);
  if (!found) {
    TaskQueue::iterator it = Tasks.begin();
    while (it != Tasks.end()) {
      if ((*it) == Task) {
        Tasks.erase(it);
        found = true;
        break;
      }
    }
  }
  return found;


}
uint64_t CTaskManager::RunTasks() {
  if (!Tasks.empty()) {
    TaskQueue::iterator it = Tasks.begin();
    while (it != Tasks.end()) {
      (*it)->RunTask(LISP_ASYNC_EV_TASK);
    }
    Tasks.clear();
  }
  TimerItem Item;
  Item.RemainMillisecond = 0;
  while (HasTimerTask()) {
    GetLatestTimerItem(Item);
    if (Item.RemainMillisecond > 0) {
      break;
    }
    RemoveFirstTimerTask();
    const std::shared_ptr<CBaseTask> Task = Item.Task;
    Task->RunTask(LISP_ASYNC_EV_TIMEOUT);
  }
  return Item.RemainMillisecond;

};

bool CTaskManager::HasTimerTask() { return (ExpiredSet.size() != 0); }

void CTaskManager::GetLatestTimerItem(TimerItem &Result) {
  TimerSet::iterator begin = ExpiredSet.begin();
  Result = *begin;
  uint64_t Current = CurrentMillisecond();
  if (LessThan(Result.NextExpiredTime, Current)) {
    Result.RemainMillisecond = 0;
  } else {
    Result.RemainMillisecond = Offset(Result.NextExpiredTime, Current);
  }
}
void CTaskManager::RemoveFirstTimerTask() {
  if (ExpiredSet.size() > 0) {
    TimerSet::iterator iter = ExpiredSet.begin();
    TimerItem Temp = *iter;
    ExpiredSet.erase(iter);

    TimerMap::iterator it;
    it = TimerTasks.find(Temp.Task);
    if (it != TimerTasks.end()) {
      TimerTasks.erase(it);
    }

    // 如果是循环定时，直接再次加回来
    if (Temp.Cycled) {
      const std::shared_ptr<CBaseTask> Task = Temp.Task;
      if (Task != nullptr) {
        AddTimerTask(Task, Temp.Period, Temp.Cycled);
      }
    }
    assert(ExpiredSet.size() == TimerTasks.size());
  }
}




bool CTaskManager::IsDuplicatedTask(const std::shared_ptr<CBaseTask> &Task) {
  TaskQueue::iterator it = Tasks.begin();
  while (it != Tasks.end()) {
    if ((*it) == Task) {
      return true;
    }
  }
  return false;
}

bool CTaskManager::IsDuplicatedTimerTask(
    const std::shared_ptr<CBaseTask> &Task) {
  TimerMap::iterator it = TimerTasks.find(Task);
  if (it != TimerTasks.end()) {
    return true;
  }
  return false;
}


bool CTaskManager::AddTimerTask(const std::shared_ptr<CBaseTask> &Task,
                                uint64_t Millisecond, bool Cycled) {
  // 查看是否是已经存在的handler，如果存在同样的handler就直接返回
  if (IsDuplicatedTimerTask(Task)) {
    return false;
  }
  // 强限定，时间不能超高一天
  assert(Millisecond < MAX_TIMER_MILLISECOND);
  // 获取当前时间的时间戳，转化成毫秒

  uint64_t NextExpiredTime = CurrentMillisecond() + Millisecond;
  TimerItem Item;
  Item.Task = Task;
  Item.NextExpiredTime = NextExpiredTime;
  Item.RemainMillisecond = 0;
  Item.Period = Millisecond;
  Item.Cycled = Cycled;
  TimerSet::iterator Timer = ExpiredSet.insert(Item);
  TimerTasks.insert(std::pair<std::weak_ptr<CBaseTask>, TimerSet::iterator>(
      Task, Timer));
  assert(ExpiredSet.size() == TimerTasks.size());
  return true;
}

bool CTaskManager::RemoveTimerTask(
    const std::shared_ptr<CBaseTask> &Task) {
  bool found = false;
  TimerMap::iterator it = TimerTasks.find(Task);
  if (it != TimerTasks.end()) {
    found = true;
    // 从超时集合中删除，对应的对象，然后在从map中删除
    ExpiredSet.erase(it->second);
    TimerTasks.erase(it);
  }
  assert(ExpiredSet.size() == TimerTasks.size());
  return found;
}
