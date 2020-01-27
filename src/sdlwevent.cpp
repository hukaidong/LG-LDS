#include "showdl.hpp"

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
  }
}

