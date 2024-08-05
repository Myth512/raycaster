#ifndef WINDOW
#define WINDOW

#include "constants.h"
#include "player.h"

bool window_initialize(SDL_Window **window, SDL_Renderer **renderer);

void window_destroy(SDL_Window *window, SDL_Renderer *renderer);

void window_process_input(bool *state, Player *player);

void window_clear(SDL_Renderer *renderer);

void window_draw_line(SDL_Renderer *renderer, int x, int y, int width, int height, int r, int g, int b);

#endif