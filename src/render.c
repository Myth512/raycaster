#include "../include/render.h"

double deg_to_rad(double angle) {
    return angle * PI / 180;
}

enum TextureID{
    TEXTURE_BRICK_WALL_ID,
    TEXTURE_STONE_WALL_ID,
    TEXTURE_GRADIENT_ID,
    TEXTURE_COUNT
};

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

Texture *texture_create_default(int width, int height) {
    printf("im here4\n");
    Texture *texture = malloc(sizeof(Texture));
    texture->width = width;
    texture->height = height;
    texture->bitmap = malloc(height * sizeof(RGB*));
    for (int i = 0; i < height; i++)
        texture->bitmap[i] = calloc(width, sizeof(RGB));
    return texture;
}

Texture* load_texture_from_image(char *path) {
    FILE *fptr;
    fptr = fopen(path, "rb");

    if (fptr == NULL)
        return 0;

    unsigned char signature[2];
    fread(signature, 2, sizeof(char), fptr);
    if (signature[0] != 'B' || signature[1] != 'M') {
        fclose(fptr);
        return 0;
    }

    int size;
    fread(&size, 1, sizeof(int), fptr);

    int bitmap_offset;
    fseek(fptr, 10, SEEK_SET);
    fread(&bitmap_offset, 1, sizeof(int), fptr);

    int width;
    fseek(fptr, 18, SEEK_SET);
    fread(&width, 1, sizeof(int), fptr);

    int height;
    fread(&height, 1, sizeof(int), fptr);

    short color_depth;
    fseek(fptr, 28, SEEK_SET);
    fread(&color_depth, 1, sizeof(short), fptr);

    printf("im here3\n");
    Texture *texture = texture_create_default(width, height);
    printf("im here5 %p\n", texture);
    fseek(fptr, bitmap_offset, SEEK_SET);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fread(&texture->bitmap[y][x].blue, 1, 1, fptr);
            fread(&texture->bitmap[y][x].green, 1, 1, fptr);
            fread(&texture->bitmap[y][x].red, 1, 1, fptr);
            fseek(fptr, 1, SEEK_CUR);
        }
    }

    fclose(fptr);
    return texture; 
} 


Texture** load_textures() {
    Texture **textures = calloc(4, sizeof(Texture*));
    textures[0] = texture_create(TEXTURE_BRICK_WALL);
    textures[1] = texture_create(TEXTURE_STONE_WALL);
    textures[2] = texture_create(TEXTURE_GRADIENT);
    printf("im here1\n");
    Texture *tmp = load_texture_from_image("../assets/image.bmp");
    if (tmp != NULL)
        textures[3] = tmp;
    else {
        printf("could not load image.bmp\n");
        exit(-1);
    }
    printf("im here2 %p\n", textures[3]);
    return textures;
}

void unload_textures(Texture **textures) {
    for (int i = 0; i < TEXTURE_COUNT; i++)
        free(textures[i]);
    free(textures);
}


void texture_destroy(Texture *texture) {
    for (int i = 0; i < texture->height; i++)
        free(texture->bitmap[i]);
    free(texture->bitmap);
    free(texture);
}

void draw_line(SDL_Renderer *renderer, int x, int y, int width, int height, RGB color) {
    if (width <= 0 || height <= 0)
        return;
	SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, 255);
	SDL_Rect rect = {x, y, width, height};
	SDL_RenderFillRect(renderer, &rect);
}

void draw_texture_line(SDL_Renderer *renderer, int x, int y, int width, int height, Texture *texture, int tx) {
    int segment_height = ceil((double)height / texture->height);
    double step = (double)height / texture->height;
    for (int ty = 0; ty < texture->height; ty++)
        draw_line(renderer, x, y + ty * step, SCALE, segment_height, texture->bitmap[texture->height - ty - 1][texture->width - tx - 1]);
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

void draw_scene(SDL_Renderer *renderer, Texture **loaded_textures, Player *player) {
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
        int wallOffset = (WINDOW_HEIGHT - wallHeight) / 2;
        int floorOffset = (WINDOW_HEIGHT + wallHeight) / 2;
        int floorHeight = (WINDOW_HEIGHT - wallHeight);

        draw_line(renderer, x, 0, SCALE, wallOffset, COLOR_SKY);
        if (closestWallID > 10) {
            Texture *texture = loaded_textures[closestWallID-11];
            int tx = fmod((rayPos.x + rayPos.y) * texture->width, texture->width);
            draw_texture_line(renderer, x, wallOffset, SCALE, wallHeight, texture, tx);
        } else {
            RGB color = decode_color(closestWallID);
            draw_line(renderer, x, wallOffset, SCALE, wallHeight, color);
        }
        draw_line(renderer, x, floorOffset, SCALE, floorHeight, COLOR_GROUND);

        rayAngle += angleStep * SCALE;
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

void render(SDL_Renderer *renderer, Texture **loaded_textures, Player *player) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    draw_scene(renderer, loaded_textures, player);
    draw_player_icon(renderer, player);
    draw_minimap(renderer);
    SDL_RenderPresent(renderer);
}