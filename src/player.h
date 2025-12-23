#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>

#include "SDL3/SDL.h"
#include "global.h"
#include "map.h"

enum player_state {
	IDLE_L = 0,
	IDLE_R,
	WALKING_L,
	WALKING_R
};

enum player_direction {
	NONE = 0,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

typedef struct
{
	int lives;
	float speed;
	int score;
	Vector2 size;
	Vector2 pos;
	SDL_Texture *texture;
	enum player_state state;
	enum player_direction dir;

	enum player_direction move_dir;

	Vector2 hitbox_pos;
	Vector2 hitbox_offset;
	Vector2 hitbox_size;

	float anim_timer;
	float duration_idle;
	float duration_walking;
	float frame_duration;
	int current_frame;
	int total_frames;
} Player;

void Player_Init(Player *player, SDL_Renderer *renderer, int lives, float speed, float idle_speed, float walking_speed, Vector2 pos, Vector2 size, char *img_path, Vector2 hitbox_offset, Vector2 hitbox_size);
void Player_Draw(Player *player, SDL_Renderer *renderer, float delta_time);
void Player_Update(Player *player, Map *map, SDL_Event event, float delta_time);
void Player_Delete(Player *player);

#endif