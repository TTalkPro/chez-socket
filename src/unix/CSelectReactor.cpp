//
// Created by david on 6/7/25.
//
#include <iostream>
#include "CSelectReactor.h"


void CSelectReactor::MaybeResize(FDType MaxHandle) {
  if (EventsSet.capacity() < MaxHandle + 1) {
    EventsSet.resize(MaxHandle + 1);
  }
}
bool CSelectReactor::RemoveEvent(const std::shared_ptr<CBaseEvent>& Event,int Events){
  PendingSet::iterator it = Pendings.find(Event);
  if (it != Pendings.end()) {
    // 在Pendings中
    if (Events == LISP_ASYNC_EVENT_NONE) {
      Pendings.erase(it);
    }else {
      Event->ClearEvents(Events);
      if (Event->GetEvents() & LISP_ASYNC_EV_IO == 0) {
        Pendings.erase(it);
      }
    }
    return true;
  }
  FDType Handle = Event->GetHandle();
  if (Handle != -1 && Handle < EventsSet.size()) {
      //说明已经在_handlers中了
    if (Events == LISP_ASYNC_EVENT_NONE) {
       EventsSet[Handle].reset();
    } else {
      Event->ClearEvents(Events);
      if (Event->GetEvents() & LISP_ASYNC_EV_IO) {
        EventsSet[Handle].reset();
      }
    }
    return true;
  }
  return false;
}
bool CSelectReactor::AddEvent(const std::shared_ptr<CBaseEvent>& Event,int Events) {
  bool Ret = false;
  FDType Handle = Event->GetHandle();
  if ( Handle != -1) {
    std::cout << "Add Event With Handler " << Handle  << std::endl;
    MaybeResize(Handle);
    if (Handle <  FD_SETSIZE) {
      if (Events != LISP_ASYNC_EVENT_NONE) {
        Event->SetEvents(Events);
      }
      Pendings.insert(Event);
      Ret = true;
    }
  }
  return Ret;
}
void CSelectReactor::Poll(uint64_t Millisecond) {
  int Results = 0;
  int FDCount = -1;
  int Events = 0;
  FD_ZERO(&ReadSet);
  FD_ZERO(&WriteSet);
  std::shared_ptr<CBaseEvent> Event;
  // 处理等待中的socket
  if (Pendings.size() > 0) {
    PendingSet::iterator it = Pendings.begin();
    FDType Handle = -1;
    while (it != Pendings.end()) {
      Handle = (*it)->GetHandle();
      if (Handle == -1) {
        it++;
        continue;
      }
      EventsSet[Handle] = (*it);
      it++;
    }
    Pendings.clear();
  }
  for (int i = 0; i < EventsSet.capacity(); i++) {
    Event = EventsSet[i].lock();
    if (Event != nullptr) {
      Events = Event->GetEvents();
      if (Events & LISP_ASYNC_EV_READ) {
        FD_SET(i, &ReadSet);
      }
      if (Events & LISP_ASYNC_EV_WRITE) {
        FD_SET(i, &WriteSet);
      }
      FDCount = i;
    }
  }
  if (Millisecond > 0) {
    TimeVal.tv_usec = Millisecond * 1000;
    Results = select(FDCount + 1, &ReadSet, &WriteSet, NULL, &TimeVal);
  } else {
    Results = select(FDCount + 1, &ReadSet, &WriteSet, NULL, NULL);
  }
  if (Results > 0) {
    for (int i = 0; i <= FDCount; i++) {
      Events = 0;
      if (FD_ISSET(i, &ReadSet)) {
        Events |= LISP_ASYNC_EV_READ;
      }
      if (FD_ISSET(i, &WriteSet)) {
        Events |= LISP_ASYNC_EV_WRITE;
      }
      if (Events != 0) {
        Event = EventsSet[i].lock();
        if (Event != nullptr) {
          Event->HandleEvent(Events);
        }
      }
    }
  }
}