#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "map.h"

// Private
TileLayer ParseCSV(const char *path_to_csv) {
	TileLayer layer = {0};

	FILE *file = fopen(path_to_csv, "r");
	if (!file) {
		printf("Failed to open CSV %s\n", path_to_csv);
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

void LoadTextures(SDL_Texture **texture, SDL_Renderer *renderer, char *path) {
	SDL_Surface *sur = IMG_Load(path);
	*texture = SDL_CreateTextureFromSurface(renderer, sur);
	SDL_SetTextureScaleMode(*texture, SDL_SCALEMODE_NEAREST);
	SDL_DestroySurface(sur);
}

// Public
void Map_Init(Map *map, SDL_Renderer *renderer, int tile_size, int layouts_num, int tilesets_num, char *layouts[], char *tilesets[], int cols[]) {
	map->tile_size = tile_size;

	for (int i = 0; i < layouts_num; i++) {
		map->layers[i].tile_positions = NULL;
		map->layers[i] = ParseCSV(layouts[i]);
		map->layers[i].tileset_cols = cols[i];
	}

	for (int i = 0; i < tilesets_num; i++) {
		LoadTextures(&map->textures[i], renderer, tilesets[i]);
	}
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
		if (map->layers[i].tile_positions != NULL) {
			free(map->layers[i].tile_positions);
			map->layers[i].tile_positions = NULL;
		}
	}

	for (int i = 0; i < TEXTURES_NUM; i++) {
		if (map->textures[i] != NULL) {
			SDL_DestroyTexture(map->textures[i]);
			map->textures[i] = NULL;
		}
	}
}