#version 330

// Wspolrzedne wierzcholkow
layout( location = 0 ) in vec4 inPosition;
layout (location = 1 ) in vec2 inUV;

// Macierz rzutowania
uniform mat4 matProjView;


out vec4 inoutPos;
out vec2 inoutUV;

void main()
{
	gl_Position = matProjView * inPosition;
	inoutPos = inPosition + 2.5f;
	inoutUV = inUV;
}
