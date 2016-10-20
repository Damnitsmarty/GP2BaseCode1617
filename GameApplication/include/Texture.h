#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "Common.h"

//Loads Texture from Memory
GLuint loadTextureFromMemory(const char * pMem, int size);

//Load Texture from File
GLuint	loadTextureFromFile(const string& filename);

//Load Texture from font
GLuint	loadTextureFromFont(const string& fontFilename, int	pointSize, const string& text);

//Utility function to convert any SDL surface to OpenGL Texture
GLuint convertSDLSurfaceToTexture(SDL_Surface * surface);

#endif
