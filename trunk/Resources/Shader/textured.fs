
#version 140

uniform sampler2D uni_DiffuseTex;
uniform vec4 uni_colorId;

smooth in vec2 var_TexCoord;

out vec4 out_Color0;
out vec4 out_Color1;

void main(void)
{	
	out_Color0 = texture(uni_DiffuseTex, var_TexCoord);
	out_Color1 = uni_colorId;
}
