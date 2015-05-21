#version 330 core

in vec3 in_Position;
in vec3 in_Normal;
in vec4 in_Color;
in vec2 in_TexCoord;

out vec3 ex_Normal;
out vec3 ex_Surface;
out vec4 ex_Color;
out vec2 ex_TexCoord;

uniform mat4 transform;
uniform mat4 projectionMatrix;
uniform mat4 worldToViewMatrix;

void main(void)
{
	mat4 totalMatrix = projectionMatrix * worldToViewMatrix;
    gl_Position = totalMatrix * vec4(in_Position, 1.0);
    ex_Normal =  in_Normal;
	ex_Surface = mat3(worldToViewMatrix) * in_Position;
	ex_Color = in_Color;
	ex_TexCoord = in_TexCoord;
}
