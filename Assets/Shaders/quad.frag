#version 330 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;
layout (location = 3) in float v_TexIndex;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;
uniform sampler2D texture7;
uniform sampler2D texture8;
uniform sampler2D texture9;
uniform sampler2D texture10;
uniform sampler2D texture11;
uniform sampler2D texture12;
uniform sampler2D texture13;
uniform sampler2D texture14;
uniform sampler2D texture15;
uniform sampler2D texture16;
uniform sampler2D texture17;
uniform sampler2D texture18;
uniform sampler2D texture19;
uniform sampler2D texture20;
uniform sampler2D texture21;
uniform sampler2D texture22;
uniform sampler2D texture23;
uniform sampler2D texture24;
uniform sampler2D texture25;
uniform sampler2D texture26;
uniform sampler2D texture27;
uniform sampler2D texture28;
uniform sampler2D texture29;
uniform sampler2D texture30;
uniform sampler2D texture31;

void main()
{
	vec4 texColor = Input.Color;

	switch(int(v_TexIndex))
	{
		case  0: texColor *= texture( texture0, Input.TexCoord); break;
		case  1: texColor *= texture( texture1, Input.TexCoord); break;
		case  2: texColor *= texture( texture2, Input.TexCoord); break;
		case  3: texColor *= texture( texture3, Input.TexCoord); break;
		case  4: texColor *= texture( texture4, Input.TexCoord); break;
		case  5: texColor *= texture( texture5, Input.TexCoord); break;
		case  6: texColor *= texture( texture6, Input.TexCoord); break;
		case  7: texColor *= texture( texture7, Input.TexCoord); break;
		case  8: texColor *= texture( texture8, Input.TexCoord); break;
		case  9: texColor *= texture( texture9, Input.TexCoord); break;
		case 10: texColor *= texture(texture10, Input.TexCoord); break;
		case 11: texColor *= texture(texture11, Input.TexCoord); break;
		case 12: texColor *= texture(texture12, Input.TexCoord); break;
		case 13: texColor *= texture(texture13, Input.TexCoord); break;
		case 14: texColor *= texture(texture14, Input.TexCoord); break;
		case 15: texColor *= texture(texture15, Input.TexCoord); break;
		case 16: texColor *= texture(texture16, Input.TexCoord); break;
		case 17: texColor *= texture(texture17, Input.TexCoord); break;
		case 18: texColor *= texture(texture18, Input.TexCoord); break;
		case 19: texColor *= texture(texture19, Input.TexCoord); break;
		case 20: texColor *= texture(texture20, Input.TexCoord); break;
		case 21: texColor *= texture(texture21, Input.TexCoord); break;
		case 22: texColor *= texture(texture22, Input.TexCoord); break;
		case 23: texColor *= texture(texture23, Input.TexCoord); break;
		case 24: texColor *= texture(texture24, Input.TexCoord); break;
		case 25: texColor *= texture(texture25, Input.TexCoord); break;
		case 26: texColor *= texture(texture26, Input.TexCoord); break;
		case 27: texColor *= texture(texture27, Input.TexCoord); break;
		case 28: texColor *= texture(texture28, Input.TexCoord); break;
		case 29: texColor *= texture(texture29, Input.TexCoord); break;
		case 30: texColor *= texture(texture30, Input.TexCoord); break;
		case 31: texColor *= texture(texture31, Input.TexCoord); break;
	}

	if (texColor.a == 0.0)
		discard;

	o_Color = texColor;
}