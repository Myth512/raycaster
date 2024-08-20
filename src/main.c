#include "../include/include.h"

int main() {
    Key_states key_states = {0};
    GLFWwindow *window = window_create(&key_states);

    RGB frame_buffer[WINDOW_HEIGHT][WINDOW_WIDTH];
    Texture_vector *loaded_textures = load_textures();

    clock_t frame_start_tick = 0, frame_end_tick = 0;
    double frame_time = 0;

    Player player = {{5, 5}, PI / 4};


    while (!glfwWindowShouldClose(window)) {
        frame_start_tick = clock();

        glfwPollEvents();

        update(&key_states, frame_time, &player);

        render(window, frame_buffer, loaded_textures, &player);

        frame_end_tick = clock();
        frame_time = (double)(frame_end_tick - frame_start_tick) / CLOCKS_PER_SEC;
    }

    window_destroy(window);
    unload_textures(loaded_textures);
    return 0;
}

