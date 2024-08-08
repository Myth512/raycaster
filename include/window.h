#ifndef WINDOW
#define WINDOW

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "constants.h"
#include "player.h"

bool window_initialize(SDL_Window **window, SDL_Renderer **renderer);

void window_destroy(SDL_Window *window, SDL_Renderer *renderer);

#endif