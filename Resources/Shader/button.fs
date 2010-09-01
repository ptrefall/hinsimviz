
#version 140

uniform vec4 uni_colorId;
uniform vec4 uni_color;

out vec4 out_Color0;
out vec4 out_Color1;

void main(void)
{	
	out_Color0 = uni_color;
	out_Color1 = uni_colorId;
}
