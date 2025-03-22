//
// Created by david on 3/22/25.
//

#ifndef CTASKBASE_H
#define CTASKBASE_H
#include <memory>

//Task应当由调用者创建，由调用者释放
//此处设计不完善，待优化
class CTaskBase: public std::enable_shared_from_this<CTaskBase>{
public:
  CTaskBase() = default;
  ~CTaskBase() = default;

};



#endif //CTASKBASE_H
