void displayScene() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for(Model m : models) {
		if(m.active)
			DrawModel(m);
	}

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
}