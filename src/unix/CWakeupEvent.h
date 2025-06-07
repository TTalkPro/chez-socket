//
// Created by david on 6/7/25.
//

#ifndef CWAKEUP_EVENT_H
#define CWAKEUP_EVENT_H
#include "../LispAsync.h"
#include "../common/CBaseEvent.h"

class CWakeupEvent final :public  CBaseEvent {
public:
  CWakeupEvent();
  ~CWakeupEvent() override;
  void HandleEvent(int Events) override;
  void Wakeup() ;
private:
  //WakeupHandles[0]是读取端
  //WakeupHandles[1]是写入端
  FDType WakeupHandles[2];
};



#endif //CWAKEUP_H
