#version 400 core

in vec4 vpos;
out vec4 gl_FragColor;

void main() {
  vec4 col = vpos;
  col.z = 1.0-vpos.z/2;
  col.w = 0.5;
  gl_FragColor = col;
}
