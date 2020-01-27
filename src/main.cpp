#include "sdlnogl.hpp"
#include "czmq.h"
#include <thread>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>

int main () {
  zsock_t *push = zsock_new_push ("inproc://showgl");
  zsock_t *pull = zsock_new_pull ("inproc://showgl");

  set_src(pull);
  auto drawer = std::thread(main_loop);
  char msgbuf[1024];

  std::sprintf(msgbuf, "%s %f %f %f %f", 
    "set_colorf", 0.0f, 0.0f, 0.0f, 1.0f);
  zstr_send(push, msgbuf);

  zstr_send(push, "clear");

  std::sprintf(msgbuf, "%s %f %f", "set_xlims", 0.0f, 1.0f);
  zstr_send(push, msgbuf);
  std::sprintf(msgbuf, "%s %f %f", "set_ylims", 0.0f, 1.0f);
  zstr_send(push, msgbuf);

  std::sprintf(msgbuf, "%s %f %f %f %f", "set_colorf", 1.0f, 1.0f, 1.0f, 1.0f);
  zstr_send(push, msgbuf);
  for (int i=0; i<10000; i++) {
    std::sprintf(msgbuf, "%s %f %f", "draw_point", (float)rand()/RAND_MAX, (float)rand()/RAND_MAX);
    zstr_send(push, msgbuf);
  }

  std::sprintf(msgbuf, "%s %f %f %f %f", "set_colorf", 1.0f, 0.0f, 0.0f, 1.0f);
  zstr_send(push, msgbuf);
  std::sprintf(msgbuf, "%s %f %f %f %f", "draw_line", 0.0f, 0.0f, 1.0f, 0.0f);
  zstr_send(push, msgbuf);
  std::sprintf(msgbuf, "%s %f %f %f %f", "set_colorf", 0.0f, 1.0f, 0.0f, 1.0f);
  zstr_send(push, msgbuf);
  std::sprintf(msgbuf, "%s %f %f %f %f", "draw_line", 0.0f, 0.0f, 0.0f, 1.0f);
  zstr_send(push, msgbuf);

  drawer.join();
  zsock_destroy (&pull);
  zsock_destroy (&push);
  return 0;
}
