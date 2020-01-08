#ifndef FILEIO_H
#define FILEIO_H

#include <memory>
#include <vector>
#include "GL/glew.h"

std::unique_ptr<char[]> fromfile(const char* D);
std::pair<std::vector<GLfloat>, std::vector<GLuint>> loadObj(const char* fname);

#endif
