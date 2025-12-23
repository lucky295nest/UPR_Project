#ifndef GLOBAL_H
#define GLOBAL_H

#define FILE_PATH_SIZE 512

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

extern char global_assets_path[FILE_PATH_SIZE];

void InitGlobalPath();

#endif