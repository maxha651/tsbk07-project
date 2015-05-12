#version 330 core
 
in vec3 in_Position;
in vec3 in_Normal;

out vec3 ex_Normal;
out vec3 ex_Surface; 

uniform mat4 transform;
uniform mat4 projectionMatrix;
uniform mat4 worldToViewMatrix;

void main(void)
{
    gl_Position = projectionMatrix * worldToViewMatrix * transform * vec4(in_Position, 1.0);
    ex_Normal = mat3(transform) * in_Normal;
	ex_Surface = vec3(worldToViewMatrix * vec4(in_Position, 1.0));
}
