#include <stdio.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void displayScene();
void reshape(int width, int height);
void mouseButton(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void mouseWheel(int button, int dir, int x, int y);
void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void animation();

void initialize() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Catch the Stars");

	glutDisplayFunc(displayScene);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	glutMouseWheelFunc(mouseWheel);
	glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(specialKeys);
	glutIdleFunc(animation);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if( GLEW_OK != err ) {
		printf("GLEW Error\n");
		exit(1);
	}

	// OpenGL
	if( !GLEW_VERSION_3_2 )
	{
		printf("No OpenGL 3.2!\n");
		exit(1);
	}

	initialize();

	glutMainLoop();

	return 0;
}