//
// Created by david on 3/22/25.
//

#include "CBaseReactor.h"
#include "CBaseHandler.h"

#include <iostream>
#include <ostream>
void CBaseReactor::MaybeResize(int Size) {
  if (Handlers.capacity() < Size + 1) {
    Handlers.resize(Size + 1);
  }
}

void CBaseReactor::RemoveFromPendingSet(const std::shared_ptr<CBaseHandler>& Handler) {
  // 在pendings的状态下就被移除了
  // 正常情况下是不应该发生的
  PendingSet::iterator it = PendingHandlers.find(Handler);
  if (it != PendingHandlers.end()) {
    std::cout << "CBaseReactor::RemoveHandler: in PendingHandlers " << std::endl;
    PendingHandlers.erase(it);
  }
}

bool CBaseReactor::AddHandler(const std::shared_ptr<CBaseHandler> &Handler) {

  bool ret = false;
  int RawHandler = Handler->GetRawHandler();
  std::cout << "CBaseReactor::AddHandler: "<< RawHandler << std::endl;
  if (RawHandler != -1) {
    if (CanAddHandler(Handler)) {
      MaybeResize(RawHandler);
      PendingHandlers.insert(Handler);
      ret = true;
    }
  }
  return ret;
}


void CBaseReactor::RemoveHandler(const std::shared_ptr<CBaseHandler> &Handler) {

  int RawHandler = Handler->GetRawHandler();
  std::cout << "CBaseReactor::RemoveHandler: "<< RawHandler << std::endl;
  if (RawHandler != -1) {
    if (RawHandler < Handlers.size()) {
      // 说明已经在Handlers中了
      std::shared_ptr<CBaseHandler> Found = Handlers[RawHandler];
      std::cout << "CBaseReactor::RemoveHandler: in Handlers " << Found << std::endl;
      if (Found != nullptr) {
        Found.reset();
        Handlers.erase(Handlers.begin() + RawHandler);
      } else {
        RemoveFromPendingSet(Handler);
      }
    } else {
      RemoveFromPendingSet(Handler);
    }
  }
}