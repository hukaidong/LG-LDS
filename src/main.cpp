#include <cmath>
#include "showdl.hpp"
#include "primitives/primitives.hpp"
#include "Eigen/Dense"

#define deg2rad (M_PI / 180.0f)


class MainShow: public sdlWindow {
  GLuint trag_vao, cube_vao;
public:
  MainShow& setup() {
    auto fshader = GetBytesFromFile(PROJ_DIR "/shdr/test.f.glsl");
    auto vshader = GetBytesFromFile(PROJ_DIR "/shdr/test.v.glsl");
    _use_shaders(vshader.get(), fshader.get());

    GLuint vidx = glGetAttribLocation(_program, "vertex");
    trag_vao = Primitives::DumpTriangles(vidx);
    cube_vao = Primitives::DumpCube(vidx);

    return *this;
  }

protected: 
  virtual void _render() {
    glUseProgram(_program);
    glClearColor(0, 0, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    GLuint mvp = glGetUniformLocation(_program, "mvp");
    _view_mvp_assign(mvp);

    glBindVertexArray(cube_vao);
    GLuint anim = glGetUniformLocation(_program, "anim");
    float angle = SDL_GetTicks() / 1000.0 * 45.0 * deg2rad;
    for (int x=-3; x<4; x++) 
    for (int y=-3; y<4; y++)
    for (int z=-3; z<4; z++){ 
      Transform t = AngleAxis(angle, vec3::UnitY())
        * Translation<GLfloat, 3>(x, y, z) 
        * Scaling<GLfloat>(0.1f, 0.1f, 0.1f);
      glUniformMatrix4fv(anim, 1, GL_FALSE, t.matrix().data());
      //Primitives::DrawVertexArrayFrame(cube_vao);
      Primitives::DrawVertexArrayMesh(cube_vao);
    }

    glBindVertexArray(0);
    glUseProgram(0);
  }


};

int main(int argc, char *argv[])
{
  MainShow show;
  show.init();
  assign_callback();
  show.setup();
  show.loop();
  return 0;
}
