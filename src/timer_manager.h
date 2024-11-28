#ifndef CHEZ_SOCKET_TIMER_MANAGER_H
#define CHEZ_SOCKET_TIMER_MANAGER_H

#include <map>
#include <set>
#include <memory>

class timer_handler;

struct timer_item
{
    std::weak_ptr<timer_handler> handler;
    uint64_t remain_millisecond;
    uint64_t next_expired_time;
};

class timer_manager  
{
public:
	timer_manager();
	virtual ~timer_manager();
	bool add_handler(const std::shared_ptr<timer_handler>& handler);
	void remove_handler(const std::shared_ptr<timer_handler>& handler);
	bool has_timer_item();
	void get_latest_item( timer_item& result );
	void remove_first();
protected:
	bool less_than( uint64_t left, uint64_t right );
    uint64_t offset( uint64_t left, uint64_t right );
	bool is_repeat_item(const std::shared_ptr<timer_handler>& handler);
private:
    //使用一个排序的set,把最先超时的handler放在最前面
	typedef std::multiset<timer_item> timer_set;
	timer_set _expired_set;
	typedef std::map<std::weak_ptr<timer_handler>, timer_set::iterator,std::owner_less<>> timer_map;
    timer_map _handlers;
};

#endif  //CHEZ_SOCKET_TIMER_MANAGER_H
