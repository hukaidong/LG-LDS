#include <ctime>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits.h>
#include "showdl.hpp"
#include "primitives/primitives.hpp"
#include "Eigen/Dense"
#include "SDL.h"
#include "SDL_ttf.h"

#define deg2rad (M_PI / 180.0f)

using namespace Eigen;
typedef std::vector<Vector3f> FrameSensorPoints;
typedef std::vector<FrameSensorPoints> SceneSensorPoints;



FrameSensorPoints prepareFrame(const std::string& line) {
  FrameSensorPoints pts;
  std::istringstream s(line);
  float x, y, z, zmax=-MAXFLOAT, zmin=MAXFLOAT;
  Vector3f point, mean=Vector3f::Zero();
  s >> x;                                     // discard index
  while (s.good()) {
    s.seekg(1, s.cur); s >> x; x = x/1000.0;
    s.seekg(1, s.cur); s >> y; y = y/1000.0;
    s.seekg(1, s.cur); s >> z; z = z/1000.0;
    zmax = z > zmax? z: zmax;
    zmin = z < zmin? z: zmin;
    point = Vector3f(x, z, y);
    mean += point;
    pts.push_back(point);
  }
  mean /= pts.size();
  for (auto &pt: pts) { pt-=mean; pt*=5; }
  return pts;
}

SceneSensorPoints prepareSceneFromFile(const char* path) {
  std::cout << "From file:\n" << path << std::endl;
  std::ifstream f(path, std::ios::in);

  if (f.fail()) { 
    std::cerr << "Fail to read file" << std::endl; exit(3);
  }

  std::string line; 
  SceneSensorPoints scene;
  while (f.good()) {
    std::getline(f, line);
    scene.push_back(prepareFrame(line));
  }
  return scene;
}


class MainShow: public sdlWindow {
  GLuint trag_vao, cube_vao;
  float lastclock=0, fpsmean=0;
public:
  SceneSensorPoints scene;
  MainShow& setup() {
    auto fshader = GetBytesFromFile(PROJ_DIR "/shdr/test.f.glsl");
    auto vshader = GetBytesFromFile(PROJ_DIR "/shdr/test.v.glsl");
    _use_shaders(vshader.get(), fshader.get());

    GLuint vidx = glGetAttribLocation(_program, "vertex");
    trag_vao = Primitives::DumpTriangles(vidx);
    cube_vao = Primitives::DumpCube(vidx);
    lastclock = (float)clock() / CLOCKS_PER_SEC;

    return *this;
  }

protected: 
  int lastframe = -1;
  virtual void _render() {
    glUseProgram(_program);
    glClearColor(0, 0, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    GLuint mvp = glGetUniformLocation(_program, "mvp");
    _view_mvp_assign(mvp);

    glBindVertexArray(cube_vao);
    GLuint anim = glGetUniformLocation(_program, "anim");
    float angle = SDL_GetTicks() * 45.0 / 1000.0 * deg2rad;
    int frame = (SDL_GetTicks() * 200 / 1000) % scene.size();
    //if (frame < lastframe) _event_should_close = true;
    lastframe = frame;
    for (auto &pt: scene.at(frame)) {
      Transform t = AngleAxis(angle, vec3::UnitY())
        * Translation<float, 3>(pt)
        * Scaling<GLfloat>(0.1f, 0.1f, 0.1f) 
        ;
      glUniformMatrix4fv(anim, 1, GL_FALSE, t.matrix().data());
      Primitives::DrawVertexArrayMesh(cube_vao);
    }
    glBindVertexArray(0);
    glUseProgram(0);

    SDL_Color white = {0xFF, 0xFF, 0xFF};
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect DestRect{.x=10, .y=10, .w=10, .h=10};
    int text_w, text_h;
    float now = (float) clock() / CLOCKS_PER_SEC;
    float flipfps = 1.0/(now-lastclock);
    float alpha = 0.1;
    std::ostringstream report;
    report.width(8);
    report.precision(2);
    report.setf(std::ios::fixed);
    fpsmean = alpha * flipfps + (1-alpha) * fpsmean;
    report << "FPS: " << fpsmean;
    text_surface = TTF_RenderText_Blended(font, report.str().c_str(), white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_QueryTexture(text_texture, NULL, NULL, &text_w, &text_h);
    DestRect.w = text_w;
    DestRect.h = text_h;
    SDL_RenderCopy(renderer, text_texture, NULL, &DestRect);
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);

    report.str("");
    report.clear();
    report << "CurrentFrame: " << frame;
    text_surface = TTF_RenderText_Blended(font, report.str().c_str(), white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_QueryTexture(text_texture, NULL, NULL, &text_w, &text_h);
    DestRect.y += DestRect.h;
    DestRect.w = text_w;
    DestRect.h = text_h;
    SDL_RenderCopy(renderer, text_texture, NULL, &DestRect);
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);

    SDL_GL_SwapWindow(window);
    SDL_RenderPresent(renderer);
    lastclock = now;
  }


};

int main(int argc, char *argv[])
{
  char path[1024]="";

  if (argc==1 || !realpath(argv[1], path)) {
    std::cerr << "No file given or not a path" << std::endl; exit(1);
  }

  MainShow show;
  show.scene = prepareSceneFromFile(path);
  show.init();
  assign_callback();
  show.setup();
  show.loop();

  return 0;
}
