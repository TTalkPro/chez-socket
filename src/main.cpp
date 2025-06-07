#include "common/CBaseEvent.h"
#include "common/CBaseReactor.h"
#include "common/CEventLoop.h"

#include <iostream>
#include <memory>
#include <shared_mutex>
#include <unistd.h>

#include <thread>

class MainDummyReactor : public CBaseReactor {
public:
  MainDummyReactor(): CBaseReactor() {}
  ~MainDummyReactor() override = default;
  bool AddEvent(const std::shared_ptr<CBaseEvent>& Event,int Events) override {
    return true;
  };
  bool RemoveEvent(const std::shared_ptr<CBaseEvent>& Event,int Events) override {
    return false;
  };
  void Poll(uint64_t Millisecond) override {
    std::this_thread::sleep_for(std::chrono::milliseconds(Millisecond));
  };
};

int main() {
  std::shared_ptr<MainDummyReactor> Reactor = std::make_shared<MainDummyReactor>();
  std::shared_ptr<CEventLoop> Loop =
      std::make_shared<CEventLoop>(Reactor);
  {
    std::shared_ptr<CBaseEvent> Event = std::make_shared<CBaseEvent>(1);
    Loop->AddEvent(Event,LISP_ASYNC_EV_READ);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    Loop->RemoveEvent(Event);
  }

  Loop->Poll();
  return 0;
}