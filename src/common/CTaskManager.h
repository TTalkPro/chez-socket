//
// Created by david on 3/22/25.
//

#ifndef CTIMERMANAGER_H
#define CTIMERMANAGER_H
#include <map>
#include <memory>
#include <vector>
#include <set>

class CBaseTask;

struct TimerItem {
  std::shared_ptr<CBaseTask> Task;
  uint64_t RemainMillisecond;
  uint64_t NextExpiredTime;
  uint64_t Period;
  bool Cycled;
};



class CTaskManager final {

public:
  CTaskManager() = default;
  ~CTaskManager() = default;
  bool AddTask(const std::shared_ptr<CBaseTask>& Task);
  bool RemoveTask(const std::shared_ptr<CBaseTask> &Task);
  uint64_t RunTasks();

protected:

  bool HasTimerTask();
  void GetLatestTimerItem(TimerItem &Result);
  void RemoveFirstTimerTask();
  bool AddTimerTask(const std::shared_ptr<CBaseTask> &Task, uint64_t Millisecond,
             bool Cycled = false);
  bool RemoveTimerTask(const std::shared_ptr<CBaseTask> &Task);
  bool LessThan(uint64_t Left, uint64_t Right) {
    return (((int64_t)(Left - Right)) < 0);
  };
  uint64_t Offset(uint64_t Left, uint64_t Right) {
    return (Left - Right);
  };
  bool IsDuplicatedTask(const std::shared_ptr<CBaseTask> &Task);
  bool IsDuplicatedTimerTask(const std::shared_ptr<CBaseTask> &Task);

private:
  /*struct TimeItemCompare {
    bool operator()(const TimerItem& a, const TimerItem& b) const {
      return a.NextExpiredTime < b.NextExpiredTime; // 先超时的在前面
    }
  };*/
  // 使用一个排序的set,把最先超时的handler放在最前面
  typedef std::multiset<TimerItem> TimerSet;
  TimerSet ExpiredSet;
  typedef std::map<std::weak_ptr<CBaseTask>,TimerSet::iterator,
                   std::owner_less<>> TimerMap;
  TimerMap TimerTasks;
  typedef std::vector<std::shared_ptr<CBaseTask>> TaskQueue;
  TaskQueue Tasks;
};

#endif // CTIMERMANAGER_H
