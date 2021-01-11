class Program {
    public:
        GLuint id;
        glm::vec3 light_ambient;
        glm::vec3 light_diffuse;
        glm::vec3 light_specular;
        glm::vec3 light_position;
        glm::vec3 camera_position;

        Program(
            const char* vertex_shader, 
            const char* fragment_shader
        );
};

Program::Program(const char* vertex_shader, const char* fragment_shader) {
    id = glCreateProgram();

	glAttachShader(id, LoadShader(GL_VERTEX_SHADER, vertex_shader));
	glAttachShader(id, LoadShader(GL_FRAGMENT_SHADER, fragment_shader));

	LinkAndValidateProgram(id);

    this->light_ambient = glm::vec3(0.6, 0.6, 0.6);
    this->light_diffuse = glm::vec3(0.97, 1.0, 0.84);
    this->light_specular = glm::vec3(0.05, 0.05, 0.05);
    this->light_position = glm::vec3(5.0, 2.0, 5.0);
};