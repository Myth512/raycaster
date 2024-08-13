#include "../include/input.h"
#include <stdio.h>

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	Key_states *key_states = (Key_states *)glfwGetWindowUserPointer(window);
	if (action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
			case GLFW_KEY_W:
				key_states->move_forward = true;
				break;
			case GLFW_KEY_S:
				key_states->move_backward = true;
				break;
			case GLFW_KEY_A:
				key_states->turn_left = true;
				break;
			case GLFW_KEY_D:
				key_states->turn_right = true;
				break;
		}
	} else if (action == GLFW_RELEASE) {
		switch(key) {
			case GLFW_KEY_W:
				key_states->move_forward = false;
				break;
			case GLFW_KEY_S:
				key_states->move_backward = false;
				break;
			case GLFW_KEY_A:
				key_states->turn_left = false;
				break;
			case GLFW_KEY_D:
				key_states->turn_right = false;
				break;
		}
	}
}