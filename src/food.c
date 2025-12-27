#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

#include "food.h"
#include "global.h"
#include "map.h"
#include "player.h"

void Food_SetTexture(char *path, SDL_Texture **texture, SDL_Renderer *renderer)
{
	SDL_Surface *sur = IMG_Load(path);

	if (!sur)
	{
		printf("Failed to load surface: %s\n", SDL_GetError());
		return;
	}

	*texture = SDL_CreateTextureFromSurface(renderer, sur);
	SDL_SetTextureScaleMode(*texture, SDL_SCALEMODE_NEAREST);
	SDL_DestroySurface(sur);

	if (!(*texture))
	{
		printf("Failed to create texture: %s\n", SDL_GetError());
	}
}

void Food_Eat(Food *food, Player *player)
{
	SDL_Rect hitbox_food = {
		(int)food->position.x+5,
		(int)food->position.y+5,
		(int)5,
		(int)5};

	SDL_Rect player_hitbox = {
		(int)player->hitbox_pos.x,
		(int)player->hitbox_pos.y,
		(int)player->hitbox_size.x,
		(int)player->hitbox_size.y};

	if (SDL_HasRectIntersection(&hitbox_food, &player_hitbox) && food->is_active)
	{
		Food_OffSwitch(food);
		player->score++;
		printf("player score: %d\n", player->score);
	}
}

void Food_Init(Food *food, Vector2 position, SDL_Renderer *renderer, char *texture_normal, char *texture_super)
{
	food->is_active = 1;
	food->position = position;
	food->is_super = (SDL_rand(100) < 5);
	if (food->is_super)
	{
		Food_SetTexture(texture_super, &food->texture, renderer);
	}
	else
	{
		Food_SetTexture(texture_normal, &food->texture, renderer);
	}
}

void Food_Draw(Food *food, SDL_Renderer *renderer, Map *map)
{
	if (food->is_active)
	{
		SDL_FRect srcrect = {0, 0, map->tile_size, map->tile_size};
	
		SDL_FRect dstrect = {food->position.x, food->position.y, map->tile_size, map->tile_size};
	
		SDL_RenderTexture(renderer, food->texture, &srcrect, &dstrect);
	}
}

void Food_OffSwitch(Food *food)
{
	if (food->is_active == 1)
	{
		food->is_active = 0;
	}
}

void Food_Destroy(Food *food)
{
	if (food->texture != NULL)
	{
		SDL_DestroyTexture(food->texture);
		food->texture = NULL;
	}
}