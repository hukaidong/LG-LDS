#version 430 core 

layout (location = 1) uniform mat4 mvp; 
layout (location = 2) uniform mat4 anim; 
layout (location = 0) in vec3 vertex;
out vec2 pcoord;

void main() {
  gl_Position = mvp * anim * vec4(vertex, 1.0);
  pcoord = vertex.xy + vec2(0, 0);

}
