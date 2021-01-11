#version 150

in vec4 inoutPos;
out vec4 outColor;

uniform samplerCube tex0;

void main()
{
	outColor = texture( tex0, inoutPos.xyz );
}
