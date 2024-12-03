#ifndef CHEZ_SOCKET_TIMER_MANAGER_H
#define CHEZ_SOCKET_TIMER_MANAGER_H

#include <map>
#include <memory>
#include <set>

class timer_handler;
class reactor;

struct timer_item {
  std::weak_ptr<timer_handler> handler;
  uint64_t remain_millisecond;
  uint64_t next_expired_time;
  uint64_t period;
  bool is_cycled;
};

class timer_manager {
  friend class reactor;
public:
  timer_manager() = default;
  ~timer_manager() = default;
  bool add_handler(const std::shared_ptr<timer_handler> &handler,
                   uint64_t millisecond, bool is_cycled = false);
  void remove_handler(const std::shared_ptr<timer_handler> &handler);
  bool has_timer_item();
  void get_latest_item(timer_item &result);
  void remove_first();

protected:
  virtual void attach_to_reactor(const std::shared_ptr<reactor> &r) {
    _intance = r;
  };
  bool less_than(uint64_t left, uint64_t right);
  uint64_t offset(uint64_t left, uint64_t right);
  bool is_repeat_item(const std::shared_ptr<timer_handler> &handler);

private:
  // 使用一个排序的set,把最先超时的handler放在最前面
  typedef std::multiset<timer_item> timer_set;
  timer_set _expired_set;
  typedef std::map<std::weak_ptr<timer_handler>, timer_set::iterator,
                   std::owner_less<>> timer_map;
  timer_map _handlers;
  std::weak_ptr<reactor> _intance;
};

#endif  //CHEZ_SOCKET_TIMER_MANAGER_H
