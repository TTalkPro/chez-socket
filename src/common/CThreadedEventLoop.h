//
// Created by david on 3/22/25.
//

#ifndef CTHREADEDEVENTLOOP_H
#define CTHREADEDEVENTLOOP_H
#include "CBaseEventLoop.h"
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>

class CThreadedEventLoop final : public CBaseEventLoop {
public:
  CThreadedEventLoop(const std::shared_ptr<CBaseReactor> &reactor)
      : CBaseEventLoop(reactor) {
    Type = EVENTLOOP_TYPE_THREADED;
  };
  ~CThreadedEventLoop();

  bool AddHandler(const std::shared_ptr<CBaseHandler> &Handler) override;
  void RemoveHandler(const std::shared_ptr<CBaseHandler> &Handler) override;
  void AddTask(const std::shared_ptr<CBaseTask>& Task) override;
  void RemoveTask(const std::shared_ptr<CBaseTask>& Task) override;
  bool InEventLoop() override;
  void Poll() override;
  // ThreadedEvnetLoop 独有的
  bool Start();
  void Stop();
  bool IsRunning() { return Running.load(); };
  void Join() {
    if (nullptr != Thread) {
      Thread->join();
    }
  }

protected:
  void ThreadEntry();
  void Run();
private:
  std::mutex TaskMutex;
  std::unique_ptr<std::thread> Thread = nullptr;
  std::atomic<bool> Running = false;
};

#endif // CTHREADEDEVENTLOOP_H
