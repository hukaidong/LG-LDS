#include "fileio.hpp"
#include "debuggl.hpp"

#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>

#ifndef PROJ_DIR
#define PROJ_DIR "."
#endif

#include "main.hpp"
#include "carefulgl.hpp"

void testSDLW::glsl_program() {
  auto vsdr = fromfile(vshaderloc);
  auto fsdr = fromfile(fshaderloc);
  _use_shaders(vsdr.get(), fsdr.get());
}

void testSDLW::glsl_data() {
  GLuint attr_idx, bd_idx=0, buffer[2];

  attr_idx = glGetAttribLocation(_program, "vertex");

  auto [indicesptr, indicesnum] = _make_indices();
  auto [verticsptr, verticsnum] = _make_vertices();

  glCreateBuffers(2, buffer);
  glNamedBufferStorage(buffer[0], sizeof(GLuint)*indicesnum, indicesptr.get(), 0);
  glNamedBufferStorage(buffer[1], sizeof(GLfloat)*verticsnum, verticsptr.get(), 0);

  glCreateVertexArrays(1, &vao);
  glEnableVertexArrayAttrib(vao, attr_idx);
  glVertexArrayAttribFormat(vao, attr_idx, 2, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(vao, attr_idx, bd_idx);

  glVertexArrayElementBuffer(vao, buffer[0]);
  glVertexArrayVertexBuffer(vao, bd_idx, buffer[1], 0, sizeof(GL_FLOAT)*2);
  glUseProgram(0);
}

void testSDLW::_render() {
  color4f white = {1, 1, 1, 1},
          black = {0, 0, 0, 1};

  glUseProgram(_program);
  GLuint uidx = glGetUniformLocation(_program, "scale");
  glUniform1f(uidx, sin((float)clock()/1000000));

  glClearColor(COLORP4F2U(&black));
  glClear(GL_COLOR_BUFFER_BIT);

  // primitive render {
  glBindVertexArray(vao);
  glDrawElements(GL_LINES, 1680, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  glUseProgram(0);
  // } primitive render

  // text render {
  int texW = 0, texH = 0;

  SDL_Surface* smsg = 
    TTF_RenderText_Shaded(Sans, "Rendered text",
        {COLORP4F2U(&white)},      // Foreground color
        {COLORP4F2U(&black)}       // Background color
    );

  SDL_Texture* txtr = SDL_CreateTextureFromSurface(renderer, smsg);
  SDL_QueryTexture(txtr, NULL, NULL, &texW, &texH);
  SDL_Rect dstrect = { 32, 32, texW, texH };
  SDL_RenderCopy(renderer, txtr, NULL, &dstrect);
  SDL_RenderPresent(renderer);

  SDL_DestroyTexture(txtr);
  SDL_FreeSurface(smsg);
  // } text render 
}

void testSDLW::_setup() {
  Sans = TTF_OpenFont(PROJ_DIR "/fonts/FreeSans.ttf", 18);

  glsl_program();
  glsl_data();
}

std::pair<std::unique_ptr<GLfloat[]>, int> testSDLW::_make_vertices () {
  auto xymesh = std::unique_ptr<GLfloat[]>(new GLfloat[882]);
  for (int x = 0; x < 21; x++)
    for (int y = 0; y < 21; y++) {
      xymesh[42*y+2*x  ] = x * 0.1f - 1.0f;
      xymesh[42*y+2*x+1] =  y * 0.1f - 1.0f;
    }

  return std::make_pair(std::move(xymesh), 21*21*2);
}

std::pair<std::unique_ptr<GLuint[]>, int> testSDLW::_make_indices (){
  auto xymesh = std::unique_ptr<GLuint[]>(new GLuint[2000]);
  auto start = xymesh.get(), p = start;
  for (; p < start+2000; p++) {
    *p = 0;
  }

  p=start;
  for (int x = 0; x < 20; x++)
    for (int y = 0; y < 20; y++) {
      int vi = y*21 + x;
      *(p  ) = vi;
      *(p+1) = vi+1;
      *(p+2) = vi;
      *(p+3) = vi+21;
      p+=4;
    }

  for (int x=0; x<20; x++) {
    const int li = 21*20;
    *(p  ) = 21*(x+1) - 1;
    *(p+1) = 21*(x+2) - 1;
    *(p+2) = li + x    ;
    *(p+3) = li + x + 1;
    p+=4;
  }

  return std::make_pair(std::move(xymesh), p-start);
}


int main() {
  testSDLW().
    init().
    loop();

  return 0;
}

