#include "showdl.hpp"
#include "primitives/primitives.hpp"


class MainShow: public sdlWindow {
  GLuint vao;
public:
  MainShow& setup() {
    auto fshader = GetBytesFromFile(PROJ_DIR "/shdr/test.f.glsl");
    auto vshader = GetBytesFromFile(PROJ_DIR "/shdr/test.v.glsl");
    _use_shaders(vshader.get(), fshader.get());

    GLuint vidx = glGetAttribLocation(_program, "vertex");
    vao = Primitives::DumpTriangles(vidx);
    // vao = Primitives::DumpCube(vidx);

    return *this;
  }

protected: 
  virtual void _render() {
    glUseProgram(_program);
    glClearColor(0, 0, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vao);

    GLuint mvp = glGetUniformLocation(_program, "mvp");
    _view_mvp_assign(mvp);

    Primitives::DrawVertexArrayObject(vao);

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
