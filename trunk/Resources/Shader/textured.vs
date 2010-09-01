
#version 140

uniform struct Transform
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 modelMatrix;
} Transform1;

in vec3 in_Position;
in vec2 in_TexCoord;

smooth out vec2 var_TexCoord;

void main(void)
{	
	gl_Position = Transform1.projectionMatrix * Transform1.viewMatrix * Transform1.modelMatrix * vec4(in_Position, 1.0);
	var_TexCoord = in_TexCoord;
}
