//
// Created by david on 6/7/25.
//

#ifndef LISP_ASYNC_WRAPPER_H
#define LISP_ASYNC_WRAPPER_H


extern "C" {
  void* lisp_async_create_loop();
  void lisp_async_destroy_loop(void* loop);
}
#endif //WRAPPER_H
