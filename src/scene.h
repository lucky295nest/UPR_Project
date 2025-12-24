#ifndef SCENE_H
#define SCENE_H

#include "map.h"
#include "player.h"

typedef struct
{
	Player *player;
	Map *map;
} Scene;

void Scene_Init(Scene *scene, Player *player, Map *map);
void Scene_Destroy(Scene *scene);

#endif