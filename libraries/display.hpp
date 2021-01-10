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

	ground = new Ground("models/scene-levels.obj");

	Model groundModel(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, "models/scene-levels.obj", program, tiles, false, false);

	models.push_back(groundModel);

	Font* pixeledFont = new Font("fonts/Pixeled.ttf");

	Text fps_text(std::to_string(frameCounter), 1160, 680, 0.2f, glm::vec3(0.47f, 0.0f, 0.85f), pixeledFont);

	texts.push_back(fps_text);
}