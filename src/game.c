#include <SDL3/SDL.h>
#include <stdio.h>

#include "game.h"
#include "map.h"
#include "player.h"
#include "scene.h"
#include "food.h"

void Game_Init(Game *game, Scene *scenes[], int scenes_num) {
	for (int i = 0; i < scenes_num; i++) {
		game->scenes[i] = *scenes[i];
	}
}

void Game_Update(Game *game, SDL_Renderer *renderer, int *current_scene, float delta_time, SDL_Event event) {
	Game_Update_Scene(game, current_scene, renderer, delta_time, event);
}

void Game_Init_Scene(Game *game, int scene, SDL_Renderer *renderer, Player *player) {
	game->scenes[scene].player = player;
}

void Game_Update_Scene(Game *game, int *current_scene, SDL_Renderer *renderer, float delta_time, SDL_Event event) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	Scene scene = game->scenes[*current_scene];
	Player *player = scene.player;
	Map *map = scene.map;
	Food *food = scene.food;

	// Game logic
	Player_Update(player, map, event, delta_time);

	Map_Draw(map->textures[1], renderer, &map->layers[1], map->tile_size); // Ground bottom
	Map_Draw(map->textures[1], renderer, &map->layers[2], map->tile_size); // Ground top
	Map_Draw(map->textures[0], renderer, &map->layers[3], map->tile_size); // Items bottom
	for (int i = 0; i < scene.food_count; i++)
	{
		Food_Draw(&food[i], renderer, map);
		Food_Eat(&food[i], player);
	}
	Player_Draw(player, renderer, delta_time);
	Map_Draw(map->textures[0], renderer, &map->layers[4], map->tile_size); // Items top
	
	SDL_RenderPresent(renderer);
}

void Game_End(Game *game) {
	for (int i = 0; i < SCENES_NUM; i++) {
		Scene_Destroy(&game->scenes[i]);
	}
}
