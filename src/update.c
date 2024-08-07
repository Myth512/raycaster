#include "../include/update.h"

void update(Key_states *key_states, double delta_time, Player *player) {
    if (key_states->move_forward != key_states->move_backward) {
        if (key_states->move_forward)
            player_move(player, 1, delta_time);
        else
            player_move(player, -1, delta_time);
    }
    if (key_states->turn_left != key_states->turn_right) {
        if (key_states->turn_left)
            player_turn(player, -1, delta_time);
        else
            player_turn(player, 1, delta_time);
    }
}