#include "../include/render.h"

double deg_to_rad(double angle) {
    return angle * PI / 180;
}

Texture* texture_create(int width, int height, RGB bitmap[height][width]) {
    Texture *texture = malloc(sizeof(Texture));
    texture->width = width;
    texture->height = height;
    texture->bitmap = malloc(height * sizeof(RGB*));
    for (int i = 0; i < height; i++) {
        texture->bitmap[i] = malloc(width * sizeof(RGB));
        memcpy(texture->bitmap[i], bitmap[i], width * sizeof(RGB));
    }
    return texture;
}

void texture_destroy(Texture *texture) {
    for (int i = 0; i < texture->height; i++)
        free(texture->bitmap[i]);
    free(texture->bitmap);
    free(texture);
}

void draw_line(SDL_Renderer *renderer, int x, int y, int width, int height, RGB color) {
	SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, 255);
	SDL_Rect rect = {x, y, width, height};
	SDL_RenderFillRect(renderer, &rect);
}

void draw_texture_line(SDL_Renderer *renderer, int x, int y, int width, int height, Texture *texture, int tx) {
    int segment_height = ceil((double)height / texture->height);
    double step = (double)height / texture->height;
    for (int ty = 0; ty < texture->height; ty++)
        draw_line(renderer, x, y + ty * step, 1, segment_height, texture->bitmap[ty][tx]);
}

RGB decode_color(int id) {
    RGB color = {0, 0, 0};
    switch(id) {
        case 2:
            color.red = 128;
            color.green = 128;
            color.blue = 128;
            break;
        case 3:
            color.red = 255;
            color.green = 255;
            color.blue = 255;
            break;
        case 4:
            color.red = 255;
            break;
        case 5:
            color.green = 255;
            break;
        case 6:
            color.blue = 255;
            break;
        case 7:
            color.red = 255;
            color.green = 255;
            break;
        case 8:
            color.green = 255;
            color.blue = 255;
            break;
        case 9:
            color.red = 255;
            color.blue = 255;
            break;
        case 10:
            color.red = 255;
            color.green = 192;
            break;
    }
    return color;
}

Texture *decode_texture(int id) {
    Texture *texture = NULL;
    switch(id) {
        case 11:
            texture = texture_create(TEXTURE_BRICK_WALL);
    }
    return texture;
}

void draw_scene(SDL_Renderer *renderer, Player *player) {
    Texture *brick_wall = texture_create(TEXTURE_BRICK_WALL);
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
        vec2 wallID = {0, 0};
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
            wallID.x = map[(int)rayPos.y][(int)rayPos.x];
        while (!wallID.x) {
            rayPos = vec2_add(&rayPos, &rayStep);
            if (rayPos.x > 0 && rayPos.x < MAP_WIDTH && rayPos.y > 0 && rayPos.y < MAP_HEIGHT)
                wallID.x = map[(int)rayPos.y][(int)rayPos.x];
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
            wallID.y = map[(int)rayPos.y][(int)rayPos.x];
        while (!wallID.y) {
            rayPos = vec2_add(&rayPos, &rayStep);
            if (rayPos.x > 0 && rayPos.x < MAP_WIDTH && rayPos.y > 0 && rayPos.y < MAP_HEIGHT)
                wallID.y = map[(int)rayPos.y][(int)rayPos.x];
            else {
                rayPos = vec2_multiply(&rayPos, 10000);
                break;
            }
        }
        dist.y = vec2_dist(&rayPos, &player->pos);

        double minDist = dist.x;
        int closestWallID = wallID.x;
        if (dist.x > dist.y) {
            minDist = dist.y;
            closestWallID = wallID.y;
        } else
            rayPos = tmp;
        minDist *= cos(player->angle - rayAngle);

        int wallHeight = WINDOW_HEIGHT / minDist; 

        draw_line(renderer, i, 0, 1, (WINDOW_HEIGHT - wallHeight) / 2, COLOR_SKY);
        if (closestWallID > 10) {
            Texture *texture = brick_wall;
            int tx = fmod((rayPos.x + rayPos.y) * 8, 8);
            draw_texture_line(renderer, i, (WINDOW_HEIGHT - wallHeight) / 2, 1, wallHeight, texture, tx);
        } else {
            RGB color = decode_color(closestWallID);
            draw_line(renderer, i, (WINDOW_HEIGHT - wallHeight) / 2, 1, wallHeight, color);
        }
        if (wallHeight > WINDOW_HEIGHT)
            wallHeight = WINDOW_HEIGHT;
        draw_line(renderer, i, (WINDOW_HEIGHT + wallHeight) / 2, 1, (WINDOW_HEIGHT - wallHeight), COLOR_GROUND);

        rayAngle += angleStep;
    }
    texture_destroy(brick_wall);
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