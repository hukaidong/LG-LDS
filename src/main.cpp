#include "fileio.hpp"
#include "debuggl.hpp"

#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>

#ifndef PROJ_DIR
#define PROJ_DIR "."
#endif

#include "fileio.hpp"
#include "main.hpp"
#include "carefulgl.hpp"
#include "glm/glm.hpp"
#define GLM_FORCE_RADIUS
#include "glm/ext.hpp"


void _sec_mvp(GLuint _program, bool show) {
  GLuint attr_idx = glGetUniformLocation(_program, "mvp");
  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, -0.5, 0.0));
  glm::mat4 view = glm::lookAt(glm::vec3(2.0, 2.0, -5.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 projection = glm::infinitePerspective(
      (0.5f*std::sin(0.3f*(float)clock()/CLOCKS_PER_SEC)+0.8f), (GLfloat)(640.0f / 480.0f), 0.1f);
  glm::mat4 mvp = projection * view * model;

  if (show) {
    auto showvec = [](const char*s, glm::vec4 a) {
      printf("%s\n", s);
      printf("%5.2f, %5.2f, %5.2f, %5.2f\n", a[0], a[1], a[2], a[3]);
      fflush(stdout);
    };

    auto showmtx = [](const char* s, glm::mat4 a) {
      printf("%s\n", s);
      printf("%5.2f, %5.2f, %5.2f, %5.2f\n", a[0][0], a[1][0], a[2][0], a[3][0]);
      printf("%5.2f, %5.2f, %5.2f, %5.2f\n", a[0][1], a[1][1], a[2][1], a[3][1]);
      printf("%5.2f, %5.2f, %5.2f, %5.2f\n", a[0][2], a[1][2], a[2][2], a[3][2]);
      printf("%5.2f, %5.2f, %5.2f, %5.2f\n", a[0][3], a[1][3], a[2][3], a[3][3]);
      fflush(stdout);
    };

    glm::vec4 orig(0, 0, 1, 1);
    showmtx("model", model);
    showvec("model", model * orig);
    showmtx("view", view);
    showvec("view", view * model * orig);
    showmtx("projection", projection);
    showvec("proj", projection * view * model * orig);
    showmtx("mvp", mvp);

  } else {
    glUniformMatrix4fv(attr_idx, 1, GL_FALSE, glm::value_ptr(mvp));
  }
}


void testSDLW::glsl_program() {
  auto vsdr = fromfile(vshaderloc);
  auto fsdr = fromfile(fshaderloc);
  _use_shaders(vsdr.get(), fsdr.get());
}

void testSDLW::glsl_data() {
  GLuint attr_idx, bd_idx=0, buffer[2];
  attr_idx = glGetAttribLocation(_program, "cube");

  auto [pary, vary] = loadObj(PROJ_DIR "/../obj/body.obj");

  glUseProgram(_program);
  glCreateBuffers(2, buffer);
  glNamedBufferStorage(buffer[0], sizeof(GLuint)*vary.size(), vary.data(), 0);
  glNamedBufferStorage(buffer[1], sizeof(GLfloat)*pary.size(), pary.data(), 0);

  glCreateVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexArrayAttrib(vao, attr_idx);
  glVertexArrayVertexBuffer(vao, bd_idx, buffer[1], 0, sizeof(GL_FLOAT)*3);
  glVertexArrayAttribFormat(vao, attr_idx, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(vao, attr_idx, bd_idx);

  glVertexArrayElementBuffer(vao, buffer[0]);

  //_sec_mvp(_program, true);

  glBindVertexArray(0);
  glUseProgram(0);
}

void testSDLW::_render() {
  color4f white = {1, 1, 1, 1},
          black = {0, 0, 0, 1};

  glUseProgram(_program);

  glClearColor(COLORP4F2U(&black));
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  _sec_mvp(_program, false);
  // primitive render {
  glBindVertexArray(vao);
  int esize;

  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &esize);
  glDrawElements(GL_TRIANGLES, esize/sizeof(GLuint), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  // } primitive render

  glUseProgram(0);

  // text render {
  int texW = 0, texH = 0;

  SDL_Surface* smsg = 
    TTF_RenderText_Blended(Sans, "Cube", {COLORP4F2U(&white)});

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

int main() {
  testSDLW().
    init().
    loop();

  return 0;
}

