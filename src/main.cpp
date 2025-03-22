#include "common/CBaseHandler.h"
#include "common/CBaseReactor.h"
#include "common/CThreadedEventLoop.h"

#include <iostream>
#include <memory>
#include <shared_mutex>
#include <unistd.h>

#include <thread>

class DummyReactor : public CBaseReactor {
public:
  DummyReactor(): CBaseReactor() {}
  ~DummyReactor() override = default;
  void Poll(uint64_t millisecond) override {
    std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
  };
  protected:
  bool CanAddHandler(const std::shared_ptr<CBaseHandler>& Handler) override {
    return true;
  }
};

int main() {
  std::shared_ptr<DummyReactor> Reactor = std::make_shared<DummyReactor>();
  std::shared_ptr<CThreadedEventLoop> Loop =
      std::make_shared<CThreadedEventLoop>(Reactor);
  Loop->Start();
  {
    std::shared_ptr<CBaseHandler> Handler = std::make_shared<CBaseHandler>(1);
    Handler->RegisterOnEventLoop(Loop);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    Handler->UnregisterOnEventLoop();
  }

  Loop->Join();
  return 0;
}