#include "../include/player.h"

void player_turn(Player *player, int direction, double delta_time) {
    player->angle += TURNING_SPEED * direction * delta_time;
    if (player->angle > 2*PI)
        player->angle -= 2*PI;
    if (player->angle < 0)
        player->angle += 2*PI;
}

void player_move(Player *player, int direction, double delta_time) {
    vec2 new_pos;
    new_pos.x = player->pos.x + MOVING_SPEED * cos(player->angle) * direction * delta_time;
    new_pos.y = player->pos.y + MOVING_SPEED * sin(player->angle) * direction * delta_time;

    if (player->angle < PI/2 || player->angle > 3*PI/2) {
        if (!map[(int)player->pos.y][(int)(new_pos.x + PLAYER_SIZE * direction)])
            player->pos.x = new_pos.x;
    } else {
        if (!map[(int)player->pos.y][(int)(new_pos.x - PLAYER_SIZE * direction)])
            player->pos.x = new_pos.x;
    }
    if (player->angle < PI) {
        if (!map[(int)(new_pos.y + PLAYER_SIZE * direction)][(int)player->pos.x])
            player->pos.y = new_pos.y;
    } else {
        if (!map[(int)(new_pos.y - PLAYER_SIZE * direction)][(int)player->pos.x])
            player->pos.y = new_pos.y;
    }
}