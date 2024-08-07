#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../include/window.h"

bool window_initialize(SDL_Window **window, SDL_Renderer **renderer)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Could not initialize SDL: \n%s", SDL_GetError());
		return false;
	}

	*window = SDL_CreateWindow(
		"Raycast Demo",
		320,
		180,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0);

	if (!*window)
	{
		fprintf(stderr, "Could not create window.\n");
		return false;
	}

	*renderer = SDL_CreateRenderer(*window, -1, 0);
	if (!*renderer)
	{
		fprintf(stderr, "Could not create renderer.\n");
		return false;
	}

	return true;
}

void window_destroy(SDL_Window *window, SDL_Renderer *renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return;
}
