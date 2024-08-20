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
    unsigned char alpha;
} RGB;

typedef struct {
    int width;
    int height;
    RGB **bitmap;
} Texture;

typedef struct {
    int size;
    Texture *el[];
} Texture_vector;

Texture_vector* load_textures();

void unload_textures(Texture_vector *textures);

#endif