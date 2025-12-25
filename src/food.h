#ifndef FOOD_H
#define FOOD_H

#include <SDL3/SDL.h>

#include "map.h"
#include "player.h"
#include "global.h"

#define MAX_FOOD 128

typedef struct
{
	Vector2 position;
	int is_super;
	SDL_Texture *normal_texture;
	SDL_Texture *super_texture;
	int tile_size;
} Food;

void Food_Init(Food *food, int food_tilesize, int food_layer, Map *map, Player *player, SDL_Renderer *renderer, char *path_normal, char *path_super);
void Food_Draw(Food *food, SDL_Renderer *renderer);
void Food_Destroy(Food *food);

#endif