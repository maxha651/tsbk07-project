#version 330 core

layout(location = 0) out vec4 out_Color;

in vec3 ex_Normal;

uniform vec4 uni_Color;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	const float specularIntensity = 0.1;
	const float specularPower = 32;

	float shade;

	shade = clamp(dot(normalize(ex_Normal), light), 0.0, 1.0);

	out_Color = vec4(1.0);//vec3(uni_Color.x * shade, uni_Color.y * shade, uni_Color.z * shade);
}