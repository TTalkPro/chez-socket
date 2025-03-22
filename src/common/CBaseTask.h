//
// Created by david on 3/22/25.
//

#ifndef CBASETASK_H
#define CBASETASK_H
#include <memory>

enum TASK_TYPE {
  TASK_TYPE_NONE = 0,
  TASK_TYPE_TIMER,
  TASK_TYPE_ASYNC
};

//Task应当由调用者创建，由调用者释放
//此处设计不完善，待优化
class CBaseTask: public std::enable_shared_from_this<CBaseTask>{
public:
  CBaseTask() = default;
  virtual  ~CBaseTask() = default;
  virtual void RunTask() = 0;

  TASK_TYPE GetTaskType() const {return Type;}
protected:
  TASK_TYPE Type = TASK_TYPE_NONE;
};



#endif //CBASETASK_H
