#version 330 core

out  vec4 out_Color;

in vec3 ex_Normal;
in vec3 ex_Surface;
in vec4 ex_Energy;

uniform vec4 uni_Color;

uniform mat4 totMatrix;
uniform mat4 projectionMatrix;
uniform mat4 worldToViewMatrix;

void main(void)
{
	const vec3 light = vec3(0.8, 0.58, 0.58);

	float shade;

	shade = clamp(dot(normalize(ex_Normal), light), 0.0, 1.0);
	float intensity = length(vec3(ex_Energy));
	vec4 tmp_Energy = clamp(ex_Energy, 0.0, 1.0);

	out_Color = vec4(1 - tmp_Energy.x, 1 - tmp_Energy.y, 1 - tmp_Energy.z, 1);
}