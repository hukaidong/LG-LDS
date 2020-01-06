#ifndef MAIN_H
#define MAIN_H

#include <memory>

#include "GL/glew.h"
#include "SDL_ttf.h"

#ifndef PROJ_DIR
#define PROJ_DIR "."
#endif

#include "sdlwindow.hpp"

class testSDLW: public sdlWindow {
  TTF_Font *Sans;
  GLuint vao;

  const char *vshaderloc{PROJ_DIR "/shdr/test.v.glsl"};
  const char *fshaderloc{PROJ_DIR "/shdr/test.f.glsl"};
  const char *fontloc{PROJ_DIR "/fonts/FreeSans.ttf"};

  void glsl_program();
  void glsl_data();

  std::pair<std::unique_ptr<GLfloat[]>, int> _make_vertices(); 
  std::pair<std::unique_ptr<GLuint[]>, int> _make_indices();

  protected:
  virtual void _render();
  virtual void _setup();
};

#endif

