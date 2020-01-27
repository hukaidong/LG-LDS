#pragma once

#include <cmath>
#include <iostream>
#include "showdl.hpp"

#define PI 3.1415926
#define GLfloatUnitDegree(x) (GLfloat)cos(PI*x/180.0), (GLfloat)sin(PI*x/180.0)

namespace Primitives {
  using namespace Eigen;

  void DrawVertexArrayObject(GLint vao) {
    GLint esize;
    glBindVertexArray(vao);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &esize);
    glDrawElements(GL_TRIANGLES, esize/sizeof(GLuint), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  GLuint InitVertexArrayObject(GLint vaidx, 
      const void *vdata, size_t sizeof_vdata, 
      const void *edata, size_t sizeof_edata) {

    GLuint vao, vbuf, vbd=0,
                ebuf, ebd=0;

    glCreateBuffers(1, &vbuf);
    glCreateBuffers(1, &ebuf);
    glCreateVertexArrays(1, &vao);

    glNamedBufferStorage(vbuf, sizeof_vdata, vdata, 0);
    glNamedBufferStorage(ebuf, sizeof_edata, edata, 0);

    glEnableVertexArrayAttrib(vao, vaidx);
    glVertexArrayAttribFormat(vao, vaidx, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, vaidx, vbd);
    glVertexArrayVertexBuffer(vao, vbd, vbuf, 0, sizeof(GLfloat)*3);
    glVertexArrayElementBuffer(vao, ebuf);

    return vao;
  }

  GLuint DumpTriangles(GLint vaidx) {

    GLuint idx[] = {0, 1, 2};
    Matrix<GLfloat, 3, 3, RowMajor> pos; pos << 0, 0, 0, 1, 0, 0, GLfloatUnitDegree(60), 0;
    pos.rowwise() -= pos.colwise().mean();

    return (InitVertexArrayObject(vaidx, pos.data(), sizeof(GLfloat)*pos.size(), idx, sizeof(idx)));
  }

}
