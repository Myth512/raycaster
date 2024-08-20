#include "../include/render.h"
#include <time.h>

double deg_to_rad(double angle) {
    return angle * PI / 180;
}

void draw_line(RGB frame_buffer[WINDOW_HEIGHT][WINDOW_WIDTH], int x, int y, int width, int height, RGB color) {
    for (int v = 0; v < height; v++) {
        for (int u = 0; u < width; u++) {
            if (x + u >= 0 && x + u < WINDOW_WIDTH && y + v >= 0 && y + v < WINDOW_HEIGHT) 
                frame_buffer[y + v][x + u] = color;
        }
    }
}

void draw_texture_line(RGB frame_buffer[WINDOW_HEIGHT][WINDOW_WIDTH], Texture *texture, int x, int y, int width, int height, int u) {
    int segment_height = ceil((double)height / texture->height);
    double step = (double)height / texture->height;
    for (int v = 0; v < texture->height; v++)
        draw_line(frame_buffer, x, y + v * step, SCALE, segment_height, texture->bitmap[v][u]);
}

RGB decode_color(int id) {
    RGB color = {  0,  0,  0}; // black
    switch(id) {
        case 2:
            color = (RGB){128, 128, 128}; // gray
            break;
        case 3:
            color = (RGB){255, 255, 255}; // white
            break;
        case 4:
            color = (RGB){255,   0,   0}; // red
            break;
        case 5:
            color = (RGB){  0, 255,   0}; // green
            break;
        case 6:
            color = (RGB){  0,   0, 255}; // blue
            break;
        case 7:
            color = (RGB){255, 255,   0}; // yellow
            break;
        case 8:
            color = (RGB){  0, 255, 255}; // cyan
            break;
        case 9:
            color = (RGB){255,   0, 255}; // purple
            break;
    }
    return color;
}

Texture* decode_texture(Texture_vector *loaded_textures, int id) {
    if (id >= TEXTURE_COUNT + 10) {
        fprintf(stderr, "invalid texture id: %d\n", id);
        exit(-1);
    }
    return loaded_textures->el[id - 10];
}

void draw_scene(RGB frame_buffer[WINDOW_HEIGHT][WINDOW_WIDTH], Texture_vector *loaded_textures, Player *player) {
    double angleStep = deg_to_rad(FOV) / WINDOW_WIDTH;
    double rayAngle = player->angle - deg_to_rad(FOV) / 2;
    vec2 rayPos = {0, 0};
    for (int x = 0; x < WINDOW_WIDTH; x += SCALE) {
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
            wallID.x = wall_map[(int)rayPos.y][(int)rayPos.x];
        while (!wallID.x) {
            rayPos = vec2_add(&rayPos, &rayStep);
            if (rayPos.x > 0 && rayPos.x < MAP_WIDTH && rayPos.y > 0 && rayPos.y < MAP_HEIGHT)
                wallID.x = wall_map[(int)rayPos.y][(int)rayPos.x];
            else
                break;
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
            wallID.y = wall_map[(int)rayPos.y][(int)rayPos.x];
        while (!wallID.y) {
            rayPos = vec2_add(&rayPos, &rayStep);
            if (rayPos.x > 0 && rayPos.x < MAP_WIDTH && rayPos.y > 0 && rayPos.y < MAP_HEIGHT)
                wallID.y = wall_map[(int)rayPos.y][(int)rayPos.x];
            else
                break;
        }
        dist.y = vec2_dist(&rayPos, &player->pos);

        double minDist = dist.x;
        int closestWallID = wallID.x;
        if (dist.x > dist.y) {
            minDist = dist.y;
            closestWallID = wallID.y;
        } else
            rayPos = tmp;
        minDist *= cos(rayAngle - player->angle);

        int wallHeight = WINDOW_HEIGHT / minDist; 
        int wallOffset = (WINDOW_HEIGHT - wallHeight) / 2;

        Texture *background = loaded_textures->el[0];
        int u = (background->width / (2*PI)) * rayAngle;

        draw_texture_line(frame_buffer, background, x, WINDOW_HEIGHT / 2, SCALE, WINDOW_HEIGHT / 2, u);

        if (closestWallID >= 10) {
            Texture *texture = decode_texture(loaded_textures, closestWallID);
            u = fmod((rayPos.x + rayPos.y) * texture->width, texture->width);
            draw_texture_line(frame_buffer, texture, x, wallOffset, SCALE, wallHeight, u);
        } else {
            RGB color = decode_color(closestWallID);
            draw_line(frame_buffer, x, wallOffset, SCALE, wallHeight, color);
        }


        for (int y = 0; y < wallOffset; y++) {
            double dist = (double)y / (WINDOW_HEIGHT / 2 - y) + 1;
            dist /= cos(player->angle - rayAngle);
            rayPos.x = player->pos.x + dist * cos(rayAngle);
            rayPos.y = player->pos.y + dist * sin(rayAngle);
            int floorID = floor_map[(int)rayPos.y][(int)rayPos.x];
            Texture *floor = decode_texture(loaded_textures, floorID);

            u = fmod(rayPos.x * floor->width, floor->width);
            int v = fmod(rayPos.y * floor->height, floor->height);
            frame_buffer[y][x] = floor->bitmap[v][u];
        }

        // for (int y = 100; x == WINDOW_WIDTH / 2 && y < wallOffset && y < 101; y++) {
        //     double dist = (double)y / (WINDOW_HEIGHT / 2 - y);
        //     printf("d: %f\n", dist);
        //     u = fmod((dist) * floor->width, floor->width);
        //     int v = fmod((dist) * floor->height, floor->height);
        //     frame_buffer[y][x] = (RGB){255, 0, 0};
        // }

        rayAngle += angleStep * SCALE;
    }
}

void render(GLFWwindow *window, RGB frame_buffer[WINDOW_HEIGHT][WINDOW_WIDTH], Texture_vector *loaded_textures, Player *player) {
    glClear(GL_COLOR_BUFFER_BIT);

    draw_scene(frame_buffer, loaded_textures, player);

    glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, frame_buffer);

    glfwSwapBuffers(window);
}