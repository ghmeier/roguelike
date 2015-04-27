#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdint.h>
#include "room.h"
#include "item.h"

#define MAX_CARRY 10
#define MAX_WEAR 12
#define MAX_HEALTH 100

typedef struct player player;
struct player{
	int x,y,speed,dir,carrying,wearing,alive,last_dm,last_atk;
	uint16_t start_room_i;
	uint8_t turn;
    item *equipped[MAX_WEAR];
    item *inventory[MAX_CARRY];
    void *damage;
};

void init_player(player* player,room* rooms, int room_number);
int is_player_move(player* player);
int add_item(player* p, item* i);
item* shrink_inventory(player* p,int index);
void equip(player* p,int index);
void remove_equipped(player* p,int index);
void shrink_equipped(player* p, int index);
int get_speed(player* p);
void take_damage(player* p,int net);
int get_damage(player *p);
int get_defense(player *p);
int get_actual_damage(player* p,int raw);
#endif