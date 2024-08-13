#include "../include/include.h"

int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        return -1;

    Key_states key_states = {0};
    glfwSetWindowUserPointer(window, &key_states);
    glfwSetKeyCallback(window, key_callback);

    RGB frame_buffer[WINDOW_HEIGHT][WINDOW_WIDTH];
    Texture **loaded_textures = load_textures();

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
    unload_textures(loaded_textures);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

