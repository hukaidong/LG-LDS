#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include "Eigen/Dense"
#include "GL/glew.h"
#include "SDL.h"
#include "SDL_ttf.h"

struct color4f { float r; float g; float b; float a; };

using namespace Eigen;
class sdlWindow {
public:
  using vec3 = Matrix<GLfloat, 3, RowMajor>;
  using vec4 = Matrix<GLfloat, 4, RowMajor>;
  using mat3 = Matrix<GLfloat, 3, 3>;
  using mat4 = Matrix<GLfloat, 4, 4>;
  using quat = Quaternion<GLfloat>;

  void loop();
  void init();

  sdlWindow() {};
  ~sdlWindow();


protected:
  GLuint _program;
  GLint width, height;
  mat4 model, proj;

  virtual void _render() = 0;

  void _use_shaders(const char*, const char *);
  void _view_mvp_assign(GLuint);
  void _view_mvp_assign(GLuint, GLuint);

private:
  SDL_Window   *window;
  SDL_Renderer *renderer;
  SDL_GLContext glcontext;

  bool _event_should_close = false;
  void _event_handleEvent();
  void _event_windowEvent(SDL_WindowEvent &event);
  void _view_mvp_init();
  void _view_mvp_fresh();
  mat4 _view_transform(vec3, quat, vec3);
  mat4 _view_lookat(vec3, vec3, vec3);
  mat4 _view_prespective(GLfloat, GLfloat, GLfloat, GLfloat);
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
