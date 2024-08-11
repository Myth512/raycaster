#ifndef TEXTURE
#define TEXTURE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} RGB;

typedef struct {
    int width;
    int height;
    RGB **bitmap;
} Texture;

Texture** load_textures();

void unload_textures(Texture **textures);

#endif