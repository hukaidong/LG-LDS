#version 400 core
in vec2 pcoord;
out vec4 FragColor;

void main() {
  FragColor = vec4(
      pcoord, 
      max(0, dot(pcoord-vec2(0.2, 0.2), vec2(-0.7, -0.7))),
      1);
}
