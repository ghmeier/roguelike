#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "npc.h"
#include "room.h"
#include "player.h"
#include "queue.h"
#include "item.h"

#define DUNGEON_ROOMS_MIN 12
#define DUNGEON_ROOMS_MAX 48
#define DUNGEON_WIDTH 160
#define DUNGEON_HEIGHT 96
#define WIN_WIDTH 80
#define H_WIN_WIDTH WIN_WIDTH/2
#define WIN_HEIGHT 24
#define H_WIN_HEIGHT WIN_HEIGHT/2
#define ITEM_MAX 100

#define MR 0
#define ML 1
#define MD 2
#define MU 3
#define MUR 4
#define MUL 5
#define MDL 6
#define MDR 7

#define IMMUTE 255
#define OPEN 0
#define SOLID 100
#define UP_STAIR -2
#define DN_STAIR -1

typedef struct dungeon dungeon;
struct dungeon{
	int width;
	int height;
	int roomNumber;
	int npc_num,npc_max,turn,num_items;

	int** rocks;
	room* rooms;
	c_node* npcs;
	c_node*** characters;
	player* playerPtr;
    c_node *items;
};

void init_dungeon(dungeon* dungeon,int width,int height,int roomNumber,int npc_num, void* mons_temp,int num_mons,void* item_temp,int num_items);
void free_dungeon(dungeon* dungeon);
void print_dungeon(dungeon* dungeon,int* c_x, int* c_y);
void print_inventory(player* p);
void print_equipment(player* p);
void add_rooms(dungeon dungeon);
int  can_place_room(room* rooms, room* r, int placed);
void connect_rooms(dungeon dungeon, room r, int placed);
void get_unvisited(dungeon dungeon, int x, int y,int destX,int destY, int* choice);
void move_npcs(dungeon* dungeon,q_node** nodes);
void move_player(dungeon* dungeon);
void init_items(dungeon* dungeon,void *templates[],int num_temps,int item_max);
void init_node_array(q_node** nodes,dungeon* dungeon);
void free_node_array(q_node** nodes,dungeon* dungeon);
void refresh_node_array(q_node** nodes,dungeon* dungeon);
void find_next(path* cur, q_node** nodes,int* dest,dungeon* dungeon);
void find_best(q_node** nodes,dungeon* dungeon,npc* npc, int* dest, int* best);
void find_shortest(dungeon* dungeon, npc* npc, int* best);
void find_random(dungeon* dungeon,npc* npc, int* best);
void find_player_move(dungeon* dungeon,player* pc, int* best);
void check_collision(dungeon* dungeon, int* toX, int* toY);
void npc_check_collision(dungeon* dungeon, npc* npc, int* toX,int* toY);
void remove_npc(dungeon* dungeon, int index);
#endif