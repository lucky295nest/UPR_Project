#ifndef SCENE_H
#define SCENE_H

#include "map.h"
#include "player.h"
#include "food.h"

typedef struct
{
	Player *player;
	Map *map;
	Food *food;
	int food_count;
} Scene;

void Scene_Init(Scene *scene, Player *player, SDL_Renderer *renderer, Map *map, Food **food, int food_layer, char *food_normal, char *food_super);
void Scene_Destroy(Scene *scene);

#endif