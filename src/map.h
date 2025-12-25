#ifndef MAP_H
#define MAP_H

#include <SDL3/SDL.h>

#include "global.h"

#define LAYERS_NUM 10
#define TEXTURES_NUM 10

typedef struct
{
	int tile_num;
	Vector2 tile_position;
} Tile;


typedef struct
{
	Vector2_int size;
	int tileset_cols;
	Tile *tiles;
	int tile_count;
} TileLayer;

typedef struct
{
	int tile_size;
	SDL_Texture *textures[TEXTURES_NUM];
	TileLayer layers[LAYERS_NUM];
} Map;

void Map_Init(Map *map, SDL_Renderer *renderer, int tile_size, int layouts_num, int tilesets_num, char *layouts[], char *tilesets[], int cols[]);
void Map_Draw(SDL_Texture *texture, SDL_Renderer *renderer, TileLayer *layer, int tile_size);
void Map_Destroy(Map *map);

#endif