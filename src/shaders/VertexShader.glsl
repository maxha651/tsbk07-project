#version 330 core
 
in vec3 in_Position;
in  vec3 uni_Color;
out  vec3 ex_Color;

uniform mat4 transform;

void main(void)
{
   gl_Position = transform * vec4(in_Position, 1.0);
   ex_Color = uni_Color;
}
