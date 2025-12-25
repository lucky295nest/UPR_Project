#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

#include "food.h"
#include "global.h"
#include "map.h"

void Food_SetTexture(char *path, SDL_Texture **texture, SDL_Renderer *renderer) {
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

void Food_Init(Food *food, Vector2 position, SDL_Renderer *renderer, char *texture_normal, char *texture_super) {
	food->is_active = 1;
	food->position = position;
	food->is_super = (SDL_rand(100) < 5);
	if (food->is_super) {
		Food_SetTexture(texture_super, &food->texture, renderer);
	} else {
		Food_SetTexture(texture_normal, &food->texture, renderer);
	}
}

void Food_Draw(Food *food, SDL_Renderer *renderer, Map *map) {
	SDL_FRect srcrect = {0, 0, map->tile_size, map->tile_size};

	SDL_FRect dstrect = {food->position.x, food->position.y, map->tile_size, map->tile_size};

	SDL_RenderTexture(renderer, food->texture, &srcrect, &dstrect);
}

void Food_OnOffSwitch(Food *food) {
	if (food->is_active == 1) {
		food->is_active = 0;
	} else {
		food->is_active = 1;
	}
}

void Food_Destroy(Food *food) {
	if (food->texture != NULL) {
		SDL_DestroyTexture(food->texture);
		food->texture = NULL;
	}
}