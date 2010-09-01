
#version 140

uniform struct Transform
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 modelMatrix;
} Transform1;

in vec3 in_Position;
in vec2 in_TexCoord;

smooth out vec2 ex_TexCoord;

void main()
{
	vec2 pos = sign(in_Position.xy);
	gl_Position = vec4(pos, 0.0, 1.0);
	ex_TexCoord = pos * 0.5 + 0.5;
}
