#ifndef FOOD_H
#define FOOD_H

#include <SDL3/SDL.h>

#include "global.h"
#include "map.h"
#include "player.h"

typedef struct {
	Vector2 position;
	int is_active;
	int is_super;
	SDL_Texture *texture;
} Food;

void Food_Init(Food *food, Vector2 position, SDL_Renderer *renderer, char *texture_normal, char *texture_super);
void Food_Eat(Food *food, Player *player);
void Food_Draw(Food *food, SDL_Renderer *renderer, Map *map);
void Food_OffSwitch(Food *food);
void Food_Destroy(Food *food);

#endif