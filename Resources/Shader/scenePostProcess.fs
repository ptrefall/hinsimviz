
#version 140

uniform vec4 uni_colorId;

uniform sampler2D uni_Color0Tex;
uniform sampler2D uni_Color1Tex;

smooth in vec2 ex_TexCoord;

out vec4 out_Color0;

void main(void)
{	
	out_Color0 = texture2D(uni_Color0Tex, ex_TexCoord.st);
	//out_Color0 = vec4(0.0, 1.0, 0.0, 1.0);
}
