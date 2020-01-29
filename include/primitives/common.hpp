#pragma once

#include <cmath>
#include <iostream>
#include "GL/glew.h"
#include "Eigen/Dense"

#define PI 3.1415926
#define GLfloatUnitDegree(x) (GLfloat)cos(PI*x/180.0), (GLfloat)sin(PI*x/180.0)

namespace Primitives {
  void DrawVertexArrayMesh(GLint vao) {
    GLint esize;
    glBindVertexArray(vao);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &esize);
    glDrawElements(GL_TRIANGLES, esize/sizeof(GLuint), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  void DrawVertexArrayFrame(GLint vao) {
    GLint esize;
    glBindVertexArray(vao);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &esize);
    glDrawElements(GL_LINES, esize/sizeof(GLuint), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  GLuint InitVertexArrayObject(GLint vaidx, 
      const GLfloat *vdata, size_t sizeof_vdata, 
      const GLuint  *edata, size_t sizeof_edata) {

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
}
