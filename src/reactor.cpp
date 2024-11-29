//
// Created by david on 11/28/24.
//
#include <memory>
#include "chez_socket.h"
#include "base_handler.h"
#include "io_handler.h"
#include "timer_handler.h"
#include "select_engine.h"
#include "timer_manager.h"
#include "reactor.h"


reactor::reactor()
{
    _engine = std::make_shared<select_engine>();
    _timer_manager = std::make_shared<timer_manager>();
    _loop.store(true);
}

reactor::~reactor()
{
    _engine = nullptr;
    _timer_manager = nullptr;
}

bool reactor::add_timer_handler(const std::shared_ptr<base_handler>& handler,uint64_t millisecond, bool is_cycled)
{
    bool ret = _timer_manager->add_handler(std::dynamic_pointer_cast<timer_handler>(handler), millisecond,is_cycled);
    if (ret)
    {
        handler->watch(EV_TIMEOUT);
    }
    return ret;
}

bool reactor::add_io_handler(const std::shared_ptr<base_handler>& handler, int events)
{
    int _events = events & EV_IO;
    int ret = _engine->add_handler(std::dynamic_pointer_cast<io_handler>(handler));
    if (ret)
    {
        handler->watch(_events);
    }
    return ret;
}

void reactor::remove_timer_handler(const std::shared_ptr<base_handler>& handler)
{
    _timer_manager->remove_handler(std::dynamic_pointer_cast<timer_handler>(handler));
    handler->unwatch(EV_TIMEOUT);
}

void reactor::remove_io_handler(const std::shared_ptr<base_handler>& handler, int events)
{
    int _events = events & EV_IO;
    handler->unwatch(_events);
    if (handler->pending_events() == 0)
    {
        _engine->remove_handler(std::dynamic_pointer_cast<io_handler>(handler));
    }
}

uint64_t reactor::run_timer_task()
{
    timer_item _item;
    _item.remain_millisecond = 0;
    while (_timer_manager->has_timer_item())
    {
        _timer_manager->get_latest_item(_item);
        if (_item.remain_millisecond > 0)
        {
            break;
        }
        _timer_manager->remove_first();
        const std::shared_ptr<timer_handler> _timer_handler = _item.handler.lock();
        if (_timer_handler == nullptr)
        {
            continue;
        }
        _timer_handler->handle_timer();
    }
    return _item.remain_millisecond;
}

void reactor::run()
{
    uint64_t _waiting = 0;
    while (_loop.load())
    {
        _waiting = run_timer_task();
        if(_loop.load())
        {
            _engine->poll(_waiting);
        }
    }
}
