//
// Created by david on 6/8/25.
//

#ifndef CBASE_SOCKET_H
#define CBASE_SOCKET_H

#include "./CBaseEvent.h"
#include "./CBuffer.h"

// 该类应该处理状态
enum SOCKET_STATE {
  SOCKET_UNINITIALIZED = 0,
  SOCKET_IDLE = 1,
  SOCKET_CLOSED = 2,
  SOCKET_CONNECTING = 3,
  SOCKET_CONNECTED = 4,
  SOCKET_ACCEPTING = 5,
};
class CBaseSocket: virtual public CBaseEvent {

public:
  void HandleEvent(int Events) override;
  bool Open(int Domain, int Type, int Protocol) = 0;
  int Read(CBuffer* Buffer,int Size) = 0;
  int Write(CBuffer* Buffer,int Size) = 0;
  void Connet(std::string IP,int Port) = 0;
  void Accept(std::string IP,int Port) = 0;
private:
  SOCKET_STATE State = SOCKET_IDLE;
  std::shared_ptr<CBuffer> ReadBuffer;
  std::shared_ptr<CBuffer> WriteBuffer;
};



#endif //CBASE_SOCKET_H
