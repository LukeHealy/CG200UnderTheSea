#pragma once

struct Image {
  unsigned int sizeX;
  unsigned int sizeY;
  char *data;
};

typedef struct Image Image;

int ImageLoad(char*, Image*);
GLuint LoadGLTextures();

