#version 330 core

in vec3 in_Position;
in vec3 in_Normal;

out vec3 ex_Normal;

uniform mat4 transform;
uniform mat4 projectionMatrix;
uniform mat4 worldToViewMatrix;

void main(void)
{
	mat4 totalMatrix = projectionMatrix * worldToViewMatrix * transform;
  gl_Position = totalMatrix * vec4(in_Position, 1.0);
  ex_Normal = mat3(transform) * in_Normal;
}