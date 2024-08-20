#include "../include/texture.h"

Texture *texture_create(int width, int height) {
    Texture *texture = malloc(sizeof(Texture));
    texture->width = width;
    texture->height = height;
    texture->bitmap = malloc(height * sizeof(RGB*));
    for (int i = 0; i < height; i++)
        texture->bitmap[i] = calloc(width, sizeof(RGB));
    return texture;
}

void texture_destroy(Texture *texture) {
    for (int i = 0; i < texture->height; i++)
        free(texture->bitmap[i]);
    free(texture->bitmap);
    free(texture);
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

    Texture *texture = texture_create(width, height);
    fseek(fptr, bitmap_offset, SEEK_SET);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fread(&texture->bitmap[y][x].blue, 1, 1, fptr);
            fread(&texture->bitmap[y][x].green, 1, 1, fptr);
            fread(&texture->bitmap[y][x].red, 1, 1, fptr);
            if (color_depth == 32)
                fseek(fptr, 1, SEEK_CUR);
        }
        fseek(fptr, width % 4, SEEK_CUR);
    }

    fclose(fptr);
    return texture; 
} 

Texture_vector* load_textures() {
    char *paths[] = {
        "../assets/background.bmp",
        "../assets/grass_floor.bmp",
        "../assets/dirt_wall.bmp",
        "../assets/stone_wall.bmp",
        "../assets/megumin.bmp",
        "../assets/wooden_floor.bmp",
        "../assets/dirt_floor.bmp"
    };
    int size = sizeof(paths) / sizeof(char *);
    Texture_vector *textures = malloc(sizeof(Texture_vector) + size * sizeof(Texture *));
    textures->size = size;

    for (int i = 0; i < textures->size; i++) {
        Texture *tmp = load_texture_from_image(paths[i]);
        if (tmp != NULL)
            textures->el[i] = tmp;
        else {
            fprintf(stderr, "failed to load %s\n", paths[i]);
            exit(-1);
        }
    }

    return textures;
}

void unload_textures(Texture_vector *textures) {
    for (int i = 0; i < textures->size; i++)
        if (textures->el[i])
            free(textures->el[i]);
    free(textures);
}