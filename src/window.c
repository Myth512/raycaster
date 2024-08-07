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

void window_process_input(bool *is_running, Player *player)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		*is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			*is_running = false;
		if (event.key.keysym.sym == SDLK_w)
			player->moveDirection = 1;
		if (event.key.keysym.sym == SDLK_s)
			player->moveDirection = -1;
		if (event.key.keysym.sym == SDLK_a)
			player->turnDirection = -1;
		if (event.key.keysym.sym == SDLK_d)
			player->turnDirection = 1;
		break;
	case SDL_KEYUP:
		if (event.key.keysym.sym == SDLK_w)
			player->moveDirection = 0;
		if (event.key.keysym.sym == SDLK_s)
			player->moveDirection = 0;
		if (event.key.keysym.sym == SDLK_a)
			player->turnDirection = 0;
		if (event.key.keysym.sym == SDLK_d)
			player->turnDirection = 0;
		break;
	}
	return;
}

void window_clear(SDL_Renderer *renderer)
{
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer);
	return;
}

void window_draw_line(SDL_Renderer *renderer, int x, int y, int width, int height, int r, int g, int b) {
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_Rect rect = {x, y, width, height};
	SDL_RenderFillRect(renderer, &rect);
	return;

}
