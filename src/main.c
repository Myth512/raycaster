#include "../include/include.h"

int map[MAP_HEIGHT][MAP_WIDTH] = 
{
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 1, 0, 1, 1},
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

void draw(Player *player, SDL_Renderer *renderer) {
    double angleStep = 1.39626 / WINDOW_WIDTH;
    double rayAngle = player->angle - 1.39626 / 2;
    for (int i = 0; i < WINDOW_WIDTH; i++) {
        vec2 rayPos = player->pos;

        int wall = 0;
        vec2 step = {cos(rayAngle) / 128, sin(rayAngle) / 128};
        while (!wall) {
            rayPos = vec2_add(&rayPos, &step);
            wall = map[(int)rayPos.x][(int)rayPos.y];
        }

        double dist = vec2_dist(&rayPos, &player->pos);

        dist *= cos(rayAngle - player->angle);
    
        int wallHeight = WINDOW_HEIGHT / dist;
        if (wallHeight > WINDOW_HEIGHT)
            wallHeight = WINDOW_HEIGHT;

        int red = 192 / dist;
        if (red > 192)
            red = 192;

        int green = 96 / dist;
        if (green > 96)
            green = 96;

        window_draw_line(renderer, i, 0, 1, (WINDOW_HEIGHT - wallHeight) / 2, 0, 48, 128);
        window_draw_line(renderer, i, (WINDOW_HEIGHT - wallHeight) / 2, 1, wallHeight, red, green, 0);
        window_draw_line(renderer, i, (WINDOW_HEIGHT + wallHeight) / 2, 1, (WINDOW_HEIGHT - wallHeight), 64, 32, 64);
        printf("x: %f y: %f a: %f\n", player->pos.x, player->pos.y, player->angle);

        rayAngle += angleStep;
    }
}
int main() {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    window_initialize(&window, &renderer);
    window_clear(renderer);
    Player player = {{1.5, 1.5}, PI / 2, {0.05, 0.1}, 80};
    bool is_running = true;

    while (is_running) {
        window_process_input(&is_running, &player);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        draw(&player, renderer);
        SDL_RenderPresent(renderer);
    }
    window_destroy(window, renderer);
    SDL_Quit();
}

