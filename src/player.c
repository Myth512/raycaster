#include "../include/player.h"

void player_rotate(Player *player, int direction) {
    player->angle += player->speed.y * direction;
    if (player->angle > 2*PI || player->angle < 0)
        player->angle = fmod(player->angle, 2*PI);
}

void player_move(Player *player, int direction) {
    double dx = player->speed.x * cos(player->angle) * direction;
    double dy = player->speed.x * sin(player->angle) * direction;
    player->pos.x += dx;
    player->pos.y += dy;
}