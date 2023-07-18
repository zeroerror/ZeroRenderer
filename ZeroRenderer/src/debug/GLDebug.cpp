#include <GL/glew.h>
#include <iostream>

#include "GLDebug.h"

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}