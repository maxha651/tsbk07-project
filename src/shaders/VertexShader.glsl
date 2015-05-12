#version 330 core
 
in vec3 in_Position;
in vec3 in_Normal;

out vec3 ex_Normal;

uniform mat4 transform;

void main(void)
{
   gl_Position = transform * vec4(in_Position, 1.0);
   ex_Normal = vec3(transform * vec4(in_Normal, 1.0));
}
