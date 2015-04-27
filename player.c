#include "player.h"
#include "middle.h"

void init_player(player* player,room* rooms, int room_number){
	int z = rand() & (room_number -1);
	int x  = rooms[z].left+1 + (rand() & (rooms[z].width-2));
	int y  = rooms[z].top+1 + (rand() & (rooms[z].height-2));
	player->x = x;
	player->y = y;
	player->start_room_i = z;
	player->speed = 10;
	player->dir = -1;
	player->turn = 0;
	player->alive = MAX_HEALTH;
	player->carrying = player->wearing = 0;
	player->last_dm = 0;
	player->last_atk = 0;

	player->damage = NULL;

}

int is_player_move(player* player){
	return player->turn == 0;
}

int add_item(player* p, item* i){
	if (p->carrying < MAX_CARRY){
		p->inventory[p->carrying] = i;
		p->carrying++;
		return 0;
	}

	return 1;
}

item* shrink_inventory(player* p,int index){
	int i=0;
	item* dropped = NULL;

	for (i=index;i<p->carrying;i++){
		if (i==index){
			dropped = p->inventory[i];
			p->carrying--;
		}
		p->inventory[i] = p->inventory[i+1];
	}
	return dropped;

}

void equip(player* p,int index){
	int i=0,ring = 0;

	item* temp = p->inventory[index];
	for (i=0;i<p->wearing;i++){
		if (p->equipped[i]->type == temp->type){
			if (temp->type == OBJ_RING && ring == 0){
				ring++;
			}else{
				p->inventory[i] = p->equipped[i];
				p->equipped[i] = temp;
				return;
			}
		}
	}

	if (p->wearing < MAX_WEAR){
		p->equipped[p->wearing++]=temp;
		shrink_inventory(p,index);
	}

}

void shrink_equipped(player* p, int index){
	int i=0;

	for (i=index;i<p->wearing;i++){
		p->equipped[i]=p->equipped[i+1];
	}

	p->wearing--;
}

void remove_equipped(player* p,int index){
	if (add_item(p,p->equipped[index]) == 0){
		shrink_equipped(p,index);
	}

}

int get_speed(player* p){
	int full_speed = p->speed,i;

	for (i=0;i<p->wearing;i++){
		full_speed += p->equipped[i]->speed;
	}

	return full_speed;
}

int get_damage(player* p){
	int full_dam = c_roll(p->damage),i;

	for (i=0;i<p->wearing;i++){
		full_dam += c_roll(p->equipped[i]->damage);
	}

	return full_dam;
}

int get_defense(player* p){
	int i,full_def = 0;
	for (i=0;i < p->wearing;i++){
		full_def += p->equipped[i]->defense;
	}

	return full_def;
}

int get_actual_damage(player* p,int raw){
	int net = raw - get_defense(p);
	if (net < 0){
		net =0;
	}

	return net;
}

void take_damage(player* p, int net){
	p->last_dm += net;
	p->alive -= net;
}