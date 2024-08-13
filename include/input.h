#ifndef INPUT
#define INPUT

#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "player.h"

typedef struct {
    bool move_forward;
    bool move_backward;
    bool turn_left;
    bool turn_right;
} Key_states;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

#endif