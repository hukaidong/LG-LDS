#ifndef GPOUT_H
#define GPOUT_H

#include "GL/glew.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "sdlwindow.hpp"

class GPPlot : public sdlWindow {

public:
  void setcolor(float, float, float, float);
  void setlims(float, float, float, float);

  void drawpoint(float, float);
  void drawline(float, float);
  void drawrect(float, float, float, float, bool, bool);

  void clear();

protected:
  virtual void _render();
};

#endif
