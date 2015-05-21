#version 330 core

layout(location = 0) out vec4 out_Color;

in vec3 ex_Normal;
in vec2 ex_TexCoord;

uniform sampler2D texUnit;

uniform mat4 transform;
uniform mat4 projectionMatrix;
uniform mat4 worldToViewMatrix;

void main(void)
{
	out_Color = texture(texUnit, ex_TexCoord);
}
