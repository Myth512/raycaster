#include "../include/window.h"

GLFWwindow* window_create(Key_states *key_states) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return 0;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raycaster", NULL, NULL);
    if (!window)
        return 0;

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        return 0;

    glfwSetWindowUserPointer(window, key_states);
    glfwSetKeyCallback(window, key_callback);
    return window;
}

void window_destroy(GLFWwindow *window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}