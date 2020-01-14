#include "SDL.h"
#include "SDL_ttf.h"
#include "czmq.h"
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

// USAGE:
//   
//   STR_SWITCH("someword") {
//   STR_CASE_STARTWITH("abc") do something...
//   STR_CASE_STARTWITH("def") do something...
//   STR_DEFAULT() do something...

#define STR_SWITCH(x)           const char* _somestr = x; if(0) 
#define STR_CASE_STARTWITH(x)   } else if (strncmp(x, _somestr, strlen(x))==0) {
#define STR_CASE_EQLUAL(x)      } else if (strcmp(x, _somestr)==0) {
#define STR_DEFAULT()           } else {

#define ZMQ_SOCK zsock_t
#define ZMQ_POLLER zpoller_t

SDL_Window *window;
SDL_Renderer *renderer;
ZMQ_SOCK *sock;
ZMQ_POLLER *poller;

int redo_index = 0;
std::vector<std::string> redos{};

enum {SHOULD_QUIT, SHOULD_NOT_QUIT} 
QuitFlg = SHOULD_NOT_QUIT;

enum {NOTHING, FLUSH, MTX_RECALC, REDO} 
RenderFlg = FLUSH;


#define AXE2CAN(x, y) \
  (int)(wprop.axe2can[0]*x+wprop.axe2can[1]), \
  (int)(wprop.axe2can[2]*y+wprop.axe2can[3])

// Canvas coordinate Right-Down
struct {
  float axe2can[4];
  float can2axe[4];
  int width=640, height=480;
  float margin=0.1;          // length of marge size / length of canvas
  float limxmin, limxmax;
  float limymin, limymax;
} wprop;

void init();
void deinit();
void handleEvent() ;
void handleMessage() ;
void handleWindowEvent(SDL_WindowEvent &event) ;
void renderflush();
void recalTransformMatrix();

void set_src(ZMQ_SOCK *s) {
  sock = s;
  poller = zpoller_new(sock);
}

void main_loop() {
  init();
    while(QuitFlg == SHOULD_NOT_QUIT) {
      handleMessage();
      switch(RenderFlg){
      case(FLUSH): renderflush(); break;
      case(MTX_RECALC): recalTransformMatrix(); break;
      case(REDO): redo_index=0; RenderFlg = NOTHING;
      case(NOTHING): default:;
      }
      handleEvent();
    }
  deinit();
}


/* PRIVATE FUNCTIONS */

void set_colorf(std::string msg) {
  int klen = strlen(__FUNCTION__);
  float r, g, b, a;
  sscanf(msg.c_str()+klen, "%f %f %f %f", &r, &g, &b, &a);

  SDL_SetRenderDrawColor(renderer, 
      (uint)(r*0xff), (uint)(g*0xff), (uint)(b*0xff), (uint)(a*0xff));
}

void set_colori(std::string msg) {
  int klen = strlen(__FUNCTION__);
  unsigned int r, g, b, a;
  sscanf(msg.c_str()+klen, "%u %u %u %u", &r, &g, &b, &a);

  SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void set_xlims(std::string msg) {
  float min, max;
  int klen = strlen(__FUNCTION__);
  sscanf(msg.c_str()+klen, "%f %f", &min, &max);

  wprop.limxmin = min;
  wprop.limxmax = max;
  RenderFlg = MTX_RECALC;
}

void set_ylims(std::string msg) {
  float min, max;
  int klen = strlen(__FUNCTION__);
  sscanf(msg.c_str()+klen, "%f %f", &min, &max);

  wprop.limymin = min;
  wprop.limymax = max;
  RenderFlg = MTX_RECALC;
}

void renderflush() {
  float *p = wprop.axe2can;
  SDL_RenderPresent(renderer);
  RenderFlg = NOTHING;
}

void draw_point(std::string msg) {
  float x, y;
  int klen = strlen(__FUNCTION__);
  sscanf(msg.c_str()+klen, "%f %f", &x, &y);
  SDL_RenderDrawPoint(renderer, AXE2CAN(x, y));
  RenderFlg = FLUSH;
}

void draw_line(std::string msg) {
  float x1, y1, x2, y2;
  int klen = strlen(__FUNCTION__);
  sscanf(msg.c_str()+klen, "%f %f %f %f", &x1, &y1, &x2, &y2);

  SDL_RenderDrawLine(renderer, AXE2CAN(x1, y1), AXE2CAN(x2, y2));
  RenderFlg = FLUSH;
}

void recalTransformMatrix() {
  // TODO: wprop.can2axe =  
   
  float w = wprop.width,
        h = wprop.height,
        m = wprop.margin,
        x1 = wprop.limxmin,
        x2 = wprop.limxmax,
        y1 = wprop.limymin,
        y2 = wprop.limymax;

  // [a, b] ...  ax + b = canvas.x
  // [c, d] ...  cy + d = canvas.y
  //
  // Solving:
  // w.m     = a.x1+b
  // w.(1-m) = a.x2+b
  //
  // h.(1-m) = c.y1+d
  // h.m     = c.y2+d

  wprop.axe2can[0] = (2*w*m-w) / (x1-x2);
  wprop.axe2can[1] = ((x1+x2)*w*m - x1*w) / (x2-x1);

  wprop.axe2can[2] = (2*h*m-h) / (y2-y1);
  wprop.axe2can[3] = ((y1+y2)*h*m - y2*h) / (y1-y2);

  RenderFlg = REDO;
}



void init() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  TTF_Init(); 

  window = SDL_CreateWindow("title",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      wprop.width, wprop.height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

  renderer = SDL_CreateRenderer(window, -1, 0);
}

void deinit() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  atexit(SDL_Quit);
}

void handleEvent() {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_WINDOWEVENT: 
      handleWindowEvent(event.window); 
    break;

    case SDL_QUIT: 
      QuitFlg = SHOULD_QUIT;
    };
  }
}

void handleWindowEvent(SDL_WindowEvent &event) {
  switch (event.event) {
  case SDL_WINDOWEVENT_RESIZED:
    wprop.width = event.data1;
    wprop.height = event.data2;
    RenderFlg = MTX_RECALC;
    SDL_RenderSetViewport(renderer, NULL);
  }
}

std::string make_redo(std::string msg) {
  if (redo_index == redos.size()) {
    redos.push_back(msg);
    redo_index++;
  } 
  return msg;
}

bool getCmdSquence(std::string &msg) {
  // if redo, handle them first
  if (redo_index < redos.size()) {
    std::string todo(redos[redo_index]);
    redo_index++;
    msg.swap(todo);
    return true;
  }

  // otherwise get new commands
  zpoller_wait(poller, 0);
  if (zpoller_expired(poller)) {
    return false;
  }
  if (zpoller_terminated(poller)){
    QuitFlg = SHOULD_QUIT;
    return false;
  }

  char* todo_recv =  zstr_recv(sock);
  std::string todo(todo_recv);
  msg.swap(todo);
  zstr_free(&todo_recv);
  return true;
}

void handleMessage() {
  std::string msg;
  while (getCmdSquence(msg)) {
    STR_SWITCH(msg.c_str()) {
    STR_CASE_STARTWITH("clear") SDL_RenderClear(renderer); break;
    STR_CASE_STARTWITH("set_colorf") set_colorf(make_redo(msg)); break;
    STR_CASE_STARTWITH("set_colori") set_colori(make_redo(msg)); break;
    STR_CASE_STARTWITH("set_xlims")  set_xlims(msg); break;
    STR_CASE_STARTWITH("set_ylims")  set_ylims(msg); break;

    STR_CASE_STARTWITH("draw_point") draw_point(make_redo(msg));
    STR_CASE_STARTWITH("draw_line")  draw_line(make_redo(msg));
    STR_CASE_STARTWITH("draw_rect")  puts("draw_rect not implement yet");
    STR_CASE_STARTWITH("draw_text")  puts("draw_text not implement yet");
    STR_DEFAULT() 
      std::cerr << "Error: message not handled " << msg << std::endl;
      fflush(NULL);
    }
  }
}
