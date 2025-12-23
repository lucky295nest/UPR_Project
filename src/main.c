#include <SDL3/SDL.h>
#include <stdio.h>

#include "player.h"

#define FPS 60
#define FRAME_TARGET_MS (1000.0f / FPS)

int main(int argc, char *argv[])
{
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  if (!SDL_CreateWindowAndRenderer("PacMan", 800, 800, SDL_WINDOW_RESIZABLE, &window, &renderer))
  {
    fprintf(stderr, "Create Window Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_SetRenderLogicalPresentation(renderer, 24 * 16, 24 * 16, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  // Game init
  Player player;
  Vector2 player_pos = {50, 50};
  Vector2 player_size = {24, 24};
  Player_Init(&player, renderer, 6, 6, 0.45f, 0.1f, player_pos, player_size, "../assets/player.png");

  // Setup timing variables
  uint64_t last_time = SDL_GetTicksNS();
  float delta_time = 0.0f;

  // Game loop
  int done = 0;
  while (!done)
  {
    // Calculate delta time
    uint64_t current_time = SDL_GetTicksNS();
    delta_time = (float)((current_time - last_time) / 1000000000.0f);
    last_time = current_time;

    // Framerate stuff
    uint64_t start_ticks = SDL_GetTicks();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_EVENT_QUIT)
      {
        done = 1;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Game logic
    Player_Update(&player, event, delta_time);
    Player_Draw(&player, renderer, delta_time);

    SDL_RenderPresent(renderer);

    // Framerate stuff
    uint64_t end_ticks = SDL_GetTicks();
    uint64_t elapsed_ticks = end_ticks - start_ticks;

    if (elapsed_ticks < FRAME_TARGET_MS)
    {
      SDL_Delay((uint32_t)(FRAME_TARGET_MS - elapsed_ticks));
    }
  }

  Player_Delete(&player);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}