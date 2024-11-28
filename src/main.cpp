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
    my_handler(uint64_t millisecond): timer_handler(millisecond), io_handler(0,EV_READ){

    }
    ~my_handler(){
        std::cout << "the my_handler destruct" << std::endl;
    }
    void handle_events(int events) override {
        std::cout << "the my_handler handle events: " << events <<  std::endl;
        if(events & EV_TIMEOUT) {
            if ( attached_reactor() != nullptr) {
                std::cout << "stop the reactor after: " << millisecond() << "ms" <<  std::endl;
                 attached_reactor()->stop();
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
    std::shared_ptr<reactor> _reactor = std::make_shared<reactor>();
    std::shared_ptr<timer_handler> _handler1 = std::make_shared<my_handler>(50000);
    _reactor->add_handler(_handler1);
    _reactor->run();
    std::cout << "end of life" << std::endl;
    return 0;
}