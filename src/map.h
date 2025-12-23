#ifndef MAP_H
#define MAP_H

#include <SDL3/SDL.h>

#include "global.h"

typedef struct
{
    Vector2_int size;
    int tileset_cols;
    int *tiles;
} TileLayer;

typedef struct
{
    SDL_Texture *texture_1;
    SDL_Texture *texture_2;
    TileLayer Layer_1;
    TileLayer Layer_2;
} Map;


void Map_Init(Map *map, SDL_Renderer *renderer, char *path_csv1, char *path_csv2, char *path_tiles1, char *path_tiles2, int t1_cols, int t2_cols);
void Map_Draw(SDL_Texture *texture, SDL_Renderer *renderer, TileLayer *layer, int tile_size);
void Map_Destroy(Map *map);

#endif