#include <iostream>

#include "carefulgl.hpp"

namespace carefully {

#ifdef DEBUG_GL

void glCompileShader(GLuint shader) {
  GLint success, infol;
  char log[1024];

  ::glCompileShader(shader);
  ::glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success) return;

  ::glGetShaderInfoLog(shader, 1024, &infol, log);
  log[infol] = 0;
  std::cerr << log << std::endl;
}

#else
void glCompileShader(GLuint shader) { glCompileShader(shader); }
#endif

}
