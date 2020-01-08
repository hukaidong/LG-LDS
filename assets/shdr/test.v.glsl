#version 400 core 

uniform mat4 mvp;
in vec3 cube;
out vec3 vpos;

void main() {
  vpos = cube;
  gl_Position = mvp * vec4(cube, 1.0); 
}
