#ifndef DEBUG_H
#define DEBUG_H

#include <GL/glew.h>

void message_callback(GLenum source, GLenum type, 
    GLuint id, GLenum severity, GLsizei length, 
    GLchar const* message, void const* user_param);
void debugenable();

#endif
