#version 400 core 

uniform float scale;

in vec2 vertex;

out vec2 pcoord;

void main() {
  vec2 pos = vertex;
  gl_Position = vec4(pos, 0.0, 1.0);
  pcoord = pos + vec2(0, 0);

}
