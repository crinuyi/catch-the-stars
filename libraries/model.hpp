#include "texture.hpp"

class Model {
    public:
        bool active;
        GLuint idProgram;
        GLuint idVAO;
        GLuint idVBO_vertices;
        GLuint idVBO_uvs;
        GLuint idVBO_normals;
        GLfloat x, y, z;
        GLfloat rotation_x, rotation_y, rotation_z;
        GLfloat scale;
        bool isCollisional;
        GLfloat collision_radius;
        bool isCollectible;
        int vertex_count;
        Program* program;
        Texture* texture;

        bool isCollision(glm::vec3 position);
        void collectStar(glm::vec3 position);
    
        Model(
            GLfloat x, 
            GLfloat y, 
            GLfloat z,
            GLfloat rotation_x,
            GLfloat rotation_y,
            GLfloat rotation_z,
            GLfloat scale,
            const char* path,
            Program* program,
            Texture* texture, 
            bool isCollisional,
            bool isCollectible
        ); //konstruktor


};

Model::Model(GLfloat x, GLfloat y, GLfloat z, 
             GLfloat rotation_x, GLfloat rotation_y, GLfloat rotation_z,
             GLfloat scale,
             const char* path, Program* program, Texture* texture, 
             bool isCollisional, bool isCollectible) {
    active = true;
    this->x = x;
    this->y = y;
    this->z = z;
    this->rotation_x = rotation_x;
    this->rotation_y = rotation_y;
    this->rotation_z = rotation_z;
    this->scale = scale;
    this->program = program;
    this->texture = texture;
    this->isCollisional = isCollisional;
    this->isCollectible = isCollectible;
    collision_radius = 1.0f;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    loadOBJ(path, vertices, uvs, normals);
    this->vertex_count = vertices.size();

    glGenVertexArrays(1, &idVAO);
	glBindVertexArray(idVAO);

	glGenBuffers(1, &idVBO_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, idVBO_vertices);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );

	glGenBuffers(1, &idVBO_uvs);
	glBindBuffer(GL_ARRAY_BUFFER, idVBO_uvs);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 1 );

    glGenBuffers(1, &idVBO_normals);
	glBindBuffer(GL_ARRAY_BUFFER, idVBO_normals);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 2 );

	glBindVertexArray( 0 );
};

bool Model::isCollision(glm::vec3 position) {
    float distance = glm::distance(
                        glm::vec3(this->x, this->y, this->z), 
                        position);
    if(this->isCollisional)
        if (distance < this->collision_radius + cameraCollisionRadius)
            return true;

    return false;
}

void Model::collectStar(glm::vec3 position) {
        float distance = glm::distance(
                            glm::vec3(this->x, this->y, this->z), 
                            position);
        if(distance < this->collision_radius + cameraCollisionRadius) {
            if(this->active)
                starCounter++;
            this->active = false;
        }
}

void checkForStarsToCollect(glm::vec3 position) {
    int i = 0;
    for(std::vector <Model>::iterator it = models.begin(); it != models.end(); it++, i++)
        if(models[i].isCollectible)
                models[i].collectStar(position);
}

bool checkIfNoCollisions(glm::vec3 position) {
    bool result = true;
    for(Model m : models) {
        if(m.isCollision(position))
            result = false;
    }
    return result;
}

void DrawModel(Model model) {
	glBindVertexArray(model.idVAO);
	glUseProgram(model.program->id);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(model.texture->target, model.texture->id);

	// Macierz widoku
	matView = getViewMatrix();
	matModel = glm::mat4x4(1.0);

	matModel = glm::translate( matModel, glm::vec3(model.x, model.y, model.z));
	matModel = glm::rotate( matModel, model.rotation_x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
	matModel = glm::rotate( matModel, model.rotation_y, glm::vec3( 0.0f, 1.0f, 0.0f ) );
	matModel = glm::rotate( matModel, model.rotation_z, glm::vec3( 0.0f, 0.0f, 1.0f ) );
	matModel = glm::scale(matModel, glm::vec3(model.scale));
	
	// Obliczanie macierzy rzutowania
	matProjView = matProj * matView * matModel;
	matNormal = glm::transpose(glm::inverse(matModel));

	//obliczanie pozycji kamery
	model.program->camera_position = ExtractCameraPos(matView*matModel);

	// Przekazanie macierzy rzutowania
	glUniformMatrix4fv( glGetUniformLocation(model.program->id, "matProjView"), 1, GL_FALSE,  glm::value_ptr(matProjView));
	glUniformMatrix3fv( glGetUniformLocation(model.program->id, "matNormal"), 1, GL_FALSE,  glm::value_ptr(matNormal));
	
	glUniform1i(glGetUniformLocation(model.program->id, "tex0"), 0);

	glUniform3fv( glGetUniformLocation(model.program->id, "light_ambient"), 1, glm::value_ptr(model.program->light_ambient)); //value_ptr zawsze podaje 1 element w przypadku tablicy
	glUniform3fv( glGetUniformLocation(model.program->id, "light_diffuse"), 1, glm::value_ptr(model.program->light_diffuse));
	glUniform3fv( glGetUniformLocation(model.program->id, "light_specular"), 1, glm::value_ptr(model.program->light_specular));
	glUniform3fv( glGetUniformLocation(model.program->id, "light_position"), 1, glm::value_ptr(model.program->light_position));
	glUniform3fv( glGetUniformLocation(model.program->id, "camera_position"), 1, glm::value_ptr(model.program->camera_position));

	glDrawArrays(GL_TRIANGLES, 0, model.vertex_count);
};