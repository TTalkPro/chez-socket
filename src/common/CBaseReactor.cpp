//
// Created by david on 3/22/25.
//

#include "CBaseReactor.h"
#include "CBaseHandler.h"
void CBaseReactor::MaybeResize(int Size) {
  if (Handlers.capacity() < Size + 1) {
    Handlers.resize(Size + 1);
  }
}


bool CBaseReactor::AddHandler(const std::shared_ptr<CBaseHandler> &Handler) {
  bool ret = false;
  int RawHandler = Handler->GetRawHandler();
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
  if (RawHandler != -1) {
    if (RawHandler < Handlers.size()) {
      // 说明已经在Handlers中了
      Handlers[RawHandler].reset();
    } else {
      // 在pendings的状态下就被移除了
      // 正常情况下是不应该发生的
      std::set<std::shared_ptr<CBaseHandler>,
               std::owner_less<>>::iterator it = PendingHandlers.find(Handler);
      if (it != PendingHandlers.end()) {
        PendingHandlers.erase(it);
      }
    }
  }
}