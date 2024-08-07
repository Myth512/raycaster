#ifndef INPUT
#define INPUT

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "player.h"

typedef struct {
    bool move_forward;
    bool move_backward;
    bool turn_left;
    bool turn_right;
} Key_states;

void process_input(Key_states *keys, bool *is_running);

#endif