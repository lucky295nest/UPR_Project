#include "scene.h"
#include "map.h"
#include "player.h"

void Scene_Init(Scene *scene, Player *player, Map *map) {
	scene->map = map;
	scene->player = player;
}

void Scene_Destroy(Scene *scene) {
	if (scene->map != NULL) {
		Map_Destroy(scene->map);
		scene->map = NULL;
	}
	if (scene->player != NULL) {
		Player_Delete(scene->player);
		scene->player = NULL;
	}
}