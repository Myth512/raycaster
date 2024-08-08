#include "../include/render.h"

double deg_to_rad(double angle) {
    return angle * PI / 180;
}

void draw_line(SDL_Renderer *renderer, int x, int y, int width, int height, int red, int green, int blue) {
	SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
	SDL_Rect rect = {x, y, width, height};
	SDL_RenderFillRect(renderer, &rect);
}

void draw_texture_line(SDL_Renderer *renderer, int x, int y, int width, int height, Texture *texture, int tx) {
    int segment_height = ceil((double)height / TEXTURE_HEIGHT);
    double step = (double)height / TEXTURE_HEIGHT;
    for (int ty = 0; ty < TEXTURE_HEIGHT; ty++) {
        Uint8 red = texture->bitmap[ty][tx].red;
        Uint8 green = texture->bitmap[ty][tx].green;
        Uint8 blue = texture->bitmap[ty][tx].blue;
        draw_line(renderer, x, y + (int)(ty * step), 1, segment_height, red, green, blue);
    } 
}

void draw_scene(SDL_Renderer *renderer, Player *player) {
    double angleStep = deg_to_rad(FOV) / WINDOW_WIDTH;
    double rayAngle = player->angle - deg_to_rad(FOV) / 2;
    vec2 rayPos = {0, 0};
    for (int i = 0; i < WINDOW_WIDTH; i++) {
        if (rayAngle < 0)
            rayAngle += 2*PI;
        if (rayAngle > 2*PI)
            rayAngle -= 2*PI;

        vec2 rayStep = {0, 0};
        vec2 dist = {0, 0};
        vec2 wallColor = {0, 0};
        double aTan = -1 / tan(rayAngle);

        if (rayAngle > PI) {
            rayPos.y = (int)player->pos.y - 0.001; // I have no clue what the purpose of this number
            rayPos.x = (player->pos.y - rayPos.y) * aTan + player->pos.x;
            rayStep.y = -1;
            rayStep.x = aTan;
        } else {
            rayPos.y = (int)player->pos.y + 1;
            rayPos.x = (player->pos.y - rayPos.y) * aTan + player->pos.x;
            rayStep.y = 1;
            rayStep.x = -aTan;
        }

        if (rayPos.x > 0 && rayPos.x < MAP_WIDTH && rayPos.y > 0 && rayPos.y < MAP_HEIGHT)
            wallColor.x = map[(int)rayPos.y][(int)rayPos.x];
        while (!wallColor.x) {
            rayPos = vec2_add(&rayPos, &rayStep);
            if (rayPos.x > 0 && rayPos.x < MAP_WIDTH && rayPos.y > 0 && rayPos.y < MAP_HEIGHT)
                wallColor.x = map[(int)rayPos.y][(int)rayPos.x];
            else {
                rayPos = vec2_multiply(&rayPos, 10000);
                break;
            }
        }
        dist.x = vec2_dist(&rayPos, &player->pos);
        vec2 tmp = rayPos;

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

        if (rayPos.x > 0 && rayPos.x < MAP_WIDTH && rayPos.y > 0 && rayPos.y < MAP_HEIGHT)
            wallColor.y = map[(int)rayPos.y][(int)rayPos.x];
        while (!wallColor.y) {
            rayPos = vec2_add(&rayPos, &rayStep);
            if (rayPos.x > 0 && rayPos.x < MAP_WIDTH && rayPos.y > 0 && rayPos.y < MAP_HEIGHT)
                wallColor.y = map[(int)rayPos.y][(int)rayPos.x];
            else {
                rayPos = vec2_multiply(&rayPos, 10000);
                break;
            }
        }
        dist.y = vec2_dist(&rayPos, &player->pos);

        double minDist = dist.x;
        int closestWallColor = wallColor.x;
        if (dist.x > dist.y) {
            minDist = dist.y;
            closestWallColor = wallColor.y;
        } else
            rayPos = tmp;
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
        if (red > 255)
            red = 255;
        green = green / minDist;
        if (green > 255)
            green = 255;
        blue = blue / minDist;
        if (blue > 255)
            blue = 255;

        int wallHeight = WINDOW_HEIGHT / minDist; 
        if (wallHeight > WINDOW_HEIGHT)
            wallHeight = WINDOW_HEIGHT;

        draw_line(renderer, i, 0, 1, (WINDOW_HEIGHT - wallHeight) / 2, 0, 48, 128);
        if (closestWallColor == 8) {
            Texture texture = {TEXTURE_BRICK_WALL};
            int tx = fmod((rayPos.x + rayPos.y) * 8, 8);
            draw_texture_line(renderer, i, (WINDOW_HEIGHT - wallHeight) / 2, 1, wallHeight, &texture, tx);
        }
        else
            draw_line(renderer, i, (WINDOW_HEIGHT - wallHeight) / 2, 1, wallHeight, red, green, blue);
        draw_line(renderer, i, (WINDOW_HEIGHT + wallHeight) / 2, 1, (WINDOW_HEIGHT - wallHeight), 64, 32, 64);

        if (rayPos.x > 0 && rayPos.x < MAP_WIDTH && rayPos.y > 0 && rayPos.y < MAP_HEIGHT) {
            SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
            SDL_RenderDrawLine(renderer, player->pos.x * MINIMAP_SCALE, player->pos.y * MINIMAP_SCALE, rayPos.x * MINIMAP_SCALE, rayPos.y * MINIMAP_SCALE);
        }

        rayAngle += angleStep;
    }
}

void draw_minimap(SDL_Renderer *renderer) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x]) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect rect = {x * MINIMAP_SCALE, y * MINIMAP_SCALE, MINIMAP_SCALE - 1, MINIMAP_SCALE - 1};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void draw_player_icon(SDL_Renderer *renderer, Player *player) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect rect = {player->pos.x * MINIMAP_SCALE - 3, player->pos.y * MINIMAP_SCALE - 3, 6, 6};
    SDL_RenderFillRect(renderer, &rect);
    vec2 next = {20 * cos(player->angle), 20 *sin(player->angle)};
    SDL_RenderDrawLine(renderer, player->pos.x * MINIMAP_SCALE, player->pos.y * MINIMAP_SCALE, player->pos.x * MINIMAP_SCALE+ next.x, player->pos.y * MINIMAP_SCALE+ next.y);
}

void render(SDL_Renderer *renderer, Player *player) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    draw_scene(renderer, player);
    draw_player_icon(renderer, player);
    draw_minimap(renderer);
    SDL_RenderPresent(renderer);
}