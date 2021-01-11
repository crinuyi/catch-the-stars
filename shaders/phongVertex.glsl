#version 330 core

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

out vec3 ourPosition;
out vec2 inoutUV;
out vec3 inoutNormal;

uniform mat4 matProjView;
uniform mat3 matNormal;
uniform mat4 matModel;

void main()
{
	gl_Position = matProjView * inPosition;

	// W Modelu Phonga, oswietlenie obiektu obliczane
	// jest na poziomie fragmentu, a nie wierzcholka
	// jak w modelu Gourauda. Wysylamy wiec wszystkie
	// dane wierzcholka jak pozycje i wektor normalny
	// do shadera fragmentow

	 ourPosition = vec3(matModel * inPosition);
	inoutUV = inUV;
	inoutNormal = normalize(matNormal * inNormal);

}
