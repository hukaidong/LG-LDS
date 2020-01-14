#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include "GL/glew.h"
#include "SDL.h"
#include "SDL_ttf.h"

struct color4f { float r; float g; float b; float a; };

class sdlWindow {
protected:
  GLuint _program;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_GLContext glcontext;

  virtual void _setup() {};
  virtual void _render() = 0;

  void _use_shaders(const char*, const char *);

private:
  bool _should_close = false;
  void _handleEvent();
  void _handleWindowEvent(SDL_WindowEvent &event);


public:
  void loop();
  sdlWindow& init();

  sdlWindow() {};
  ~sdlWindow();
};

#define COLORP4F2U(target)     \
    (Uint8) ((target)->r * 0xFF), \
    (Uint8) ((target)->g * 0xFF), \
    (Uint8) ((target)->b * 0xFF), \
    (Uint8) ((target)->a * 0xFF)  \
 

#define COLORP4U2F(target)     \
    (float) ((target)->r) / 0xFF, \
    (float) ((target)->g) / 0xFF, \
    (float) ((target)->b) / 0xFF, \
    (float) ((target)->a) / 0xFF  \


#endif
