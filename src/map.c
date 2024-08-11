#include "../include/map.h"

// 0 - no wall
// 1 - black wall
// 2 - gray wall
// 3 - white wall
// 4 - red wall
// 5 - green wall
// 6 - blue wall
// 7 - yellow wall
// 8 - cyan wall
// 9 - purple wall
// 10 - orange wall
// 11 - brick wall
// 12 - stone wall
// 13 - gradient wall
// 14 - colorful gradient wall

int map[MAP_HEIGHT][MAP_WIDTH] = 
{
    {11, 11, 12, 12, 12, 11, 13, 13, 13, 11},
    {11,  0,  0,  0, 14,  0,  0,  0,  0, 11},
    {15, 15, 15,  0, 14,  0, 13,  0,  0, 11},
    {15,  0, 12,  0, 14,  0,  0,  0,  0, 11},
    {15,  0,  0,  0, 14, 14, 14,  0, 13, 11},
    {16,  0,  0,  0,  0,  0,  0,  0,  0, 11},
    {16,  0,  0,  0, 14, 14, 14,  0, 13, 11},
    {16,  0,  0,  0, 14,  0,  0,  0,  0, 11},
    {16,  0,  0,  0,  0,  0,  0,  0,  0, 11},
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 11},
};