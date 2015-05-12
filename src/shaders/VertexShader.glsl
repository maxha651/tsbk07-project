#version 330 core
 
in vec3 in_Position;
in vec3 in_Normal;

out vec3 ex_Normal;
out vec3 ex_Surface; 

uniform mat4 totMatrix;
uniform mat4 projectionMatrix;
uniform mat4 worldToViewMatrix;

void main(void)
{
    gl_Position = totMatrix * vec4(in_Position, 1.0);
    ex_Normal = vec3(worldToViewMatrix * vec4(in_Normal, 1.0));
	ex_Surface = vec3(worldToViewMatrix * vec4(in_Position, 1.0));
}
