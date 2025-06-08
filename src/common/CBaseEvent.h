//
// Created by david on 3/22/25.
//

#ifndef CBASE_EVENT_H
#define CBASE_EVENT_H
#include "../LispAsync.h"
#include <memory>

class CEventLoop;

class CBaseEvent : public std::enable_shared_from_this<CBaseEvent>  {
public:
  CBaseEvent() = default;
  CBaseEvent(FDType Handle){this->Handle = Handle;};
  virtual ~CBaseEvent();
  FDType GetHandle() const {return Handle;}
  void SetHandle(FDType Handle) {this->Handle = Handle;}

  int GetIndex(){ return Index;}
  void SetIndex(int Index){this->Index = Index;}

  int GetEvents() const {return Events;}
  void SetEvents(int Events) {this->Events |= Events;}
  void ClearEvents(int Events) {this->Events &= ~Events;}

  const std::weak_ptr<CEventLoop>& GetEventLoop() const {return EventLoop;}

  virtual void HandleEvent(int Events) {};
  void Attach(const std::shared_ptr<CEventLoop>& EventLoop);
  void Detach();

private:
  std::weak_ptr<CEventLoop> EventLoop;
  FDType Handle = -1;
  int Events = LISP_ASYNC_EVENT_NONE;
  int Index = -1;
};



#endif //CBASE_EVENT_H
