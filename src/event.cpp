#include "showdl.hpp"

void sdlWindow::_event_handleEvent() {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_WINDOWEVENT:
    _event_windowEvent(event.window);
    break;
  case SDL_QUIT:
    _event_should_close = true;
    break;
  };
}

void sdlWindow::_event_windowEvent(SDL_WindowEvent &event) {
  switch (event.event) {
  case SDL_WINDOWEVENT_RESIZED:
    width = event.data1;
    height = event.data2;
    glViewport(0, 0, width, height);
    _view_mvp_fresh();
  }
}

