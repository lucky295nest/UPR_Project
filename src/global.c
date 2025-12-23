#include <SDL3/SDL.h>
#include <stdio.h>

#include "global.h"

char global_assets_path[512];

void InitGlobalPath() {
	const char *base = SDL_GetBasePath();

	if (!base) {
		printf("SDL_GetBasePath failed: %s\n", SDL_GetError());
	}

	snprintf(global_assets_path, sizeof(global_assets_path), "%sassets/", base);
}