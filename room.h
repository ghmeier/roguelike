#ifndef ROOM_H
#define ROOM_H
#define ROOM_MIN_WIDTH 8
#define ROOM_MIN_HEIGHT 5
#define ROOM_VARIANCE 10
#define DUNGEON_ROOMS_MIN 12
#define DUNGEON_ROOMS_MAX 48
#define DUNGEON_WIDTH 160
#define DUNGEON_HEIGHT 96

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

typedef struct stair stair;
struct stair{
	uint8_t up;
	int pos[2];
};

typedef struct room room;
struct room{
	int top,left,bottom,right,width,height;
	int center[2];
	//stair stair;
};

void print_room(room r);
void make_room(room* r,int x, int y, int width, int height);
void get_random_room(room* r);
void get_distance_vector(room* r1,room* r2,int* coords);
int is_distant(room* r1, room* r2);
int distance(int x, int y);
void midpoint(int x1, int x2, int y1, int y2, int* point);
void free_room(room* r);

#endif