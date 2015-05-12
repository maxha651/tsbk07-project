#version 330 core

out  vec4 out_Color;

in vec3 ex_Normal;

uniform vec4 uni_Color;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;

	shade = dot(normalize(ex_Normal), light);

	out_Color = vec4(uni_Color.x * shade, uni_Color.y * shade, uni_Color.z * shade, 1.0);
}
