#pragma once

#include "./common.hpp"

namespace Primitives {
  using namespace Eigen;

  GLuint DumpTriangles(GLint vaidx) {
    GLuint idx[] = {0, 1, 2};
    Matrix<GLfloat, 3, 3, RowMajor> pos; pos << 0, 0, 0, 1, 0, 0, GLfloatUnitDegree(60), 0;
    pos.rowwise() -= pos.colwise().mean();

    return (InitVertexArrayObject(vaidx, pos.data(), sizeof(GLfloat)*pos.size(), idx, sizeof(idx)));
  }
}
