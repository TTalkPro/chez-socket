//
// Created by david on 11/28/24.
//

#ifndef CZSOCKET_REACTOR_H
#define CZSOCKET_REACTOR_H

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>

class timer_manager;
class io_manager;
class base_handler;
class wakeup_handler;
class wakeup_operation;

class reactor : public std::enable_shared_from_this<reactor>
{
public:
    static std::shared_ptr<reactor>& instance();
private:
    thread_local static std::mutex _instance_guard;
    thread_local static std::shared_ptr<reactor> _instance;

public:
    ~reactor();
    void attach_handler(const std::shared_ptr<base_handler>& handler);
    bool add_timer_handler(const std::shared_ptr<base_handler>& handler,
                           uint64_t millisecond, bool is_cycled = false);
    bool add_io_handler(const std::shared_ptr<base_handler>& handler, int events);
    void remove_timer_handler(const std::shared_ptr<base_handler>& handler);
    void remove_io_handler(const std::shared_ptr<base_handler>& handler,
                           int events);
    void wakeup();
    void wakeup(wakeup_operation* operation,void* data, size_t size);
    void run();
    void stop() { _loop.exchange(false); };
    bool in_thread();

protected:
    reactor(const reactor&) = delete;
    reactor& operator=(const reactor) = delete;
    reactor();
    void initialize();
private:
    uint64_t run_timer_task();
    std::thread::id _thread_id;
    std::unique_ptr<io_manager> _io_manager;
    std::unique_ptr<timer_manager> _timer_manager;
    std::shared_ptr<wakeup_handler> _wakeup_handler;
    std::atomic<bool> _loop;
};

#endif // CZSOCKET_REACTOR_H
