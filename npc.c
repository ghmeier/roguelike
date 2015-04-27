#include "npc.h"
#include "middle.h"

char NPC_TYPE_CHAR[] = {'p','P','g','G'};
int NPC_CHAR[] = {0,NPC_SMART,NPC_TELEP,NPC_SMART|NPC_TELEP};


void init_npc(npc* npc,room room,player* player){

	npc->y = room.top + (rand() & (room.height-1));
	npc->x = room.left + (rand() & (room.width-1));
	//npc->speed = (rand() & 15) + 5;
	npc->p = player;
	//npc->type = rand() & (NPC_TYPE_NUM-1);
	//npc->out = NPC_TYPE_CHAR[npc->type];
	npc->dir = rand() % 4;
	npc->dest_x = -1;
	npc->dest_y = -1;
	npc->alive = 0;

}

void init_npc_multiple(c_node*** characters, c_node* npcs,int npc_number,room* rooms,int room_number,player* player,void* mon[],int num_mons){
	int i=0,z=0,fails = 0;

	for (i=0;i<npc_number;i++){
		npcs[i].turn = 0;
		npc cur;

		init_npc(&cur,rooms[z],player);
		if (cur.x == player->x && cur.y == player->y && fails < 20){
			//free(npcs[i].npc);
			fails++;
		} else {
			if (characters[cur.x][cur.y]){
				fails++;
				//free(npcs[i].npc);
			}else{
				printf(".");
				fails = 0;
			}
		}

		if (fails==0){
			npcs[i].n = (npc*)malloc(sizeof(npc));
			int random = rand()%num_mons;
			npc_factory(mon[random],&cur);
			*npcs[i].n = cur;
			npcs[i].i = NULL;
			npcs[i].p = NULL;
			characters[npcs[i].n->x][npcs[i].n->y] = &npcs[i];
			npcs[i].index = i;
		}else {
			i--;
		}

		z++;
		if (z == player->start_room_i){
			z++;
		}

		if (z >=room_number){
			z = z % room_number;
		}
	}
}

void update_dest(npc* npc, int** rocks){
	int is_telep = NPC_CHAR[npc->type] & NPC_TELEP;

	if (is_telep == NPC_TELEP ){
		npc->dest_x = npc->p->x;
		npc->dest_y = npc->p->y;
	}else if (distance(npc->x - npc->p->x, npc->y - npc->p->y) < 10){
		//check if the npc is in sight, else default dest
		if (is_obstructed(npc,npc->p,rocks) == 1){
			npc->dest_x = npc->p->x;
			npc->dest_y = npc->p->y;
		}
	}
}

// returns 0 if view is obstructed, 1 if not
int is_obstructed(npc* npc, player* player,int** rocks){
	int xMod,yMod,i=0,x=npc->x,y=npc->y;
	xMod = npc->x - player->x;
	yMod = npc->y - player->y;

	while(i<10 && x<=player->x && y<=player->y){
		x += 1;
		if (xMod != 0){
			y += yMod/xMod * (x-npc->x);
		}

		if (rocks[x][y] != 0 ){
			return 0;
		}
		i++;
	}
	return 1;
}

void print_npc(npc* npc){
	printf("NPC at: %d,%d with speed: %d, dest: %d,%d, out: %c, alive: %d\n",npc->x,npc->y,npc->speed,npc->dest_x,npc->dest_y,npc->out,npc->alive );
}