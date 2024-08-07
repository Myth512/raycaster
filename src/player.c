#include "../include/player.h"
#include <stdio.h>

void player_rotate(Player *player, double delta_time) {
    player->angle += TURNING_SPEED * player->turnDirection * delta_time;
    if (player->angle > 2*PI)
        player->angle -= 2*PI;
    if (player->angle < 0)
        player->angle += 2*PI;
}

void player_move(Player *player, double delta_time) {
    printf("%f\n", delta_time);
    double dx = MOVING_SPEED * cos(player->angle) * player->moveDirection * delta_time;
    double dy = MOVING_SPEED * sin(player->angle) * player->moveDirection * delta_time;
    player->pos.x += dx;
    player->pos.y += dy;
}