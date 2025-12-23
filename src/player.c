#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "player.h"

// Private
void SetTexture(SDL_Texture **texture, char *path, SDL_Renderer *renderer)
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

Vector2_int ChooseSprite(Player *player)
{
    Vector2_int result;

    switch (player->state)
    {
    case IDLE_R:
        player->frame_duration = player->duration_idle;
        result.x = 2 + (player->current_frame % 2);
        result.y = 0;
        break;
    case IDLE_L:
        player->frame_duration = player->duration_idle;
        result.x = 0 + (player->current_frame % 2);
        result.y = 0;
        break;
    case WALKING_R:
        player->frame_duration = player->duration_walking;
        result.x = 0 + (player->current_frame % 8);
        result.y = 2;
        break;
    case WALKING_L:
        player->frame_duration = player->duration_walking;
        result.x = 0 + (player->current_frame % 8);
        result.y = 1;
        break;
    default:
        printf("Something went wrong with player state\n");
        break;
    }

    result.x *= player->size.x;
    result.y *= player->size.y;
    return result;
}

// Public
void Player_Init(Player *player, SDL_Renderer *renderer, int lives, float speed, float idle_speed, float walking_speed, Vector2 pos, Vector2 size, char *img_path)
{
    player->lives = lives;
    player->speed = speed * 10;
    player->size = size;
    player->pos = pos;
    player->score = 0;

    player->dir = RIGHT;
    player->state = IDLE_R;

    player->anim_timer = 0;
    player->total_frames = 8;
    player->duration_idle = idle_speed;
    player->duration_walking = walking_speed;

    player->frame_duration = 0;
    player->current_frame = 0;

    SetTexture(&player->texture, img_path, renderer);
}

void Player_Draw(Player *player, SDL_Renderer *renderer, float delta_time)
{
    Vector2_int grid_point = ChooseSprite(player);

    SDL_FRect srcrect = {
        grid_point.x,
        grid_point.y,
        player->size.x,
        player->size.y};

    SDL_FRect dstrect = {
        player->pos.x,
        player->pos.y,
        player->size.x,
        player->size.y};

    SDL_RenderTexture(renderer, player->texture, &srcrect, &dstrect);
}

void Player_Update(Player *player, SDL_Event event, float delta_time)
{
    // Animation timing
    player->anim_timer += delta_time;

    if (player->anim_timer >= player->frame_duration)
    {
        player->current_frame = (player->current_frame + 1) % player->total_frames;
        player->anim_timer -= player->frame_duration;
    }

    // Movement
    const bool *state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
    {
        player->pos.y -= player->speed * delta_time;
        if (player->dir == LEFT)
        {
            player->state = WALKING_L;
        }
        else
        {
            player->state = WALKING_R;
        }
    }
    else if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
    {
        player->pos.y += player->speed * delta_time;
        if (player->dir == LEFT)
        {
            player->state = WALKING_L;
        }
        else
        {
            player->state = WALKING_R;
        }
    }
    else if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
    {
        player->dir = RIGHT;
        player->pos.x += player->speed * delta_time;
        player->state = WALKING_R;
    }
    else if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
    {
        player->dir = LEFT;
        player->pos.x -= player->speed * delta_time;
        player->state = WALKING_L;
    }
    else
    {
        if (player->dir == LEFT)
        {
            player->state = IDLE_L;
        }
        else
        {
            player->state = IDLE_R;
        }
    }
}

void Player_Delete(Player *player)
{
    if (!player->texture)
    {
        SDL_DestroyTexture(player->texture);
        player->texture = NULL;
    }
}