//
// Created by david on 3/22/25.
//
#include <cassert>
#include <chrono>
#include <limits>
#include <memory>
#include <sstream>
#include "CBaseTask.h"
#include "CTimerManager.h"

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

bool CTimerManager::IsRepeatItem(
    const std::shared_ptr<CBaseTask> &Task) {
  TimerMap::iterator it = Tasks.find(Task);
  if (it != Tasks.end()) {
    return true;
  }
  return false;
}

bool CTimerManager::AddTask(const std::shared_ptr<CBaseTask> &Task,
                                uint64_t Millisecond, bool Cycled) {
  // 查看是否是已经存在的handler，如果存在同样的handler就直接返回
  if (IsRepeatItem(Task)) {
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
  Tasks.insert(std::pair<std::weak_ptr<CBaseTask>, TimerSet::iterator>(
      Task, Timer));
  assert(ExpiredSet.size() == Tasks.size());
  return true;
}

void CTimerManager::RemoveTask(
    const std::shared_ptr<CBaseTask> &Task) {
  TimerMap::iterator it = Tasks.find(Task);
  if (it != Tasks.end()) {
    // 从超时集合中删除，对应的对象，然后在从map中删除
    ExpiredSet.erase(it->second);
    Tasks.erase(it);
  }
  assert(ExpiredSet.size() == Tasks.size());
}
bool CTimerManager::HasTimerItem() { return (ExpiredSet.size() != 0); }

void CTimerManager::GetLatestItem(TimerItem &Result) {
  TimerSet::iterator begin = ExpiredSet.begin();
  Result = *begin;
  uint64_t Current = CurrentMillisecond();
  if (LessThan(Result.NextExpiredTime, Current)) {
    Result.RemainMillisecond = 0;
  } else {
    Result.RemainMillisecond = Offset(Result.NextExpiredTime, Current);
  }
}
void CTimerManager::RemoveFirst() {
  if (ExpiredSet.size() > 0) {
    TimerSet::iterator iter = ExpiredSet.begin();
    TimerItem Temp = *iter;
    ExpiredSet.erase(iter);

    TimerMap::iterator it;
    it = Tasks.find(Temp.Task);
    if (it != Tasks.end()) {
      Tasks.erase(it);
    }

    // 如果是循环定时，直接再次加回来
    if (Temp.Cycled) {
      const std::shared_ptr<CBaseTask> Task = Temp.Task;
      if (Task != nullptr) {
        AddTask(Task, Temp.Period, Temp.Cycled);
      }
    }
    assert(ExpiredSet.size() == Tasks.size());
  }
}