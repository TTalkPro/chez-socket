//
// Created by david on 3/22/25.
//

#ifndef CEVENTBASE_H
#define CEVENTBASE_H
#include <thread>
#include <set>
#include <vector>
#include <mutex>
#include <queue>
#include <memory>
#include <atomic>
//整体上EventLoop要处理的只有2件事情
//一件事情是IO事件，另外一个就是异步任务
//异步任务分会时间任务和一般任务
class CHandlerBase;
class CTaskBase;

class CEventLoopBase : public std::enable_shared_from_this<CEventLoopBase> {
public:
  CEventLoopBase() = default;
  virtual ~CEventLoopBase();
  bool IsRunning() const { return Running.load(); }
  void Join() {
    if (Thread->joinable()) {
      Thread->join(); // 不是detach才去join
    }
  }
  bool Start();
  bool InEventLoop(); //如果在当前EventLoop的线程中
  bool AddHandler(const std::shared_ptr<CHandlerBase>& Handler);
  void RemoveHandler(const std::shared_ptr<CHandlerBase>& Handler);

protected:
  void ThreadEntry();
  virtual void Run() = 0; // 运行
  virtual bool CanAddHandler(const std::shared_ptr<CHandlerBase>& Handler) = 0;
  void MaybeResize(int Size);
protected:
  //Pending状态是属于强指针，因此要先让Handler执行UnregisterOnEventLoop
  //才能进行删除
  std::set<std::shared_ptr<CHandlerBase>, std::owner_less<>> PendingHandlers;
  std::vector<std::weak_ptr<CHandlerBase>> Handlers;
  //任务队列
  std::mutex TasksMutex;
  std::queue<std::shared_ptr<CTaskBase>> Tasks;
private:
  std::atomic<bool> Running = false;
  std::unique_ptr<std::thread>Thread = nullptr;

};

#endif // CEVENTBASE_H
