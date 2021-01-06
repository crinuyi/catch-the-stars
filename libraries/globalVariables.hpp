#include <vector>

class Model;
bool checkIfNoCollisions(glm::vec3 position);
void checkForStarsToCollect(glm::vec3 position);
class Program;
class Ground;

std::vector<Model> models;
Ground* ground;

glm::mat4 matProj;
glm::mat4 matView;
glm::mat4 matModel;
glm::mat4 matProjView;
glm::mat3 matNormal;

int frameCounter;
int starCounter;

const char* files[6] = {
			"textures/skybox/posx.jpg",
			"textures/skybox/negx.jpg",
			"textures/skybox/posy.jpg",
			"textures/skybox/negy.jpg",
			"textures/skybox/posz.jpg",
			"textures/skybox/negz.jpg"
};