#ifndef RENDER
#define RENDER

#include <SDL2/SDL.h>
#include "vec2.h"
#include "player.h"
#include "map.h"

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} RGB;

typedef struct {
    int width;
    int height;
    RGB **bitmap;
} Texture;

Texture** load_textures();

void unload_textures(Texture **textures);

void render(SDL_Renderer *renderer, Texture **loaded_textures, Player *player);

#endif