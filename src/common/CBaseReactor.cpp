//
// Created by david on 3/22/25.
//

#include "CBaseReactor.h"

class DummyReactor : public CBaseReactor {
  bool AddEvent(const std::shared_ptr<CBaseEvent>& Event,int Events) override {return false;};
  bool RemoveEvent(const std::shared_ptr<CBaseEvent>& Event,int Events) override {return false;};
  void Poll(uint64_t Millisecond) override {};
};


void CBaseReactor::Attach(
    const std::shared_ptr<CEventLoop> &EventLoop) {
  this->EventLoop = EventLoop;
}
