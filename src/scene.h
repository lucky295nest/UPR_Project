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
} Scene;

void Scene_Init(Scene *scene, Player *player, Map *map, Food *food);
void Scene_Destroy(Scene *scene);

#endif