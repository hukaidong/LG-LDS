#pragma once

#include "./common.hpp"

namespace Primitives {
  GLuint DumpCube(GLint vaidx) {
    GLfloat cube_vertices[] = {
    // front
    -1.0, -1.0,  1.0,
      1.0, -1.0,  1.0,
      1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // back
    -1.0, -1.0, -1.0,
      1.0, -1.0, -1.0,
      1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0
    };

    GLuint cube_elements[] = {
    // front
    0, 1, 2,
    2, 3, 0,
    // right
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // left
    4, 0, 3,
    3, 7, 4,
    // bottom
    4, 5, 1,
    1, 0, 4,
    // top
    3, 2, 6,
    6, 7, 3
    };

    return (InitVertexArrayObject(vaidx, cube_vertices, sizeof(cube_vertices), cube_elements, sizeof(cube_elements)));
  }
}
