#version 400 core 

uniform mat4 mvp;
in vec3 cube;
in vec3 color;
out vec3 vpos;

void main() {
  vpos = color;
  gl_Position = mvp * vec4(cube, 1.0); 
}
