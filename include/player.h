#ifndef PLAYER
#define PLAYER

#include "math.h"
#include "vec2.h"
#include "constants.h"

typedef struct {
    vec2 pos;
    double angle;
    vec2 speed;
    int FOV;
} Player;

void player_rotate(Player *player, int direction);

void player_move(Player *player, int direction);

#endif