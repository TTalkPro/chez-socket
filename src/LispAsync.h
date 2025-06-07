//
// Created by david on 11/28/24.
//

#ifndef LISP_ASYNC_H
#define LISP_ASYNC_H

#define LISP_ASYNC_EVENT_NONE 0x00 //
#define LISP_ASYNC_EV_READ 0x02    // 0010
#define LISP_ASYNC_EV_WRITE 0x04   // 0100
#define LISP_ASYNC_EV_IO 0x06      // 0110
#define LISP_ASYNC_EV_CLOSED 0x80  //关闭
#define LISP_ASYNC_EV_SIGNAL 0x81  //信号
#define LISP_ASYNC_EV_ERROR 0x82   //异常
#define LISP_ASYNC_EV_TIMEOUT 0x83 //超时
#define LISP_ASYNC_EV_TASK 0x84    //异步任务

#if defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) ||      \
    defined(__DragonFly__) || defined(__NetBSD__)
#define FDType int
#endif
extern "C" {
  typedef  void (*event_callback)(void*, int events);
}

#endif // LISP_ASYNC_H
