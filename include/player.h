#ifndef PLAYER
#define PLAYER

#include <math.h>
#include "vec2.h"
#include "constants.h"

typedef struct {
    vec2 pos;
    double angle;
    int moveDirection;
    int turnDirection;
} Player;

void player_turn(Player *player, int direction, double delta_time);

void player_move(Player *player, int direction, double delta_time);

#endif