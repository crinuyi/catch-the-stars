const static int numberOfConfetti = 1000;

class ConfettiModel {
    public:
        float minXZ = -15.0f;
		float maxXZ = 15.0f;
        float maxY = 7.0f;
        glm::vec3 velocity = glm::vec3(0.00f, -0.005f, 0.0f);
        glm::vec3 rotationVelocity = glm::vec3(0.0f, 0.1f, 0.0f);
        
        GLuint idVBO_transf;
        glm::vec3 positions[numberOfConfetti];
        glm::vec3 rotations[numberOfConfetti];
        Model* confetti;

        ConfettiModel(Model* confetti);
        void drawConfetti();
};

ConfettiModel::ConfettiModel(Model* confetti) {
    this->confetti = confetti;
    
    for(int i=0; i<numberOfConfetti; i++) {
        positions[i] = glm::vec3(
            minXZ + rand()/(float)RAND_MAX*(maxXZ-minXZ),
            rand() / (float) RAND_MAX * (maxY),
            minXZ + rand()/(float)RAND_MAX*(maxXZ-minXZ)
        );
        rotations[i] = glm::vec3(
            rand()/(float)RAND_MAX*(2*M_PI),
            rand()/(float)RAND_MAX*(2*M_PI),
            rand()/(float)RAND_MAX*(2*M_PI)
        );
    }

    glBindVertexArray(this->confetti->idVAO);

    glGenBuffers(1, &(this->idVBO_transf));
    glBindBuffer(GL_ARRAY_BUFFER, this->idVBO_transf);
    glBufferData(GL_ARRAY_BUFFER, numberOfConfetti * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);

    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(3 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
}

void ConfettiModel::drawConfetti() {
    glBindVertexArray(this->confetti->idVAO);
	glUseProgram(this->confetti->program->id);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(this->confetti->texture->target, this->confetti->texture->id);

    glm::mat4 transformations[numberOfConfetti];
    for (int i=0; i<numberOfConfetti; i++) {
        transformations[i] = glm::mat4(1.0f);
        transformations[i] = glm::translate(transformations[i],this->positions[i]);
        transformations[i] = glm::rotate(transformations[i], this->rotations[i].y, glm::vec3(0.0,1.0,0.0));
        transformations[i] = glm::rotate(transformations[i], this->rotations[i].x, glm::vec3(1.0,0.0,0.0));
        transformations[i] = glm::rotate(transformations[i], this->rotations[i].z, glm::vec3(0.0,0.0,1.0));
        transformations[i] = glm::scale(transformations[i], glm::vec3(this->confetti->scale));

        positions[i] += velocity;
        rotations[i] += rotationVelocity;

        if(positions[i].y < -0.1f)
            positions[i].y = maxY;
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->idVBO_transf);
    glBufferSubData(GL_ARRAY_BUFFER, 0, numberOfConfetti * sizeof(glm::mat4), &transformations[0][0]);

	matView = getViewMatrix();
	matProjView = matProj * matView;
	matNormal = glm::transpose(glm::inverse(matModel));

	glUniformMatrix4fv( glGetUniformLocation(this->confetti->program->id, "matProjView"), 1, GL_FALSE,  glm::value_ptr(matProjView));
	glUniformMatrix3fv( glGetUniformLocation(this->confetti->program->id, "matNormal"), 1, GL_FALSE,  glm::value_ptr(matNormal));
	
	glUniform1i(glGetUniformLocation(this->confetti->program->id, "tex0"), 0);
	glDrawArraysInstanced(GL_TRIANGLES, 0, this->confetti->vertex_count, numberOfConfetti);
};