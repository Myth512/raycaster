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

Texture** load_textures() {
    Texture **textures = calloc(TEXTURE_COUNT, sizeof(Texture*));
    Texture *tmp;

    tmp = load_texture_from_image("../assets/background.bmp");
    if (tmp != NULL)
        textures[0] = tmp;
    else {
        fprintf(stderr, "failed to load background.bmp\n");
        exit(-1);
    }
    tmp = load_texture_from_image("../assets/image.bmp");
    if (tmp != NULL)
        textures[2] = tmp;
    else {
        fprintf(stderr, "failed to load image.bmp\n");
        exit(-1);
    }
    tmp = load_texture_from_image("../assets/gradient.bmp");
    if (tmp != NULL)
        textures[3] = tmp;
    else {
        fprintf(stderr, "failed to load gradient.bmp\n");
        exit(-1);
    }
    tmp = load_texture_from_image("../assets/megumin.bmp");
    if (tmp != NULL)
        textures[4] = tmp;
    else {
        fprintf(stderr, "failed to load megumin.bmp\n");
        exit(-1);
    }

    return textures;
}

void unload_textures(Texture **textures) {
    for (int i = 0; i < TEXTURE_COUNT; i++)
        free(textures[i]);
    free(textures);
}