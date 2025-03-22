//
// Created by david on 3/22/25.
//

#ifndef CTIMERTASK_H
#define CTIMERTASK_H
#include "CBaseTask.h"

class CTimerTask: virtual public CBaseTask {
public:
  CTimerTask(uint64_t Millisecond,bool Cycled):CBaseTask() {
    this->Millisecond = Millisecond;
    this->Cycled = Cycled;
  };
  ~CTimerTask() override = default;

};

#endif //CTIMERTASK_H
