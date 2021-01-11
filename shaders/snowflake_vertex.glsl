#version 330

// Wspolrzedne wierzcholkow
layout( location = 0 ) in vec4 inPosition;
layout (location = 1 ) in vec2 inUV;
layout( location = 2 ) in vec3 inNormal;
// macierz modelu dla calej instancji
layout( location = 3 ) in mat4 matModelInst;


// Macierz rzutowania
uniform mat4 matProjView;


out vec4 inoutPos;
out vec2 inoutUV;

void main()
{
	gl_Position = matProjView * matModelInst * inPosition;
	inoutPos = inPosition;
	inoutUV = inUV;
}
