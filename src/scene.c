#include <SDL3/SDL.h>
#include <stdio.h>

#include "food.h"
#include "map.h"
#include "player.h"
#include "scene.h"

void Scene_CountFoodTiles(Scene *scene, Map *map, int layer) {
	TileLayer *food_layer = &map->layers[layer];

	int count = 0;
	for (int y = 0; y < food_layer->size.y; y++) {
		for (int x = 0; x < food_layer->size.x; x++) {
			int tile = food_layer->tiles[y * food_layer->size.x + x].tile_num;
			if (tile == 0) {
				count++;
			}
		}
	}

	scene->food_count = count;
	scene->food = malloc(sizeof(Food) * count);
}

void Scene_Init(Scene *scene, Player *player, SDL_Renderer *renderer, Map *map, Food **food, int food_layer, char *food_normal, char *food_super) {
	scene->map = map;
	scene->player = player;
	TileLayer *layer = &map->layers[food_layer];

	if (food != NULL) {
		Scene_CountFoodTiles(scene, map, food_layer);
		int i = 0;
		for (int y = 0; y < layer->size.y; y++) {
			for (int x = 0; x < layer->size.x; x++) {
				int tile = layer->tiles[y * layer->size.x + x].tile_num;
				if (tile >= 0) {
					Vector2 pos = {
						x * map->tile_size,
						y * map->tile_size,
					};

					Food_Init(&scene->food[i], pos, renderer, food_normal, food_super);

					i++;
				}
			}
		}
	}
}

void Scene_Destroy(Scene *scene) {
	if (scene->map != NULL) {
		Map_Destroy(scene->map);
		scene->map = NULL;
	}

	if (scene->food != NULL) {
		for (int i = 0; i < scene->food_count; i++) {
			Food_Destroy(&scene->food[i]);
		}

		free(scene->food);
		scene->food = NULL;
	}

	if (scene->player != NULL) {
		Player_Delete(scene->player);
		scene->player = NULL;
	}
}