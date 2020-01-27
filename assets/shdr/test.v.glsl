#version 400 core 

in vec3 vertex;
out vec2 pcoord;

void main() {
  vec2 pos = vertex.xy;
  gl_Position = vec4(pos, 0.0, 1.0);
  pcoord = pos + vec2(0, 0);

}
