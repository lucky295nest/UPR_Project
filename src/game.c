#include <SDL3/SDL.h>
#include <stdio.h>

#include "game.h"
#include "map.h"
#include "scene.h"
#include "player.h"

void Game_Init(Game *game, Player *player, Scene *scenes[], int scenes_num) {
	for (int i = 0; i < scenes_num; i++)
	{
		game->scenes[i] = *scenes[i];
		game->scenes[i].player = player;
	}
}

void Game_Init_Scene(Game *game, int scene, SDL_Renderer *renderer) {
	// Global path init
	InitGlobalPath();

	Player *player = game->scenes[scene].player;

	// Player init
	Vector2 player_pos = {2.5f * 16, 2.5f * 16};
	Vector2 player_size = {24, 24};

	Vector2 player_hitbox_offset = {8, 15};
	Vector2 player_hitbox_size = {10, 8};

	char player_img_path[512];
	snprintf(player_img_path, sizeof(player_img_path), "%splayer.png", global_assets_path);
	Player_Init(player, renderer, 6, 6, 0.45f, 0.075f, player_pos, player_size, player_img_path, player_hitbox_offset, player_hitbox_size);
}

void Game_Update_Scene(Game *game, int scenes_num, SDL_Renderer *renderer, float delta_time, SDL_Event event) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	Player *player = game->scenes[scenes_num].player;
	Map *map = game->scenes[scenes_num].map;

	// Game logic
	Player_Update(player, map, event, delta_time);

	Map_Draw(map->textures[1], renderer, &map->layers[1], map->tile_size); // Ground bottom
	Map_Draw(map->textures[1], renderer, &map->layers[2], map->tile_size); // Ground top
	Map_Draw(map->textures[0], renderer, &map->layers[3], map->tile_size); // Items bottom
	Player_Draw(player, renderer, delta_time);

	Map_Draw(map->textures[0], renderer, &map->layers[4], map->tile_size); // Items top

	SDL_RenderPresent(renderer);
}

void Game_End(Game *game) {

	for (int i = 0; i < SCENES_NUM; i++)
	{
		Scene_Destroy(&game->scenes[i]);
	}
}
