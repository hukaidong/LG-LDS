#version 400 core

in vec4 vpos;
out vec4 gl_FragColor;

void main() {
  vec4 col = vpos;
  //gl_FragColor = vec4(col.xy, distance(vec2(1, 1), col.xy), 0.8);
  gl_FragColor = vec4(col.xyz, 0.8);
}
