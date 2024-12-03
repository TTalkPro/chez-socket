//
// Created by david on 11/28/24.
//

#ifndef CZSOCKET_REACTOR_H
#define CZSOCKET_REACTOR_H

#include <memory>
#include <atomic>

class timer_manager;
class io_manager;
class io_handler;
class wakeup_handler;

class reactor : public  std::enable_shared_from_this<reactor>
{
public:
    reactor();
    ~reactor();
    void attach(const std::shared_ptr<io_handler>& handler)
    {
        handler->attach_reactor(shared_from_this());
    };
    bool add_timer_handler(const std::shared_ptr<io_handler>& handler,uint64_t millisecond, bool is_cycled = false);
    bool add_io_handler(const std::shared_ptr<io_handler>& handler, int events);
    void remove_timer_handler(const std::shared_ptr<io_handler>& handler);
    void remove_io_handler(const std::shared_ptr<io_handler>& handler, int events);
    void run();
    void stop() { _loop.exchange(false); };

private:
    uint64_t run_timer_task();
    std::shared_ptr<io_manager> _engine;
    std::shared_ptr<timer_manager> _timer_manager;
    std::atomic<bool> _loop;
};


#endif //CZSOCKET_REACTOR_H
