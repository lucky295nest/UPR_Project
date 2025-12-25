#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <time.h>

#include "food.h"
#include "global.h"
#include "map.h"
#include "player.h"

int GetTilePositions(TileLayer *layer) {
	int count = 0;
	for (int i = 0; i < layer->size.x * layer->size.y; i++) {
		if (layer->tiles[i] != -1) {
			count++;
		}
	}

	if (count > 0) {
		layer->tile_positions = malloc(sizeof(Vector2) * count);
		int current_index = 0;
		for (int y = 0; y < layer->size.y; y++) {
			for (int x = 0; x < layer->size.x; x++) {
				int tile_id = layer->tiles[y * (int)layer->size.x + x];

				if (tile_id != -1) {
					layer->tile_positions[current_index].x = x;
					layer->tile_positions[current_index].y = y;
					current_index++;
				}
			}
		}
	}

	return count;
}

void Food_SetTexture(SDL_Texture **texture, char *path, SDL_Renderer *renderer) {
	SDL_Surface *sur = IMG_Load(path);

	if (!sur) {
		printf("Failed to load surface: %s\n", SDL_GetError());
		return;
	}

	*texture = SDL_CreateTextureFromSurface(renderer, sur);
	SDL_SetTextureScaleMode(*texture, SDL_SCALEMODE_NEAREST);
	SDL_DestroySurface(sur);

	if (!(*texture)) {
		printf("Failed to create texture: %s\n", SDL_GetError());
	}
}

void Food_Init(Food *food, int food_tilesize, int food_layer, Map *map, Player *player, SDL_Renderer *renderer, char *path_normal, char *path_super) {
	int food_count = GetTilePositions(&map->layers[food_layer]);
	for (int i = 0; i < food_count && i < MAX_FOOD; i++) {
		food[i].position = map->layers[food_layer].tile_positions[i];
		food[i].tile_size = food_tilesize;
		food[i].is_super = (rand() % 100 < 20);
		Food_SetTexture(&food[i].normal_texture, path_normal, renderer);
	}
}

void Food_Draw(Food *food, SDL_Renderer *renderer) {
	for (int i = 0; i < MAX_FOOD; i++) {
		SDL_FRect srcrect = {
			0,
			0,
			16,
			16};

		SDL_FRect dstrect = {
			0,
			0,
			16,
			16};

		SDL_RenderTexture(renderer, food[i].normal_texture, &srcrect, &dstrect);
	}
}

void Food_Destroy(Food *food) {
	SDL_DestroyTexture(food->normal_texture);
	SDL_DestroyTexture(food->super_texture);
}