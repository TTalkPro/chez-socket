//
// Created by david on 3/22/25.
//

#ifndef CBASEHANDLER_H
#define CBASEHANDLER_H
#include <memory>

class CBaseEventLoop;

class CBaseHandler : public std::enable_shared_from_this<CBaseHandler>  {
public:
  CBaseHandler() = default;
  virtual ~CBaseHandler();
  int GetRawHandler() const {return RawHandler;}
  bool IsRegistered() const {return  Registered;};
  bool RegisterOnEventLoop(const std::shared_ptr<CBaseEventLoop>& EventLoop);
  void UnregisterOnEventLoop();
protected:
  bool Registered = false;
  std::shared_ptr<CBaseEventLoop> EventLoop = nullptr;
  int RawHandler = -1;
};



#endif //CBASEHANDLER_H
