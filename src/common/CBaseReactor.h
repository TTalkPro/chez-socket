//
// Created by david on 3/22/25.
//

#ifndef BASECREACTOR_H
#define BASECREACTOR_H
#include <memory>
#include <set>
#include <vector>

class CBaseHandler;

class CBaseReactor {
public:
  CBaseReactor() = default;
  virtual ~CBaseReactor() = default;
  bool AddHandler(const std::shared_ptr<CBaseHandler>& Handler);
  void RemoveHandler(const std::shared_ptr<CBaseHandler>& Handler);
  virtual void Poll(uint64_t millisecond) = 0;
protected:
  virtual bool CanAddHandler(const std::shared_ptr<CBaseHandler>& Handler) = 0;
  void MaybeResize(int Size);
  void RemoveFromPendingSet(const std::shared_ptr<CBaseHandler>& Handler);
  //Pending状态是属于强指针，因此要先让Handler执行UnregisterOnEventLoop
  //才能进行删除
  typedef  std::set<std::shared_ptr<CBaseHandler>, std::owner_less<>> PendingSet;
  PendingSet PendingHandlers;
  typedef std::vector<std::shared_ptr<CBaseHandler>> HandlerVector;
  HandlerVector Handlers;
};



#endif //BASECREACTOR_H
