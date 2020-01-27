#include "sdlwindow.hpp"
#include "carefulgl.hpp"

sdlWindow& sdlWindow::init() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  TTF_Init(); 

  window = SDL_CreateWindow("SDL",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

  renderer = SDL_CreateRenderer(window, -1, 0);
  glcontext = SDL_GL_CreateContext(window);

  glewInit();
  glEnable(GL_BLEND);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  return *this;
}

sdlWindow::~sdlWindow() {
  glDeleteProgram(_program);
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  atexit(SDL_Quit);
}

void sdlWindow::loop() {
  while (!_should_close) {
    SDL_GL_MakeCurrent(window, glcontext);
    _handleEvent();

    _render();
    SDL_GL_SwapWindow(window);
  }
}

void sdlWindow::_use_shaders(char const *vert_shader_s,
                                char const *frag_shader_s) {

  GLuint vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vert_shader_s, NULL);
  carefully::glCompileShader(vert);

  GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &frag_shader_s, NULL);
  carefully::glCompileShader(frag);

  GLuint program = glCreateProgram();
  glAttachShader(program, vert);
  glAttachShader(program, frag);
  glLinkProgram(program);
  sdlWindow::_program = program;
}
