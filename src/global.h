#ifndef GLOBAL_H
#define GLOBAL_H

typedef struct
{
	float x;
	float y;
} Vector2;

typedef struct
{
	int x;
	int y;
} Vector2_int;

extern char global_assets_path[512];

void InitGlobalPath();

#endif