//
// Created by david on 6/7/25.
//

#ifndef CSELECT_REACTOR_H
#define CSELECT_REACTOR_H
#include "../LispAsync.h"
#include "../common/CBaseEvent.h"
#include "../common/CBaseReactor.h"
#include <memory>
#include <set>
#include <sys/select.h>
#include <vector>

#include <cstring>

class CSelectReactor final: public  CBaseReactor {
public:
  CSelectReactor():CBaseReactor() {
    FD_ZERO(&ReadSet);
    FD_ZERO(&WriteSet);
    memset(&TimeVal, 0, sizeof(struct timeval));
  };
  bool AddEvent(const std::shared_ptr<CBaseEvent>& Event,int Events) override;
  bool RemoveEvent(const std::shared_ptr<CBaseEvent>& Event,int Events) override;
  void Poll(uint64_t Millisecond) override;
protected:
  //CSelectReactor(const  CSelectReactor&) = delete;
  //CSelectReactor& operator=(const CSelectReactor) = delete;
  void MaybeResize(FDType MaxHandle);
private:
  typedef std::set<std::shared_ptr<CBaseEvent>, std::owner_less<>> PendingSet;
  PendingSet Pendings;
  typedef std::vector<std::weak_ptr<CBaseEvent>> EventsVector;
  EventsVector EventsSet;
  fd_set ReadSet;
  fd_set WriteSet;
  struct timeval TimeVal;
};



#endif //CSELECT_REACTOR_H
