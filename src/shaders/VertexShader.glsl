#version 330 core
 
in vec3 in_Position;



uniform mat4 transform;

void main(void)
{
   gl_Position = transform * vec4(in_Position, 1.0);
  
}
