#version 330 core

out  vec4 out_Color;

uniform vec4 uni_Color;

void main(void)
{
  out_Color = vec4(uni_Color);
}
