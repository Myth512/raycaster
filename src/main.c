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
    {5, 5, 5, 7, 7, 7, 7, 5, 5, 5},
    {3, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {3, 0, 1, 1, 0, 0, 1, 2, 0, 6},
    {3, 0, 1, 0, 0, 0, 0, 3, 0, 6},
    {3, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {2, 0, 2, 0, 0, 0, 0, 3, 0, 6},
    {2, 0, 2, 2, 0, 0, 3, 3, 0, 6},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 6},
    {2, 1, 1, 1, 1, 4, 4, 4, 4, 4},
};

// 0 - no wall
// 1 - red wall
// 2 - green wall
// 3 - blue wall
// 4 - yellow
// 5 - cyan
// 6 - purple
// 7 - white
// 8 - black

double degToRad(double angle) {
    return angle * PI / 180;
}

void draw(Player *player, SDL_Renderer *renderer) {
    double angleStep = degToRad(player->FOV) / WINDOW_WIDTH;
    double rayAngle = player->angle - degToRad(player->FOV) / 2;
    vec2 rayPos = {0, 0};
    for (int i = 0; i < WINDOW_WIDTH; i++) {
        if (rayAngle < 0)
            rayAngle += 2*PI;
        if (rayAngle > 2*PI)
            rayAngle -= 2*PI;

        vec2 rayStep = {10, 10};
        vec2 dist = {0, 0};
        vec2 wallColor = {0, 0};
        double aTan = -1 / tan(rayAngle);

        if (rayAngle > PI) {
            rayPos.y = (int)player->pos.y - 0.001; // i have no clue what the purpose of this number
            rayPos.x = (player->pos.y - rayPos.y) * aTan + player->pos.x;
            rayStep.y = -1;
            rayStep.x = aTan;
        } else {
            rayPos.y = (int)player->pos.y + 1;
            rayPos.x = (player->pos.y - rayPos.y) * aTan + player->pos.x;
            rayStep.y = 1;
            rayStep.x = -aTan;
        }

        while (rayPos.x > 0 && rayPos.x < MAP_WIDTH && rayPos.y > 0 && rayPos.y < MAP_HEIGHT) {
            wallColor.x = map[(int)rayPos.y][(int)rayPos.x];
            if (wallColor.x)
                break;
            rayPos = vec2_add(&rayPos, &rayStep);
        }
        vec2 tmp = rayPos;
        dist.x = vec2_dist(&rayPos, &player->pos);

        double nTan = -tan(rayAngle);
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

        while (rayPos.x > 0 && rayPos.x < MAP_WIDTH && rayPos.y > 0 && rayPos.y < MAP_HEIGHT) {
            wallColor.y = map[(int)rayPos.y][(int)rayPos.x];
            if (wallColor.y)
                break;
            rayPos = vec2_add(&rayPos, &rayStep);
        }
        dist.y = vec2_dist(&rayPos, &player->pos);


        int wallHeight = 0;

        double minDist = dist.x;
        int closestWallColor = wallColor.x;
        if (dist.x > dist.y) {
            minDist = dist.y;
            closestWallColor = wallColor.y;
        }
        minDist *= cos(player->angle - rayAngle);

        int red = 0, green = 0, blue = 0;
        switch(closestWallColor) {
            case 1:
                red = 255;
                break;
            case 2:
                green = 255;
                break;
            case 3:
                blue = 255;
                break;
            case 4:
                red = 255;
                green = 255;
                break;
            case 5:
                green = 255;
                blue = 255;
                break;
            case 6:
                blue = 255;
                red = 255;
                break;
            case 7:
                red = 255;
                green = 255;
                blue = 255;
                break;
        }
        
        red = red / minDist;
        green = green / minDist;
        blue = blue / minDist;

        wallHeight = WINDOW_HEIGHT / minDist; 
        if (wallHeight > WINDOW_HEIGHT)
            wallHeight = WINDOW_HEIGHT;

        window_draw_line(renderer, i, 0, 1, (WINDOW_HEIGHT - wallHeight) / 2, 0, 48, 128);
        window_draw_line(renderer, i, (WINDOW_HEIGHT - wallHeight) / 2, 1, wallHeight, red, green, blue);
        window_draw_line(renderer, i, (WINDOW_HEIGHT + wallHeight) / 2, 1, (WINDOW_HEIGHT - wallHeight), 64, 32, 64);

        if (dist.x < dist.y) {
            SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
            SDL_RenderDrawLine(renderer, player->pos.x * 32, player->pos.y * 32, tmp.x * 32, tmp.y * 32);
        } else {
            SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
            SDL_RenderDrawLine(renderer, player->pos.x * 32, player->pos.y * 32, rayPos.x * 32, rayPos.y * 32);
        }

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
    Player player = {{5, 5}, 0, {0.1, 0.05}, 90};
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

