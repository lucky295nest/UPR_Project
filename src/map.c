#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "map.h"

TileLayer ParseCSV(const char *path_to_csv) {
	TileLayer layer = {0};

	FILE *file = fopen(path_to_csv, "r");
	if (!file) {
		printf("Failed to open CSV %s", path_to_csv);
		return layer;
	}

	char line[1024];

	while (fgets(line, sizeof(line), file)) {
		if (layer.size.x == 0) {
			char *tmp = strdup(line);
			char *tok = strtok(tmp, ",");

			while (tok) {
				layer.size.x++;
				tok = strtok(NULL, ",");
			}
			free(tmp);
		}
		layer.size.y++;
	}

	rewind(file);

	layer.tiles = malloc(sizeof(int) * layer.size.x * layer.size.y);

	if (!layer.tiles) {
		fclose(file);
		return layer;
	}

	int y = 0;
	while (fgets(line, sizeof(line), file)) {
		int x = 0;
		char *tok = strtok(line, ",");
		while (tok && x < layer.size.x) {
			layer.tiles[y * layer.size.x + x] = atoi(tok);
			tok = strtok(NULL, ",");
			x++;
		}
		if (y >= layer.size.y) {
			break;
		}
		y++;
	}

	fclose(file);
	return layer;
}

void LoadTextures(Map *map, SDL_Renderer *renderer, char *path_img_1, char *path_img_2) {
	SDL_Surface *sur1 = IMG_Load(path_img_1);
	map->textures[0] = SDL_CreateTextureFromSurface(renderer, sur1);
	SDL_SetTextureScaleMode(map->textures[0], SDL_SCALEMODE_NEAREST);
	SDL_DestroySurface(sur1);

	SDL_Surface *sur2 = IMG_Load(path_img_2);
	map->textures[1] = SDL_CreateTextureFromSurface(renderer, sur2);
	SDL_SetTextureScaleMode(map->textures[1], SDL_SCALEMODE_NEAREST);
	SDL_DestroySurface(sur2);
}

void Map_Init(Map *map, SDL_Renderer *renderer, char *path_csv1, char *path_csv2, char *path_tiles1, char *path_tiles2, int t1_cols, int t2_cols) {
	map->layers[0] = ParseCSV(path_csv1);
	map->layers[1] = ParseCSV(path_csv2);
	map->layers[0].tileset_cols = t1_cols;
	map->layers[1].tileset_cols = t2_cols;
	LoadTextures(map, renderer, path_tiles1, path_tiles2);
}

void Map_Draw(SDL_Texture *texture, SDL_Renderer *renderer, TileLayer *layer, int tile_size) {
	for (int x = 0; x < layer->size.x; x++) {
		for (int y = 0; y < layer->size.y; y++) {
			int tile_id = layer->tiles[y * layer->size.x + x];

			if (tile_id == -1) {
				continue;
			}

			int texture_index = tile_id;

			int src_x = (texture_index % (layer->tileset_cols)) * tile_size;
			int src_y = (texture_index / (layer->tileset_cols)) * tile_size;

			SDL_FRect src = {src_x, src_y, tile_size, tile_size};

			SDL_FRect dst = {(x * tile_size), (y * tile_size), tile_size, tile_size};

			SDL_RenderTexture(renderer, texture, &src, &dst);
		}
	}
}

void Map_Destroy(Map *map) {
	for (int i = 0; i < LAYERS_NUM; i++) {
		if (map->layers[i].tiles != NULL) {
			free(map->layers[i].tiles);
			map->layers[i].tiles = NULL;
		}
	}

	for (int i = 0; i < TEXTURES_NUM; i++) {
		if (map->textures[i] != NULL) {
			SDL_DestroyTexture(map->textures[i]);
			map->textures[i] = NULL;
		}
	}
}