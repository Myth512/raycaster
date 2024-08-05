#include "../include/include.h"

int map[MAP_HEIGHT][MAP_WIDTH] = 
{
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

void draw(Player *player, SDL_Renderer *renderer) {
    vec2 ray_pos = {0, 0};
    double ray_angle = player->angle - 30 * DEGREE;
    if (ray_angle < 0)
        ray_angle += 2*PI;
    if (ray_angle > 2*PI)
        ray_angle -= 2*PI;

    vec2 step = {0, 0};
    vec2 distance = {10e6, 10e6};
    int x = 0;
    for (int r = 0; r < 120; r++) {
        int dof = 0;
        double aTan = -1 / tan(ray_angle);
        if (ray_angle > PI) {
            ray_pos.y = (int)player->pos.y - 0.0001;
            ray_pos.x = (player->pos.y - ray_pos.y) * aTan + player->pos.x;
            step.y = -1;
            step.x = aTan;
        }
        if (ray_angle < PI) {
            ray_pos.y = (int)player->pos.y + 1;
            ray_pos.x = (player->pos.y - ray_pos.y) * aTan + player->pos.x;
            step.y = 1;
            step.x = -aTan;
        }
        if (ray_angle == 0 || ray_angle == PI) {
                if (r == 60)
                    printf("im here1\n");
            ray_pos = player->pos;
            dof = 16;
        }
        while (dof < 16)
        {
            if (ray_pos.x < 0 || ray_pos.y < 0 || ray_pos.x >= MAP_WIDTH || ray_pos.y >= MAP_HEIGHT)
                break;
            if (map[(int)ray_pos.y][(int)ray_pos.x]) {
                if (r == 60)
                    printf("x: %f y: %f\n", ray_pos.x, ray_pos.y);
                distance.x = vec2_dist(&ray_pos, &player->pos);
                dof = 16;
            } else {
                ray_pos = vec2_add(&ray_pos, &step);
                dof++;
            }
        }

        dof = 0;
        double nTan = -tan(ray_angle);
        if (ray_angle > PI/2 && ray_angle < 3*PI/2) {
            ray_pos.x = (int)player->pos.x - 0.0001;
            ray_pos.y = (player->pos.x - ray_pos.x) * nTan + player->pos.y;
            step.x = -1;
            step.y = nTan;
        }
        if (ray_angle < PI/2 || ray_angle > 3*PI/2) {
            ray_pos.x = (int)player->pos.x + 1;
            ray_pos.y = (player->pos.x - ray_pos.x) * nTan + player->pos.y;
            step.x = 1;
            step.y = -nTan;
        }
        if (ray_angle == 0 || ray_angle == PI) {
            ray_pos = player->pos;
            dof = 16;
        }
        while (dof < 16)
        {
            if (ray_pos.x < 0 || ray_pos.y < 0 || ray_pos.x >= MAP_WIDTH || ray_pos.y >= MAP_HEIGHT)
                break;
            if (map[(int)ray_pos.y][(int)ray_pos.x]) {
                if (r == 60)
                    printf("x: %f y: %f\n", ray_pos.x, ray_pos.y);
                distance.y = vec2_dist(&ray_pos, &player->pos);
                dof = 16;
            } else {
                if (r == 60)
                    printf("im here\n");
                ray_pos = vec2_add(&ray_pos, &step);
                dof++;
            }
        }

        double min_dist = distance.x < distance.y ? distance.x : distance.y;
        if (r == 60)
            printf("x: %f y: %f a: %f d: %f\n", player->pos.x, player->pos.y, player->angle, min_dist);
        double lineH = 720/min_dist;
        if (lineH > 720)
            lineH = 720;
        
        int y = 360 - (int)lineH/2;
        int red = 0;
        if (r == 60)
            red = 255;
        window_draw_line(renderer, x, y, 10, (int)lineH, red, 0, 255);
        x += 10;
        ray_angle += DEGREE / 2;
    }
}
int main() {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    window_initialize(&window, &renderer);
    window_clear(renderer);
    Player player = {{5, 2}, PI, {0.05, 0.01}};
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

