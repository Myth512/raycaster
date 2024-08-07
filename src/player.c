#include "../include/player.h"

void player_rotate(Player *player) {
    player->angle += player->speed.y * player->turnDirection;
    if (player->angle > 2*PI)
        player->angle -= 2*PI;
    if (player->angle < 0)
        player->angle += 2*PI;
}

void player_move(Player *player) {
    double dx = player->speed.x * cos(player->angle) * player->moveDirection;
    double dy = player->speed.x * sin(player->angle) * player->moveDirection;
    player->pos.x += dx;
    player->pos.y += dy;
}