#include <SDL3/SDL.h>
#include <stdio.h>

#include "global.h"
#include "map.h"
#include "player.h"

#define FPS 60
#define FRAME_TARGET_MS (1000.0f / FPS)

int main(int argc, char *argv[]) {
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return 1;
	}

	if (!SDL_CreateWindowAndRenderer("PacMan", 800, 800, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
		fprintf(stderr, "Create Window Error: %s\n", SDL_GetError());
		return 1;
	}

	SDL_SetRenderLogicalPresentation(renderer, 30 * 16, 20 * 16, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	// Global path init
	InitGlobalPath();

	// Game init
	Player player;
	Vector2 player_pos = {50, 50};
	Vector2 player_size = {24, 24};

	char player_img_path[512];
	snprintf(player_img_path, sizeof(player_img_path), "%splayer.png", global_assets_path);
	Player_Init(&player, renderer, 6, 6, 0.45f, 0.1f, player_pos, player_size, player_img_path);

	// Map stuff
	Map map;

	// Map tilesets
	char tilesets_buf[2][FILE_PATH_SIZE];
	snprintf(tilesets_buf[0], sizeof(tilesets_buf[0]), "%s%s", global_assets_path, "items.png");
	snprintf(tilesets_buf[1], sizeof(tilesets_buf[1]), "%s%s", global_assets_path, "tilemap.png");

	// Map layouts
	char layouts_buf[5][FILE_PATH_SIZE];
	snprintf(layouts_buf[0], sizeof(layouts_buf[0]), "%s%s", global_assets_path, "level1/level-1_collisions.csv");
	snprintf(layouts_buf[1], sizeof(layouts_buf[1]), "%s%s", global_assets_path, "level1/level-1_ground_bottom.csv");
	snprintf(layouts_buf[2], sizeof(layouts_buf[2]), "%s%s", global_assets_path, "level1/level-1_ground_top.csv");
	snprintf(layouts_buf[3], sizeof(layouts_buf[3]), "%s%s", global_assets_path, "level1/level-1_items_bottom.csv");
	snprintf(layouts_buf[4], sizeof(layouts_buf[4]), "%s%s", global_assets_path, "level1/level-1_items_top.csv");

	int cols[5] = {
		1,	// Collisions
		10, // Ground bottom
		10, // Ground top
		12, // Items bottom
		12}; // Items top

	char *layouts[5];
	for (int i = 0; i < 5; i++) {
		layouts[i] = layouts_buf[i];
	}

	char *tilesets[2];
	for (int i = 0; i < 2; i++) {
		tilesets[i] = tilesets_buf[i];
	}

	Map_Init(&map, renderer, 5, 2, layouts, tilesets, cols);

	// Setup timing variables
	uint64_t last_time = SDL_GetTicksNS();
	float delta_time = 0.0f;

	// Game loop
	int done = 0;
	while (!done) {
		// Calculate delta time
		uint64_t current_time = SDL_GetTicksNS();
		delta_time = (float)((current_time - last_time) / 1000000000.0f);
		last_time = current_time;

		// Framerate stuff
		uint64_t start_ticks = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				done = 1;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Game logic
		Player_Update(&player, event, delta_time);

		Map_Draw(map.textures[1], renderer, &map.layers[1], 16); // Ground bottom
		Map_Draw(map.textures[1], renderer, &map.layers[2], 16); // Ground top
		Map_Draw(map.textures[0], renderer, &map.layers[3], 16); // Items bottom
		Player_Draw(&player, renderer, delta_time);
		Map_Draw(map.textures[0], renderer, &map.layers[4], 16); // Items top

		SDL_RenderPresent(renderer);

		// Framerate stuff
		uint64_t end_ticks = SDL_GetTicks();
		uint64_t elapsed_ticks = end_ticks - start_ticks;

		if (elapsed_ticks < FRAME_TARGET_MS) {
			SDL_Delay((uint32_t)(FRAME_TARGET_MS - elapsed_ticks));
		}
	}

	Map_Destroy(&map);
	Player_Delete(&player);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}