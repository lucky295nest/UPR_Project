#ifndef GAME_H
#define GAME_H

#include "global.h"
#include "map.h"
#include "player.h"
#include "scene.h"

#define SCENES_NUM 10

typedef struct
{
	Scene scenes[SCENES_NUM];
} Game;

void Game_Init(Game *game, Player *player, Scene *scenes[], int scenes_num);
void Game_Init_Scene(Game *game, int scene, SDL_Renderer *renderer);
void Game_Update_Scene(Game *game, int scenes_num, SDL_Renderer *renderer, float delta_time, SDL_Event event);
void Game_End(Game *game);

#endif