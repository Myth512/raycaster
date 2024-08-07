#include "../include/input.h"

void process_input(Key_states *key_states, bool *is_running) {
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
            key_states->move_forward = true;
		if (event.key.keysym.sym == SDLK_s)
            key_states->move_backward = true;
		if (event.key.keysym.sym == SDLK_a)
            key_states->turn_left = true;
		if (event.key.keysym.sym == SDLK_d)
            key_states->turn_right = true;
		break;
	case SDL_KEYUP:
		if (event.key.keysym.sym == SDLK_w)
            key_states->move_forward = false;
		if (event.key.keysym.sym == SDLK_s)
            key_states->move_backward = false;
		if (event.key.keysym.sym == SDLK_a)
            key_states->turn_left = false;
		if (event.key.keysym.sym == SDLK_d)
            key_states->turn_right = false;
		break;
	}
	return;
}