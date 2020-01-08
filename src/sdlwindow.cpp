#include "sdlwindow.hpp"
#include "carefulgl.hpp"

void sdlWindow::_handleEvent() {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_WINDOWEVENT:
    _handleWindowEvent(event.window);
    break;
  case SDL_QUIT:
    _should_close = true;
    break;
  };
}

void sdlWindow::_handleWindowEvent(SDL_WindowEvent &event) {
  switch (event.event) {
  case SDL_WINDOWEVENT_RESIZED:
    int width = event.data1;
    int length = event.data2;
    glViewport(0, 0, width, length);
    glDepthRangef(0, 100);
  }
}

sdlWindow& sdlWindow::init() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  TTF_Init(); 

  window = SDL_CreateWindow("",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

  renderer = SDL_CreateRenderer(window, -1, 0);
  glcontext = SDL_GL_CreateContext(window);

  glewInit();
  glEnable(GL_DEPTH_TEST);

  _setup();
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
