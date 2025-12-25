#include <SDL3/SDL.h>
#include <stdio.h>
#include <time.h>

#include "food.h"
#include "game.h"
#include "global.h"
#include "map.h"
#include "player.h"
#include "scene.h"

#define FPS 60
#define FRAME_TARGET_MS (1000.0f / FPS)

void Set_Player(Player *player, SDL_Renderer *renderer, Vector2 pos) {
	InitGlobalPath();

	// Player init
	pos.x -= 1;
	pos.y -= 1;

	Vector2 player_pos = {pos.x * 16, pos.y * 16};
	Vector2 player_size = {24, 24};

	Vector2 player_hitbox_offset = {8, 15};
	Vector2 player_hitbox_size = {10, 8};

	char player_img_path[512];
	snprintf(player_img_path, sizeof(player_img_path), "%splayer.png", global_assets_path);
	Player_Init(player, renderer, 6, 6, 0.45f, 0.075f, player_pos, player_size, player_img_path, player_hitbox_offset, player_hitbox_size);
}

// ------------------------------------------------------ Level 1 ------------------------------------------------------
void Level1_Init(Map *map, SDL_Renderer *renderer, Player *player, Scene *scene, Food *food) {
	InitGlobalPath();

	Vector2 player_position = {2.7, 3.5};
	Set_Player(player, renderer, player_position);

	// Map tilesets
	char tilesets_buf[4][FILE_PATH_SIZE];
	snprintf(tilesets_buf[0], sizeof(tilesets_buf[0]), "%s%s", global_assets_path, "items.png");
	snprintf(tilesets_buf[1], sizeof(tilesets_buf[1]), "%s%s", global_assets_path, "tilemap.png");
	snprintf(tilesets_buf[2], sizeof(tilesets_buf[2]), "%s%s", global_assets_path, "food.png");
	snprintf(tilesets_buf[3], sizeof(tilesets_buf[3]), "%s%s", global_assets_path, "super_food.png");

	// Map layouts
	char layouts_buf[6][FILE_PATH_SIZE];
	snprintf(layouts_buf[0], sizeof(layouts_buf[0]), "%s%s", global_assets_path, "level2/level-2_collisions.csv");
	snprintf(layouts_buf[1], sizeof(layouts_buf[1]), "%s%s", global_assets_path, "level2/level-2_ground_bottom.csv");
	snprintf(layouts_buf[2], sizeof(layouts_buf[2]), "%s%s", global_assets_path, "level2/level-2_ground_top.csv");
	snprintf(layouts_buf[3], sizeof(layouts_buf[3]), "%s%s", global_assets_path, "level2/level-2_items_bottom.csv");
	snprintf(layouts_buf[4], sizeof(layouts_buf[4]), "%s%s", global_assets_path, "level2/level-2_items_top.csv");
	snprintf(layouts_buf[5], sizeof(layouts_buf[5]), "%s%s", global_assets_path, "level2/level-2_food_placement.csv");

	int cols[6] = {
		1,	// Collisions
		10, // Ground bottom
		10, // Ground top
		12, // Items bottom
		12, // Items top
		1}; // Food placement

	char *layouts[6];
	for (int i = 0; i < 5; i++) {
		layouts[i] = layouts_buf[i];
	}

	char *tilesets[4];
	for (int i = 0; i < 4; i++) {
		tilesets[i] = tilesets_buf[i];
	}

	Map_Init(map, renderer, 16, 5, 2, layouts, tilesets, cols);
	Food_Init(food, 16, 5, map, player, renderer, tilesets[2], tilesets[3]);
	Scene_Init(scene, player, map, food);
}

// -------------------------------------------------------- Main --------------------------------------------------------
int main(int argc, char *argv[]) {
	srand(time(NULL));
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

	Game game;
	Player player;

	// ------------------------- Scene 2 -------------------------
	Scene scene_1;
	Map map_1;
	Food food_1[MAX_FOOD];
	Level1_Init(&map_1, renderer, &player, &scene_1, food_1);

	// ------------------------ Game init ------------------------
	Scene *scenes[1] = {&scene_1};
	int current_scene = 0;

	Game_Init(&game, scenes, 1);

	// Setup timing variables
	uint64_t last_time = SDL_GetTicksNS();
	float delta_time = 0.0f;

	// ------------------------ Game loop ------------------------
	int done = 0;
	while (!done) {
		// -------------------- Delta time --------------------
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

		// Game loop
		Game_Update(&game, renderer, &current_scene, delta_time, event);

		// ------------------ Framerate cap ------------------
		uint64_t end_ticks = SDL_GetTicks();
		uint64_t elapsed_ticks = end_ticks - start_ticks;

		if (elapsed_ticks < FRAME_TARGET_MS) {
			SDL_Delay((uint32_t)(FRAME_TARGET_MS - elapsed_ticks));
		}
	}

	Game_End(&game);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}