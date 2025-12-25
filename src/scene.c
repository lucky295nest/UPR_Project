#include "scene.h"
#include "food.h"
#include "map.h"
#include "player.h"

void Scene_Init(Scene *scene, Player *player, Map *map, Food *food) {
	scene->map = map;
	scene->player = player;
	scene->food = food;
}

void Scene_Destroy(Scene *scene) {
	if (scene->map != NULL) {
		Map_Destroy(scene->map);
		scene->map = NULL;
	}

	if (scene->food != NULL) {
		Food_Destroy(scene->food);
		scene->food = NULL;
	}

	if (scene->player != NULL) {
		Player_Delete(scene->player);
		scene->player = NULL;
	}
}