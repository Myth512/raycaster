#ifndef RENDER
#define RENDER

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vec2.h"
#include "player.h"
#include "texture.h"
#include "map.h"

void render(GLFWwindow *window, RGB frame_buffer[WINDOW_HEIGHT][WINDOW_WIDTH], Texture_vector *loaded_textures, Player *player);

#endif