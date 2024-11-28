
#include <sstream>
#include <typeinfo>
#include <cassert>
#include <chrono>
#include <limits>
#include <memory>
#include "timer_handler.h"
#include "timer_manager.h"
//最大时长为1天
const unsigned MAX_TIMER_MILLISECOND = 86400000;


uint64_t current_millisecond() {
    std::chrono::milliseconds current = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());
    return current.count();
}

bool operator<(const timer_item &left, const timer_item &right) {
    //比较两个时间对象大小，左侧减右侧是负数的，那就代表是小
    return ((int64_t)(left.next_expired_time - right.next_expired_time) < 0);
}

timer_manager::timer_manager() {

}

timer_manager::~timer_manager() {

}

bool timer_manager::add_handler(const std::shared_ptr<timer_handler>& handler) {
    //查看是否是已经存在的handler，如果存在同样的handler就直接返回
    if (is_repeat_item(handler)) {
        return false;
    }
    uint64_t  millisecond = handler->millisecond();
    //强限定，时间不能超高一天
    assert(millisecond < MAX_TIMER_MILLISECOND);
    //获取当前时间的时间戳，转化成毫秒

    uint64_t next_expired_time = current_millisecond() + millisecond;
    timer_item item;
    item.handler = handler;
    item.next_expired_time = next_expired_time;
    item.remain_millisecond = UINT64_MAX;
    item.period = millisecond;
    item.is_cycled = handler->is_cycled();
    timer_set::iterator timer = _expired_set.insert(item);
    _handlers.insert(std::pair < std::weak_ptr < timer_handler > , timer_set::iterator > (handler, timer));
    assert(_expired_set.size() == _handlers.size());
    return true;
}

bool timer_manager::is_repeat_item(const std::shared_ptr<timer_handler>& handler) {
    timer_map::iterator it = _handlers.find(handler);
    if (it != _handlers.end()) {
        return true;
    }
    return false;
}

void timer_manager::remove_handler(const std::shared_ptr<timer_handler> &handler) {
    timer_map::iterator it = _handlers.find(handler);
    if (it != _handlers.end()) {
        //从超时集合中删除，对应的对象，然后在从map中删除
        _expired_set.erase(it->second);
        _handlers.erase(it);
    }
    assert(_expired_set.size() == _handlers.size());
}

bool timer_manager::has_timer_item() {
    return (_expired_set.size() != 0);
}

void timer_manager::get_latest_item(timer_item &result) {
    timer_set::iterator begin = _expired_set.begin();
    result = *begin;
    uint64_t current = current_millisecond();
    if (less_than(result.next_expired_time, current)) {
        result.remain_millisecond = 0;
    } else {
        result.remain_millisecond = offset(result.next_expired_time, current);
    }
}

void timer_manager::remove_first() {
    if(_expired_set.size() > 0) {
        timer_set::iterator iter = _expired_set.begin();
        timer_item temp = *iter;
        _expired_set.erase(iter);
        timer_map::iterator it;
        it = _handlers.find(temp.handler);
        if (it != _handlers.end()) {
            _handlers.erase(it);
        }
        // 如果是循环定时，直接再次加回来
        if (temp.is_cycled) {
            add_handler(temp.handler.lock());
        }
        assert(_expired_set.size() == _handlers.size());
    }
}

bool timer_manager::less_than(uint64_t left, uint64_t right) {
    return (((int64_t)(left - right)) < 0);
}

uint64_t timer_manager::offset(uint64_t left, uint64_t right) {
    return left - right;
}
