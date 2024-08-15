#ifndef WINDOW
#define WINDOW

#include <stdbool.h>
#include "constants.h"
#include "player.h"
#include "input.h"

GLFWwindow* window_create(Key_states *key_states);

void window_destroy(GLFWwindow *window);

#endif