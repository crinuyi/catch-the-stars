#include <stdio.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "libraries/globalVariables.hpp"
#include "libraries/objLoader.hpp"
#include "libraries/ground.hpp"
#include "libraries/shaderStuff.hpp"
#include "libraries/movement.hpp"
#include "libraries/program.hpp"
#include "libraries/model.hpp"
#include "libraries/font.hpp"
#include "libraries/text.hpp"
#include "libraries/animation.hpp"
#include "libraries/display.hpp"


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