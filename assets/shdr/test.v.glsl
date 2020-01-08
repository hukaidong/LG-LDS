#version 400 core 

uniform mat4 mvp;
in vec3 cube;
in vec3 color;
uniform vec3 position;
uniform mat4 rotation;
out vec3 vpos;

void main() {
  vpos = color;
  gl_Position = mvp * (rotation * vec4(cube, 1.0) + vec4(position, 1.0));
}
