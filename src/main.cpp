#include <shared_mutex>
#include<unistd.h>
#include <memory>
#include <iostream>
#include "reactor.h"
#include "common/io_handler.h"
#include "common/tcp_handler.h"
#include "common/wakeup_operation.h"

#include <thread>


std::string g_str = std::string("Hello World!");

class my_handler : public timer_handler {
public:
  my_handler(uint64_t millisecond): timer_handler() {
    _millisecond = millisecond;
  }

  ~my_handler() {
    std::cout << "the my_handler destruct" << std::endl;
  }
  uint64_t millisecond() const { return _millisecond; };
protected:
  void handle_timeout() override {
    if (attached_reactor() != nullptr) {
      std::cout << "stop the reactor after: " << _millisecond << "ms" <<
          std::endl;
      attached_reactor()->stop();
    }
  }



private:
  char _buffer[1024];
  uint64_t _millisecond;
};

class my_wakeup_operation : public wakeup_operation {
public:
  my_wakeup_operation() = default;

  ~my_wakeup_operation() = default;

  void handle_wakeup(void *data, size_t size) override {
    std::cout << "the my_wakeup_operation handle_wakeup: " << std::endl;
    if (size > 0) {
      std::string str = std::string((const char *)data, size);
      std::cout << str << std::endl;
    }

  }
};

void async_call(std::shared_ptr<reactor> &r) {
  my_wakeup_operation *op = new my_wakeup_operation();
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  std::cout << "send a wakeup message" << std::endl;
  r->wakeup(op, (void *)g_str.c_str(), g_str.length());
}

int main() {
  std::shared_ptr<reactor> _reactor = reactor::instance();
  std::shared_ptr<base_handler> _handler1 = std::make_shared<my_handler>(50000);
  // _reactor->add_io_handler(_handler1, EV_READ);
  _reactor->add_timer_handler(_handler1, std::dynamic_pointer_cast<my_handler>(_handler1)->millisecond());
  std::thread thread1(async_call, std::ref(_reactor));
  thread1.detach();
  // _reactor->run();
  // std::thread thread1(async_call,std::ref(_reactor));
  // thread1.join();
  _reactor->run();
  std::cout << "end of life" << std::endl;
  return 0;
}