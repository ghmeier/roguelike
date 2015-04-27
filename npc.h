#ifndef NPC_H
#define NPC_H

# define NPC_TYPE_NUM	   4

//Just using the bit field shown in class. Seems like the best solution, so I'm using it.
# define NPC_SMART         0x00000001
# define NPC_TELEP		   0x00000002
# define NPC_BIT02         0x00000004
# define NPC_BIT03         0x00000008
# define NPC_BIT04         0x00000010
# define NPC_BIT05         0x00000020
# define NPC_BIT06         0x00000040
# define NPC_BIT07         0x00000080
# define NPC_BIT08         0x00000100
# define NPC_BIT09         0x00000200
# define NPC_BIT10         0x00000400
# define NPC_BIT11         0x00000800
# define NPC_BIT12         0x00001000
# define NPC_BIT13         0x00002000
# define NPC_BIT14         0x00004000
# define NPC_BIT15         0x00008000
# define NPC_BIT16         0x00010000
# define NPC_BIT17         0x00020000
# define NPC_BIT18         0x00040000
# define NPC_BIT19         0x00080000
# define NPC_BIT20         0x00100000
# define NPC_BIT21         0x00200000
# define NPC_BIT22         0x00400000
# define NPC_BIT23         0x00800000
# define NPC_BIT24         0x01000000
# define NPC_BIT25         0x02000000
# define NPC_BIT26         0x04000000
# define NPC_BIT27         0x08000000
# define NPC_BIT28         0x10000000
# define NPC_BIT29         0x20000000
# define NPC_BIT30         0x40000000
# define NPC_BIT31         0x80000000

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "player.h"
#include "room.h"
#include "queue.h"
#include "item.h"

extern char NPC_TYPE_CHAR[];
extern int NPC_CHAR[];

typedef struct npc npc;
struct npc{
	int x,y,dest_x,dest_y;
	char out;
    char*description,*name;
    int hp,speed;
	uint8_t type,dir,alive,color;
	player* p;
    void* damage;
};

typedef struct c_node c_node;
struct c_node{
	npc* n;
	player* p;
    item* i;
	uint32_t turn;
	uint32_t index;
};

void init_npc(npc* npc,room room,player* player);
void init_npc_multiple(c_node*** characters,c_node* npcs,int npc_number,room* rooms,int room_number,player* player,void* mon[],int num_mons);
void print_npc(npc* npc);
void update_dest(npc* npc,int** rocks);
int is_obstructed(npc* npc,player* player,int** rocks);

#endif