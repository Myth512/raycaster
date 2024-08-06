#include "../include/include.h"

// int map[MAP_HEIGHT][MAP_WIDTH] = 
// {
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
//     {1, 1, 1, 0, 1, 0, 1, 0, 0, 1},
//     {1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
//     {1, 0, 1, 0, 1, 1, 1, 0, 1, 1},
//     {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
//     {1, 0, 0, 0, 1, 1, 1, 0, 1, 1},
//     {1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
//     {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
// };

int map[MAP_HEIGHT][MAP_WIDTH] = 
{
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 1, 0, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

void draw(Player *player, SDL_Renderer *renderer) {
    double angleStep = 1.39626 / WINDOW_WIDTH;
    double rayAngle = player->angle - 1.39626 / 2;
    vec2 rayPos = {0, 0};
    for (int i = 0; i < WINDOW_WIDTH; i++) {
        if (rayAngle < 0)
            rayAngle += 2*PI;
        if (rayAngle > 2*PI)
            rayAngle -= 2*PI;

        //horizontal check
        vec2 rayStep = {10, 10};
        vec2 dist = {0, 0};
        if (i == WINDOW_WIDTH / 2)
            printf("rayAngle = %f playerAngle = %f\n", rayAngle, player->angle);
        double aTan = -1 / tan(rayAngle);
        if (rayAngle > PI) {
            rayPos.y = (int)player->pos.y - 0.001;
            rayPos.x = (player->pos.y - rayPos.y) * aTan + player->pos.x;
            rayStep.y = -1;
            rayStep.x = aTan;
        } else {
            rayPos.y = (int)player->pos.y + 1;
            rayPos.x = (player->pos.y - rayPos.y) * aTan + player->pos.x;
            rayStep.y = 1;
            rayStep.x = -aTan;
        }
        // printf("horizontal step %f %f\n", rayStep.x, rayStep.y);
        int hit = 0;
        while (rayPos.x > 0 && rayPos.x < MAP_WIDTH && rayPos.y > 0 && rayPos.y < MAP_HEIGHT) {
            hit = map[(int)rayPos.y][(int)rayPos.x];
            if (hit && i == WINDOW_WIDTH / 2)
                printf("horizontal hit at x: %f y: %f\t", rayPos.x, rayPos.y);
            if (hit)
                break;
            rayPos = vec2_add(&rayPos, &rayStep);
        }
        vec2 tmp = rayPos;
        dist.x = vec2_dist(&rayPos, &player->pos);
        if (hit && i == WINDOW_WIDTH / 2)
            printf("d: %f\n", dist.x);

        //vertival check
        double nTan  = -tan(rayAngle);
        if (rayAngle > PI/2 && rayAngle < 3*PI/2) {
            rayPos.x = (int)player->pos.x - 0.0001;
            rayPos.y = (player->pos.x - rayPos.x) * nTan + player->pos.y;
            rayStep.y = nTan;
            rayStep.x = -1;
        } else {
            rayPos.x = (int)player->pos.x + 1;
            rayPos.y = (player->pos.x - rayPos.x) * nTan + player->pos.y;
            rayStep.y = -nTan;
            rayStep.x = 1;
        }

        if (i == WINDOW_WIDTH / 2)
            printf("here x: %f y: %f\n", tmp.x, tmp.y);
        // printf("vertiacal step %f %f\n", rayStep.x, rayStep.y);
        hit = 0;
        while (rayPos.x > 0 && rayPos.x < MAP_WIDTH && rayPos.y > 0 && rayPos.y < MAP_HEIGHT) {
            hit = map[(int)rayPos.y][(int)rayPos.x];
            // if (hit && i == WINDOW_WIDTH / 2)
                // printf("vertical hit at x: %f y: %f\t", rayPos.x, rayPos.y);
            if (hit)
                break;
            rayPos = vec2_add(&rayPos, &rayStep);
        }
        dist.y = vec2_dist(&rayPos, &player->pos);
        // if (hit && i == WINDOW_WIDTH / 2)
            // printf("d: %f\n", dist.y);

        // double minDist = dist.x < dist.y ? dist.x : dist.y;
        // int wall = 0;
        // vec2 step = {cos(rayAngle) / 1024, sin(rayAngle) / 1024};
        // while (!wall) {
        //     rayPos = vec2_add(&rayPos, &step);
        //     wall = map[(int)rayPos.x][(int)rayPos.y];
        // }


        int red = 0;
        int green = 0;
        int blue = 0;
        int wallHeight = 0;

        if (dist.x < dist.y) { 
            wallHeight = WINDOW_HEIGHT / (dist.x * cos(player->angle - rayAngle)); 
            if (i == WINDOW_WIDTH / 2)
                blue = 255;
            red = 255 / dist.x;
        } else {
            wallHeight = WINDOW_HEIGHT / (dist.y * cos(player->angle - rayAngle));
            if (i == WINDOW_WIDTH / 2)
                blue = 255;
            green = 255 / dist.y;
        }
        if (wallHeight > WINDOW_HEIGHT)
            wallHeight = WINDOW_HEIGHT;

        // int red = 192 / minDist;
        // if (red > 192)
        //     red = 192;

        // int green = 96 / minDist;
        // if (green > 96)
        //     green = 96;

        window_draw_line(renderer, i, 0, 1, (WINDOW_HEIGHT - wallHeight) / 2, 0, 48, 128);
        window_draw_line(renderer, i, (WINDOW_HEIGHT - wallHeight) / 2, 1, wallHeight, red, green, blue);
        window_draw_line(renderer, i, (WINDOW_HEIGHT + wallHeight) / 2, 1, (WINDOW_HEIGHT - wallHeight), 64, 32, 64);

        if (dist.x < dist.y) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawLine(renderer, player->pos.x * 32, player->pos.y * 32, tmp.x * 32, tmp.y * 32);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderDrawLine(renderer, player->pos.x * 32, player->pos.y * 32, rayPos.x * 32, rayPos.y * 32);
        }
        // if (i == WINDOW_HEIGHT) {
        //     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //     SDL_RenderDrawLine(renderer, player->pos.x * 32, player->pos.y * 32, tmp.x * 32, tmp.y * 32);
        //     // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        //     // SDL_RenderDrawLine(renderer, player->pos.x * 32, player->pos.y * 32, (int)(rayPos.x * 32), (int)(rayPos.y * 32));
        // }
        // printf("player x: %f y: %f a: %f\n", player->pos.x, player->pos.y, player->angle);

        rayAngle += angleStep;
    }
}

void drawMiniMap(SDL_Renderer *renderer) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x]) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect rect = {x * 32, y * 32, 31, 31};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void drawPlayer(SDL_Renderer *renderer, Player *player) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect rect = {player->pos.x * 32 - 3, player->pos.y * 32 - 3, 6, 6};
    SDL_RenderFillRect(renderer, &rect);
    vec2 next = {20 * cos(player->angle), 20 *sin(player->angle)};
    SDL_RenderDrawLine(renderer, player->pos.x * 32, player->pos.y * 32, player->pos.x * 32 + next.x, player->pos.y * 32 + next.y);
}

int main() {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    window_initialize(&window, &renderer);
    window_clear(renderer);
    Player player = {{5, 5}, 0, {0.1, 0.05}, 80};
    bool is_running = true;

    while (is_running) {
        window_process_input(&is_running, &player);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        draw(&player, renderer);
        drawMiniMap(renderer);
        drawPlayer(renderer, &player);
        SDL_RenderPresent(renderer);
    }
    window_destroy(window, renderer);
    SDL_Quit();
}

