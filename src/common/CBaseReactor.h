//
// Created by david on 3/22/25.
//

#ifndef CBASE_REACTOR_H
#define CBASE_REACTOR_H
#include <iostream>
#include <memory>

class CBaseEvent;
class CEventLoop;

class CBaseReactor {
  friend class CEventLoop;
public:
  CBaseReactor() = default;
  virtual ~CBaseReactor() {
    std::cout << "CBaseReactor::~CBaseReactor()" << std::endl;
  };
  virtual bool AddEvent(const std::shared_ptr<CBaseEvent>& Event,int Events) = 0 ;
  virtual bool RemoveEvent(const std::shared_ptr<CBaseEvent>& Event,int Events) = 0;
  virtual void Poll(uint64_t Millisecond) = 0;
  const std::weak_ptr<CEventLoop>& GetEventLoop() const {return EventLoop;};
protected:
  void Attach(const std::shared_ptr<CEventLoop>& EventLoop);
private:
  std::weak_ptr<CEventLoop> EventLoop;
};



#endif //CBASE_REACTOR_H
