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
  glm::mat4 view = glm::lookAt(glm::vec3(2.0, 2.0, -10.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0001));
  glm::mat4 projection = glm::infinitePerspective(
      (0.5f*std::sin(0.3f*(float)clock()/CLOCKS_PER_SEC)+0.8f), 
      //1.1f,
      (GLfloat)(640.0f / 480.0f), 0.1f);
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
  debugenable();
}

void testSDLW::glsl_data() {
  GLuint attr_idx[2], bd_idx[]={0, 1, 2}, buffer[4];

  attr_idx[0] = glGetAttribLocation(_program, "cube");
  attr_idx[1] = glGetAttribLocation(_program, "color");

  auto [pary, vary] = loadObj(PROJ_DIR "/../obj/body.obj");
  GLfloat ground[] = {
    -10, -3,  10,
    -10, -3, -10,
     10, -3, -10,
    -10, -3,  10,
     10, -3,  10,
     10, -3, -10,
  };
  GLfloat col[] = {1, 1, 1};


  glUseProgram(_program);
  
  glCreateBuffers(4, buffer);
  glNamedBufferStorage(buffer[0], sizeof(GLuint)*vary.size(), vary.data(), 0);
  glNamedBufferStorage(buffer[1], sizeof(GLfloat)*pary.size(), pary.data(), 0);
  glNamedBufferStorage(buffer[2], sizeof(ground), ground, 0);
  glNamedBufferStorage(buffer[3], sizeof(col), col, 0);

  glCreateVertexArrays(2, vaos);

  glEnableVertexArrayAttrib(vaos[0], attr_idx[0]);
  glEnableVertexArrayAttrib(vaos[0], attr_idx[1]);
  glVertexArrayAttribFormat(vaos[0], attr_idx[0], 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribFormat(vaos[0], attr_idx[1], 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayVertexBuffer(vaos[0], bd_idx[0], buffer[1], 0, sizeof(GLfloat)*3);
  glVertexArrayAttribBinding(vaos[0], attr_idx[0], bd_idx[0]);
  glVertexArrayAttribBinding(vaos[0], attr_idx[1], bd_idx[0]);
  glVertexArrayElementBuffer(vaos[0], buffer[0]);


  glEnableVertexArrayAttrib(vaos[1], attr_idx[0]);
  glEnableVertexArrayAttrib(vaos[1], attr_idx[1]);
  glVertexArrayAttribFormat(vaos[1], attr_idx[0], 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribFormat(vaos[1], attr_idx[1], 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayVertexBuffer(vaos[1], bd_idx[1], buffer[2], 0, sizeof(GLfloat)*3);
  glVertexArrayVertexBuffer(vaos[1], bd_idx[2], buffer[3], 0, 0);
  glVertexArrayAttribBinding(vaos[1], attr_idx[0], bd_idx[1]);
  glVertexArrayAttribBinding(vaos[1], attr_idx[1], bd_idx[2]);

  glUseProgram(0);
}

float LastFrame=0, fps=0;
const float alpha=0.95;

void testSDLW::_render() {
  color4f white = {1, 1, 1, 1},
          black = {0, 0, 0, 1};

  glUseProgram(_program);

  glClearColor(COLORP4F2U(&black));
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  _sec_mvp(_program, false);

  // primitive render {
  int esize;

  glBindVertexArray(vaos[0]);
  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &esize);
  glDrawElements(GL_TRIANGLES, esize/sizeof(GLuint), GL_UNSIGNED_INT, 0);

  glBindVertexArray(vaos[1]);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(0);
  // } primitive render

  glUseProgram(0);

  // text render {
  clock_t now = clock();
  fps = fps*alpha + (1.0f-alpha)*
    CLOCKS_PER_SEC/(float)((now - LastFrame)); 

  std::ostringstream msg_in; {
    msg_in.setf(std::ios::fixed);
    msg_in.precision(2);
    msg_in.width(5);
  }

  msg_in << "fps: " << fps << "\n";
  msg_in << "esize: " << esize/sizeof(GLfloat);
  
  char buf[20];
  int curW=5, curH=5, texW = 0, texH = 0;
  std::istringstream msg_out(msg_in.str());
  while (!msg_out.eof()) {
    msg_out.getline(buf, 20);
    SDL_Surface* smsg = TTF_RenderText_Blended(Sans, buf, {COLORP4F2U(&white)});
    SDL_Texture* txtr = SDL_CreateTextureFromSurface(renderer, smsg);
    SDL_QueryTexture(txtr, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { curW, curH, texW, texH };
    SDL_RenderCopy(renderer, txtr, NULL, &dstrect);
    SDL_DestroyTexture(txtr);
    SDL_FreeSurface(smsg);
    curH += texH;
  }
  SDL_RenderPresent(renderer);

  LastFrame = now;
  // } text render 
}

void testSDLW::_setup() {
  Sans = TTF_OpenFont(PROJ_DIR "/fonts/FreeSans.ttf", 12);
  LastFrame = clock();
  glsl_program();
  glsl_data();
}

int main() {
  testSDLW().
    init().
    loop();

  return 0;
}

