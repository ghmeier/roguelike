#include "room.h"
#include <math.h>

void make_room(room* r,int x, int y, int width, int height){
	int top = y,bottom = y+height, left = x, right = x +width;

	if (bottom >= DUNGEON_HEIGHT){
		bottom = DUNGEON_HEIGHT-1;
	}
	if (top <= 0) {
		top = 1;
	}
	if (left <= 0){
		left = 1;
	}
	if (right >= DUNGEON_WIDTH){
		right = DUNGEON_WIDTH - 1;
	}

	midpoint(left,right,top,bottom,r->center);
	r->top = top;
	r->bottom = bottom;
	r->left = left;
	r->right = right;
	r->width = right-left;
	r->height = bottom-top;
}

void get_random_room(room* r){
	int x,y,width,height;

	x = (rand() % (DUNGEON_WIDTH - ROOM_MIN_WIDTH)) + 1;
	y = (rand() % (DUNGEON_HEIGHT - ROOM_MIN_HEIGHT)) +1;
	width = (rand() & ROOM_VARIANCE) + ROOM_MIN_WIDTH;
	height = (rand() & ROOM_VARIANCE) + ROOM_MIN_HEIGHT;

	make_room(r,x,y,width,height);
}

void print_room(room r){
	printf("\nRoom: %d<%d<%d|%d<%d<%d\n",r.left,r.center[0],r.right,r.top,r.center[1],r.bottom);
}

//puts x,y distance into coords
void get_distance_vector(room* r1, room* r2,int * coords){

	int xDistance = r1->center[0] - r2->center[0];
	int yDistance = r1->center[1] - r2->center[1];

	coords[0]=xDistance;
	coords[1]=yDistance;

}

int is_distant(room* r1, room* r2){
	int coords[2];
	coords[0] = r1->left;
	coords[1] = r1->top;
	get_distance_vector(r1,r2,coords);

	int minDist = distance((r1->width + r2->width) / 2,(r1->height + r2->height) / 2) + 3;
	int actualDist = distance(coords[0],coords[1]);

	if (actualDist <= minDist){
		return 0;
	}

	return 1;
}

void midpoint(int x1, int x2,int y1, int y2,int* point){
	int x = (x1+x2) / 2;
	int y = (y1+y2) /2;

	point[0] = x;
	point[1] = y;
}

int distance(int x, int y){
	double temp = x * x + y * y;
	return sqrt(temp);
}

void free_room(room* r){
	free(r->center);
	//free(r->stair);
	free(r);
}