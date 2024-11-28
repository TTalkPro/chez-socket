#include <memory>
#include <iostream>
#include <chrono>
#include <thread>
#include "event.h"
#include "timer_handler.h"
#include "timer_manager.h"
#include "io_handler.h"
#include "select_engine.h"

class my_timer_handler :public timer_handler {
public:
    my_timer_handler() = default;
    ~my_timer_handler(){
        std::cout << "the my_timer_handler destruct" << std::endl;
    }
    void handle_timeout() override {
        std::cout << "the my_timer_handler handle timeout" << std::endl;
    }
};


void log_timer_item(const timer_item& item)
{
    std::shared_ptr<timer_handler> _handler = item.handler.lock();
    if(_handler != nullptr) {
        if(item.remain_millisecond <= 0) {
            _handler->handle_timeout();
        }
        std::cout << "handler is:" << _handler  << " handler reference count: "<< _handler.use_count() << " next_expired_time: " << item.next_expired_time
                  << " remain_millisecond: " << item.remain_millisecond << std::endl;
    }

}



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
    std::shared_ptr<io_handler> _handler = std::make_shared<io_handler>(1,EV_TIMEOUT| EV_READ| EV_WRITE);
    std::shared_ptr<select_engine> _engine = std::make_shared<select_engine>();
    _engine->add_handler(_handler);
    std::cout << "end of life" << std::endl;
    return 0;
}