#ifndef RENDER
#define RENDER

#include <SDL2/SDL.h>
#include "vec2.h"
#include "player.h"
#include "texture.h"
#include "map.h"

void render(SDL_Renderer *renderer, Texture **loaded_textures, Player *player);

#endif