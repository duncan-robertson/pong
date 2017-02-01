#ifndef IMAGE_LOADER_HPP
#define IMAGE_LOADER_HPP

#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#ifdef __APPLE__
    #include <gl.h>
#else
    #include <GL/gl.h>
#endif


GLuint loadBMP_custom(const char* imagepath);
GLuint loadBMP_SDL(const char* imagepath);

#endif
