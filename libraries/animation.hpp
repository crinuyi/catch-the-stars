#include <chrono>

#define __CHECK_FOR_ERRORS {GLenum errCode; if ((errCode = glGetError()) != GL_NO_ERROR) printf("Error (%d): %s in file %s at line %d !\n", errCode, gluErrorString(errCode), __FILE__,  __LINE__);}

void keyboard();

void animation() {
    keyboard();
    __CHECK_FOR_ERRORS;
	glutPostRedisplay();
}