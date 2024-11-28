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


class my_handler :public timer_handler, public io_handler{
public:
    my_handler(uint64_t millisecond): timer_handler(millisecond), io_handler(STDIN_FILENO,EV_READ){

    }
    ~my_handler(){
        std::cout << "the my_handler destruct" << std::endl;
    }
    void handle_events(int events) override {
        std::cout << "the my_handler handle events: " << events <<  std::endl;
        if(events & EV_TIMEOUT) {
            reactor* r = base_handler::attached_reactor();
            if (r != nullptr) {
                std::cout << "stop the reactor after: " << millisecond() << "ms" <<  std::endl;
                r->stop();
            }
        }
        if(events & EV_READ){
            int n = read(fd(),buffer,1023);
            buffer[n] = 0;
            std::string _str = std::string(buffer);
            std::cout <<"get "<< n << "bytes: " << _str <<std::endl;
        }
    }
private:
    char buffer[1024];
};




int main() {
//    std::shared_ptr <timer_manager> _timer_manager = std::make_shared<timer_manager>();
//    std::shared_ptr <timer_handler> _handler1 = std::make_shared<my_timer_handler>();
//    std::shared_ptr <timer_handler> _handler2 = std::make_shared<my_timer_handler>();
//    std::cout << "handler1: " << _handler1 << " handler2: " << _handler2 << std::endl;
//    _timer_manager->add_item(_handler1, 500, true);
//    _timer_manager->add_item(_handler2, 400, false);
//    timer_item _item;
//    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//    for(int i = 0; i < 7; i ++){
//        std::cout << "round: " << i << std::endl;
//        _timer_manager->get_latest_item(_item);
//        log_timer_item(_item);
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        _timer_manager->remove_first();
//        _item.handler.reset();
//        if( i == 5){
//            _timer_manager->remove_item(_handler1);
//        }
//    }
    std::shared_ptr<reactor> _reactor = std::make_shared<reactor>();
    std::shared_ptr<timer_handler> _handler1 = std::make_shared<my_handler>(500);
    _reactor->add_handler(_handler1);
    _reactor->run();
    std::cout << "end of life" << std::endl;
    return 0;
}