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

void render(SDL_Renderer *renderer, Player *player);

#endif