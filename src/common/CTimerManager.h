//
// Created by david on 3/22/25.
//

#ifndef CTIMERMANAGER_H
#define CTIMERMANAGER_H
#include <map>
#include <memory>
#include <set>

class CTimerTask;

struct TimerItem {
  std::weak_ptr<CTimerTask> Task;
  uint64_t RemainMillisecond;
  uint64_t NextExpiredTime;
  uint64_t Period;
  bool Cycled;
};

class CTimerManager {
public:
  CTimerManager() = default;
  ~CTimerManager() = default;
  bool AddTask(const std::shared_ptr<CTimerTask> &Task, uint64_t Millisecond,
               bool Cycled = false);
  void RemoveTask(const std::shared_ptr<CTimerTask> &Task);
  bool HasTimerItem();
  void GetLatestItem(TimerItem &Result);
  void RemoveFirst();

protected:
  bool LessThan(uint64_t Left, uint64_t Right) {
    return (((int64_t)(Left - Right)) < 0);
  };
  uint64_t Offset(uint64_t Left, uint64_t Right) {
    return (Left - Right);
  };
  bool IsRepeatItem(const std::shared_ptr<CTimerTask> &Task);

private:
  // 使用一个排序的set,把最先超时的handler放在最前面
  typedef std::multiset<TimerItem> TimerSet;
  TimerSet ExpiredSet;
  typedef std::map<std::weak_ptr<CTimerTask>,TimerSet::iterator,
                   std::owner_less<>> TimerMap;
  TimerMap Tasks;
};

#endif // CTIMERMANAGER_H
