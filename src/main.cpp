#include <sys/socket.h>
#include<unistd.h>
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>
#include "chez_socket.h"
#include "common/timer_handler.h"
#include "common/io_handler.h"
#include "common/wakeup_handler.h"
#include "reactor.h"

std::string str = std::string("Hello World!");
class my_handler: public timer_handler, public io_handler
{
public:
    my_handler(uint64_t millisecond):timer_handler(), io_handler(0)
    {
        _millisecond = millisecond;
    }

    ~my_handler()
    {
        std::cout << "the my_handler destruct" << std::endl;
    }
    void handle_timer() override
    {
        if (attached_reactor() != nullptr)
        {
            std::cout << "stop the reactor after: " << _millisecond << "ms" << std::endl;
            attached_reactor()->stop();
        }
    }
    void handle_io(int events) override
    {
        std::cout << "the my_handler handle events: " << events << std::endl;
        if (events & EV_READ)
        {
            int n = read(fd(), _buffer, 1023);
            _buffer[n] = 0;
            std::string _str = std::string(_buffer);
            std::cout << "get " << n << "bytes: " << _str << std::endl;
            attached_reactor()->remove_io_handler(shared_from_this(),EV_READ);

        }
    }
    uint64_t millisecond() const  { return _millisecond; };

private:
    char _buffer[1024];
    uint64_t _millisecond;
};

class my_wakeup_handler: public wakeup_handler
{
public:
    my_wakeup_handler() = default;
    ~my_wakeup_handler() = default;
    void handle_wakeup(const wakeup_msg &msg) override
    {
        std::cout << "the my_wakeup_handler handle_wakeup: "  << std::endl;
        std::string str = std::string((const char*)msg.data,msg.length);
        std::cout << str << std::endl;
    }
};
void async_call(std::shared_ptr<reactor>& r, std::shared_ptr<wakeup_handler>& handler)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    r->wakeup(handler.get(),(void*)str.c_str(),str.length());
}


int main()
{

    std::shared_ptr<reactor> _reactor = std::make_shared<reactor>();
    std::shared_ptr<base_handler> _handler1 = std::make_shared<my_handler>(50000);
    std::shared_ptr<wakeup_handler> _handler2 = std::make_shared<my_wakeup_handler>();

    _reactor->attach(_handler1);
    _reactor->attach(_handler2);
    _reactor->add_io_handler(_handler1, EV_READ);
    _reactor->add_timer_handler(_handler1, std::dynamic_pointer_cast<my_handler>(_handler1)->millisecond());
    std::thread thread1(async_call, std::ref(_reactor), std::ref(_handler2));
    thread1.detach();
    _reactor->run();
    std::cout << "end of life" << std::endl;
    return 0;
}
