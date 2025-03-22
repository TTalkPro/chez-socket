//
// Created by david on 3/22/25.
//

#ifndef CTIMERTASK_H
#define CTIMERTASK_H
#include "CBaseTask.h"

class CTimerTask: virtual public CBaseTask {
public:
  CTimerTask(uint64_t Millisecond,bool Cycled):CBaseTask() {
    Type = TASK_TYPE_TIMER;
    this->Millisecond = Millisecond;
    this->Cycled = Cycled;
  };
  ~CTimerTask() override = default;
  uint64_t GetMillisecond() const{ return Millisecond; };
  bool GetCycled() const{ return Cycled; };
protected:
  uint64_t Millisecond = 0;
  bool Cycled = false;
};

#endif //CTIMERTASK_H
