#ifndef UPDATE
#define UPDATE

#include <stdbool.h>
#include "player.h"
#include "input.h"

void update(Key_states *key_states, double delta_time, Player *player);

#endif