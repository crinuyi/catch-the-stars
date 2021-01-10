int keyPressed[256] = {0};
int _mouse_buttonState = GLUT_UP;
int _mouse_buttonX, _mouse_buttonY;
glm::vec3 cameraRotation = glm::vec3(0.0f,-(M_PI/2.0),0.0f); //pitch, yaw, roll
glm::vec3 cameraPosition = glm::vec3(0.0f,0.0f,0.0f);
glm::vec3 cameraFront = glm::vec3(0.0,0.0,-1.0);
glm::vec3 cameraUp = glm::vec3(0.0,1.0,0.0);
float cameraCollisionRadius = 1.0f;

//https://community.khronos.org/t/extracting-camera-position-from-a-modelview-matrix/68031
glm::vec3 ExtractCameraPos(const glm::mat4 & a_modelView) {
  // Get the 3 basis vector planes at the camera origin and transform them into model space.
  //
  // NOTE: Planes have to be transformed by the inverse transpose of a matrix
  //       Nice reference here: http://www.opengl.org/discussion_boards/showthread.php/159564-Clever-way-to-transform-plane-by-matrix
  //
  //       So for a transform to model space we need to do:
  //            inverse(transpose(inverse(MV)))
  //       This equals : transpose(MV) - see Lemma 5 in http://mathrefresher.blogspot.com.au/2007/06/transpose-of-matrix.html
  //
  // As each plane is simply (1,0,0,0), (0,1,0,0), (0,0,1,0) we can pull the data directly from the transpose matrix.
  //
  glm::mat4 modelViewT = transpose(a_modelView);

  // Get plane normals
  glm::vec3 n1(modelViewT[0]);
  glm::vec3 n2(modelViewT[1]);
  glm::vec3 n3(modelViewT[2]);

  // Get plane distances
  float d1(modelViewT[0].w);
  float d2(modelViewT[1].w);
  float d3(modelViewT[2].w);

  // Get the intersection of these 3 planes
  // http://paulbourke.net/geometry/3planes/
  glm::vec3 n2n3 = cross(n2, n3);
  glm::vec3 n3n1 = cross(n3, n1);
  glm::vec3 n1n2 = cross(n1, n2);

  glm::vec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
  float denom = dot(n1, n2n3);

  return top / -denom;
}

void moveCamera(glm::vec3 vectorShift) {
	glm::vec3 newPosition = cameraPosition + vectorShift - glm::vec3(0.0f, 1.0f, 0.0f);
    float altitude = ground->getAltitude(newPosition);
	glm::vec3 newFixedPosition = glm::vec3(newPosition.x,altitude+1.0f, newPosition.z);
    if(altitude > -5.0f) {
		checkForStarsToCollect(newFixedPosition);
		if(checkIfNoCollisions(newFixedPosition))
			cameraPosition = newFixedPosition;
	}
}

glm::mat4 getViewMatrix() {
	glm::vec3 direction;
    direction.x = cos(cameraRotation[1]) * cos(cameraRotation[0]);
    direction.y = sin(cameraRotation[0]);
    direction.z = sin(cameraRotation[1]) * cos(cameraRotation[0]);
    cameraFront = glm::normalize(direction); //wektor, w którym patrzy kamera

    return glm::lookAt(
        cameraPosition,
        cameraPosition+cameraFront,
        cameraUp
    );
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	matProj = glm::perspective(glm::radians(80.0f), width/(float)height, 0.1f, 100.0f);
}

void specialKeys(int key, int x, int y) {

}

void keyboard() {
	glm::vec3 cameraFrontXZ = glm::normalize(glm::vec3(cameraFront.x, 0.0, cameraFront.z)); //kamera zawsze w poziomie
	if(keyPressed[27]) //ESC
		exit(0);
	if(keyPressed['w']) {
		moveCamera(cameraFrontXZ*0.05f);
	}
	if(keyPressed['s']) {
		moveCamera(-cameraFrontXZ*0.05f);
	}
	if(keyPressed['a']) {
		moveCamera(-glm::normalize(glm::cross(cameraFront, cameraUp)) * 0.05f);
	}
	if(keyPressed['d']) {
		moveCamera(glm::normalize(glm::cross(cameraFront, cameraUp)) * 0.05f);
	}
}

void keyboardUp(unsigned char key, int x, int y) {
    keyPressed[key] = 0;
}

void keyboardDown(unsigned char key, int x, int y) {
    keyPressed[key] = 1;
}

void mouseButton(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON) {
        _mouse_buttonState = state;

        if(state == GLUT_DOWN) {
            _mouse_buttonX = x;
            _mouse_buttonY = y;
        }
    }
}

void mouseMotion(int x, int y) {
    if(_mouse_buttonState == GLUT_DOWN)
    {
        cameraRotation.y += 2*(x - _mouse_buttonX)/(float)glutGet( GLUT_WINDOW_WIDTH );
        _mouse_buttonX = x;
        cameraRotation.x += 2*(_mouse_buttonY - y)/(float)glutGet( GLUT_WINDOW_HEIGHT );
        _mouse_buttonY = y;
		if(cameraRotation.x < -glm::radians(85.0f)) //blokowanie patrzenia prosto w górę 
			cameraRotation.x = -glm::radians(85.0f);
		if(cameraRotation.x > glm::radians(85.0f)) //blokowanie patrzenia prosto w dół
			cameraRotation.x = glm::radians(85.0f);
    }
}

void mouseWheel(int button, int dir, int x, int y) {

}