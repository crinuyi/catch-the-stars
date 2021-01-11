#version 150 core

in vec3 ourPosition;
in vec2 inoutUV;
in vec3 inoutNormal;

out vec4 outColor;

uniform sampler2D tex0;

// Parametry oswietlenia
uniform vec3 light_ambient;
uniform vec3 light_diffuse;
uniform vec3 light_specular;
uniform vec3 light_position;
uniform vec3 camera_position;

// ---------------------------------------------------------------------------
vec3 Calculate_PointLight(vec3 fragPos, vec3 fragNormal)
{
	// ---------------------------
	// Ambient
	vec3 ambientPart = light_ambient;


	// ---------------------------
	// Diffuse

	// Obliczenie wektora (swiatlo - wierzcholek)
	// czyli kierunku padania swiatla na wierzcholek
	vec3 lightDirection = normalize(light_position - vec3(fragPos));

	// obliczenie kata pomiedzy wektorem lightDir oraz wektorem normalnym
	// wartosc kata okresla pod jakim katem padaja promienie
	float lightCoeff = max(dot(fragNormal, lightDirection), 0.0);

	vec3 diffusePart = lightCoeff * light_diffuse;


	// ------------------
	// Specular
	vec3 viewDir = normalize(camera_position - vec3(fragPos));
	vec3  reflectDir = reflect(-lightDirection, fragNormal);
	// obliczanie wspolczynnika specular z parametrem shininess
	float specularCoeff = pow(max(dot(viewDir, reflectDir), 0.0), 128.0f);
	vec3  specularPart = specularCoeff * light_specular;

	// -----------------
	// Ostateczny
	return (ambientPart + diffusePart + specularPart);
}


void main()
{

	// ustawienie domyslnego koloru fragmentu
	// mozna tutaj uwzglednic tekstury i inne parametry
	vec4 objectColor = texture(tex0, inoutUV);


	// Zastosowanie oswietlenia do fragmentu
	vec4 result = vec4(Calculate_PointLight(ourPosition, inoutNormal), 1.0f) * objectColor;

	outColor = result;

}
