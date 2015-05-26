#version 330 core

out  vec4 out_Color;

in vec3 ex_Normal;
in vec3 ex_Surface;
in vec4 ex_Energy;

uniform vec4 uni_Color;

uniform mat4 totMatrix;
uniform mat4 projectionMatrix;
uniform mat4 worldToViewMatrix;

in vec3 light_Dir;

void main(void)
{
	const vec3 light = normalize(vec3(0.55, 0.55, -1.0));

	float shade;

	

	shade = clamp(dot(normalize(ex_Normal), light_Dir), 0.0, 1.0);
	float intensity = length(vec3(ex_Energy));
	vec4 tmp_Energy = clamp(ex_Energy, 0.0, 1.0);

	vec4 illu_Color = uni_Color*shade;

	out_Color = tmp_Energy;
}
