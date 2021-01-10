#include <chrono>

#define __CHECK_FOR_ERRORS {GLenum errCode; if ((errCode = glGetError()) != GL_NO_ERROR) printf("Error (%d): %s in file %s at line %d !\n", errCode, gluErrorString(errCode), __FILE__,  __LINE__);}

void keyboard();

void animation() {
    keyboard();

    texts[0].text = std::to_string(frameCounter) + " fps";
    texts[1].text = std::to_string(starCounter);

    for(int i=1; i<41; i+=2)
        models[i].rotation_y += 0.025f;

    for(int i=2; i<41; i+=2)
        models[i].rotation_y -= 0.025f;

    __CHECK_FOR_ERRORS;
	glutPostRedisplay();

    // https://stackoverflow.com/questions/28530798/how-to-make-a-basic-fps-counter
    static std::chrono::time_point<std::chrono::system_clock> oldTime = std::chrono::high_resolution_clock::now();
    static int fps; fps++;

    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime) >= std::chrono::seconds{ 1 }) {
        oldTime = std::chrono::high_resolution_clock::now();
        frameCounter = fps;
        fps = 0;
    }
}