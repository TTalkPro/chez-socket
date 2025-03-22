//
// Created by david on 3/22/25.
//

#ifndef CBASETASK_H
#define CBASETASK_H
#include <memory>

//Task应当由调用者创建，由调用者释放
//此处设计不完善，待优化
class CBaseTask: public std::enable_shared_from_this<CBaseTask>{
public:
  CBaseTask() = default;
  virtual  ~CBaseTask() = default;
  virtual void RunTask() = 0;
  uint64_t GetMillisecond() const{ return Millisecond; };
  bool GetCycled() const{ return Cycled; };
protected:
  uint64_t Millisecond = 0;
  bool Cycled = false;
};



#endif //CBASETASK_H
