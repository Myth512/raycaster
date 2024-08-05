#include "../include/vec2.h"

vec2 vec2_add(vec2 *a, vec2 *b) {
    return (vec2){a->x + b->x, a->y + b->y};
}

vec2 vec2_sub(vec2 *a, vec2 *b) {
    return (vec2){a->x - b->x, a->y - b->y};
}

vec2 vec2_dot(vec2 *a, vec2 *b) {
    return (vec2){a->x * b->x, a->y * b->y};
}

vec2 vec2_multiply(vec2 *a, double c) {
    return (vec2){a->x * c, a->y * c};
}

double vec2_len(vec2 *a) {
    return sqrt(a->x * a->x + a->y * a->y);
}

double vec2_dist(vec2 *a, vec2 *b) {
    return sqrt((b->x - a->x) * (b->x - a->x) + (b->y - a->y) * (b->y - a->y));
}