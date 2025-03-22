//
// Created by david on 3/22/25.
//

#ifndef CHANDLERBASE_H
#define CHANDLERBASE_H
#include <memory>

class CEventLoopBase;

class CHandlerBase : public std::enable_shared_from_this<CHandlerBase>  {
public:
  CHandlerBase() = default;
  virtual ~CHandlerBase();
  int GetRawHandler() const {return RawHandler;}
  bool IsRegistered() const {return  Registered;};
  bool RegisterOnEventLoop(const std::shared_ptr<CEventLoopBase>& EventLoop);
  void UnregisterOnEventLoop();
protected:
  bool Registered = false;
  std::shared_ptr<CEventLoopBase> EventLoop = nullptr;
  int RawHandler = -1;
};



#endif //CHANDLERBASE_H
