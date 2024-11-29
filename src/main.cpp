#include <sys/socket.h>
#include<unistd.h>
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>
#include "chez_socket.h"
#include "timer_handler.h"
#include "io_handler.h"
#include "reactor.h"


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


int main()
{
    std::shared_ptr<reactor> _reactor = std::make_shared<reactor>();
    std::shared_ptr<base_handler> _handler1 = std::make_shared<my_handler>(3000);
    _reactor->attach(_handler1);
    _reactor->add_io_handler(_handler1, EV_READ);
    _reactor->add_timer_handler(_handler1, std::dynamic_pointer_cast<my_handler>(_handler1)->millisecond());
    _reactor->run();
    std::cout << "end of life" << std::endl;
    return 0;
}
