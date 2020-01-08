#version 400 core 

uniform mat4 mvp;
in vec3 cube;
out vec4 vpos;
out vec4 gl_Position;

void main() {
  vpos = vec4(cube, 1);
  vec4 pos = mvp * vec4(cube, 1.0); 
  //gl_Position = vec4(pos.xyz/pos.w, 1.0);
  gl_Position = pos;
}
