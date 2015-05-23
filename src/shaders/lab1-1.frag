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
	const float specularIntensity = 0.1;
	const float specularPower = 32;

	float shade;

	shade = clamp(dot(normalize(ex_Normal), light), 0.0, 1.0);

	out_Color = vec4(uni_Color.x*ex_Energy.x, uni_Color.y*ex_Energy.y, uni_Color.z*ex_Energy.z, uni_Color.w);
}
