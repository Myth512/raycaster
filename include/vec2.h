#ifndef VEC2
#define VEC2

#include <math.h>

typedef struct {
    double x;
    double y;
} vec2;

vec2 vec2_add(vec2 *a, vec2 *b);

vec2 vec2_sub(vec2 *a, vec2 *b);

vec2 vec2_dot(vec2 *a, vec2 *b);

vec2 vec2_multiply(vec2 *a, double c);

double vec2_len(vec2 *a);

double vec2_dist(vec2 *a, vec2 *b);

#endif