void displayScene() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for(Model m : models) {
		if(m.active)
			DrawModel(m);
	}

	glDisable(GL_DEPTH_TEST);
	for(Text t : texts)
		drawText(t);
	glEnable(GL_DEPTH_TEST);

	glUseProgram( 0 );
	glBindVertexArray( 0 );

	glutSwapBuffers();
}

void initialize() {
	glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
	glEnable(GL_DEPTH_TEST);
	frameCounter = 0;

	Program* program = new Program("shaders/vertex.glsl", "shaders/fragment.glsl");

	Texture* tiles = new Texture("textures/tiles.jpg");
	Texture* tree = new Texture("textures/tree.png");

	ground = new Ground("models/ground.obj");

	Model groundModel(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, "models/ground.obj", program, tiles, false, false);

	models.push_back(groundModel);

	for (int i=0; i<40; i++) {
		float min = -15.0f;
		float max = 15.0f;
		float x = min + rand()/(float)RAND_MAX*(max-min); //losowanie między 2 liczbami
	    float z = min + rand()/(float)RAND_MAX*(max-min);
		Model starModel(x, ground->getAltitude(glm::vec3(x, 0.0f, z)), z, 0.0f, 0.0f, 0.0f, 0.5f, "models/star.obj", program, tree, false, true);
		models.push_back(starModel);
	}

	Font* pixeledFont = new Font("fonts/Pixeled.ttf");
	Font* digitalDarkSystemFont = new Font("fonts/digital dark system.ttf");

	Text fpsText(std::to_string(frameCounter), 1160, 680, 0.2f, glm::vec3(0.47f, 0.0f, 0.85f), pixeledFont);
	Text starCounterText(std::to_string(starCounter), 25, 40, 1.5f, glm::vec3(0.47f, 0.0f, 0.85f), digitalDarkSystemFont);

	texts.push_back(fpsText);
	texts.push_back(starCounterText);
}