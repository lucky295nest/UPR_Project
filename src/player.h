#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>

#include "SDL3/SDL.h"
#include "global.h"

enum player_state
{
    IDLE_L = 0,
    IDLE_R,
    WALKING_L,
    WALKING_R
};

enum player_direction
{
    LEFT = 0,
    RIGHT
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

    float anim_timer;
    float duration_idle;
    float duration_walking;
    float frame_duration;
    int current_frame;
    int total_frames;
} Player;

void Player_Init(Player *player, SDL_Renderer *renderer, int lives, float speed, float idle_speed, float walking_speed, Vector2 pos, Vector2 size, char *img_path);
void Player_Draw(Player *player, SDL_Renderer *renderer, float delta_time);
void Player_Update(Player *player, SDL_Event event, float delta_time);
void Player_Delete(Player *player);

#endif