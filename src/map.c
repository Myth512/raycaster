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
// 10 - reserved for background
// 11 - reserved for floor
// 12 - gradient 1
// 13 - gradient 2
// 14 - megumin

int map[MAP_HEIGHT][MAP_WIDTH] = 
{
    {14,  0,  0,  0,  0, 13, 12, 12, 12, 13},
    {14,  0,  0,  0,  0,  0,  0,  0,  0, 13},
    {14, 14, 14,  0, 14,  0, 12,  0,  0, 13},
    {12,  0, 14,  0, 14,  0,  0,  0,  0, 13},
    {12,  0,  0,  0, 14, 14, 13,  0, 12, 13},
    {12,  0,  0,  0,  0,  0,  0,  0,  0, 13},
    {12,  0,  0,  0, 14, 14, 13,  0, 12, 13},
    {12,  0,  0,  0, 12,  0,  0,  0,  0, 13},
    {12,  0,  0,  0,  0,  0,  0,  0,  0, 13},
    {12, 12, 12, 12, 12, 12, 12, 12, 12, 13},
};