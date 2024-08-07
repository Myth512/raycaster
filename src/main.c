#include "../include/include.h"

int main() {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Key_states key_states = {0};
    bool is_running = window_initialize(&window, &renderer);
    Uint32 frame_start_time = 0, frame_end_time = 0, frame_time = 0;
    Player player = {{5, 5}, PI / 4};

    while (is_running) {
        frame_start_time = SDL_GetTicks();

        process_input(&key_states, &is_running);

        update(&key_states, frame_time / 1000.0f, &player);

        render(renderer, &player);

        frame_end_time = SDL_GetTicks();
        frame_time = frame_end_time - frame_start_time;
    }
    window_destroy(window, renderer);
}

