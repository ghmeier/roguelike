#include "dungeon.h"
#include "queue.h"
#include "middle.h"

static const int moves[8][2] = {{1,0},{-1,0},{0,1},{0,-1},{1,-1},{-1,-1},{-1,1},{1,1}};

void print_dungeon(dungeon* dungeon, int* c_x, int* c_y){

	int i,j,start_x,start_y;

	start_x = *c_x - H_WIN_WIDTH;
	if (start_x< 0 ){
		start_x = 0;

	}else if (start_x+WIN_WIDTH >= DUNGEON_WIDTH){
		start_x = DUNGEON_WIDTH - WIN_WIDTH;
	}

	start_y = *c_y - H_WIN_HEIGHT;
	if (start_y < 0){
		start_y = 0;
	}else if (start_y+WIN_HEIGHT >= DUNGEON_HEIGHT){
		start_y = DUNGEON_HEIGHT -WIN_HEIGHT;
	}

	char toPrint = '#';
	for (j=start_y;j<start_y+WIN_HEIGHT;j++){
		for (i=start_x;i<start_x + WIN_WIDTH;i++){
			attron(COLOR_PAIR(10));
			toPrint='#';

			if(dungeon->rocks[i][j] <= 0){
				toPrint = '.';
				attron(COLOR_PAIR(9));
				if(dungeon->rocks[i][j] == UP_STAIR){
					toPrint = '<';
				}else if(dungeon->rocks[i][j] == DN_STAIR){
					toPrint = '>';
				}
				if (dungeon->characters[i][j] && dungeon->characters[i][j]->i){
					attron(COLOR_PAIR(dungeon->characters[i][j]->i->color));
					toPrint = OBJ_CHAR[dungeon->characters[i][j]->i->type];
				}

				if (dungeon->characters[i][j] && dungeon->characters[i][j]->n){
					attron(COLOR_PAIR(dungeon->characters[i][j]->n->color));
					toPrint = dungeon->characters[i][j]->n->out;
					//mvprintw(j-start_y,i-start_x,"%d",dungeon->characters[i][j]->n->hp);
				}
				if (dungeon->playerPtr->x == i && dungeon->playerPtr->y == j && dungeon->playerPtr->alive >= 0){
					attron(COLOR_PAIR(8));
					toPrint = '@';
				}
			}
			mvaddch(j-start_y,i-start_x,toPrint);
		}
		attron(COLOR_PAIR(10));
	}

	mvprintw(0,0,"HP: %d of %d Spd: %d, Def: %d",dungeon->playerPtr->alive,MAX_HEALTH,get_speed(dungeon->playerPtr),get_defense(dungeon->playerPtr));
	if (dungeon->playerPtr->last_dm > 0){
		mvprintw(WIN_HEIGHT-1,0,"Took %d damage.",dungeon->playerPtr->last_dm);
	}
	if (dungeon->playerPtr->last_atk > 0){
		mvprintw(WIN_HEIGHT,0,"Dealt %d damage.",dungeon->playerPtr->last_atk);
	}

}

void print_inventory(player* p){
	int i;
	mvprintw(0,0,"Inventory:");
	for (i=0;i<p->carrying;i++){
		mvprintw(i+1,0,"item %d: %s %s",i,p->inventory[i]->name,OBJ_TYPES[p->inventory[i]->type]);
	}
	for (;i<MAX_CARRY;i++){
		mvprintw(i+1,0,"item %d: empty",i);
	}
}

void print_equipment(player* p){
	int i;
	mvprintw(0,0,"Equipment:");
	for (i=0;i<p->wearing;i++){
		mvprintw(i+1,0,"equip %c: %s",'a'+i,p->equipped[i]->name);
	}
	for (;i<MAX_WEAR;i++){
		mvprintw(i+1,0,"equip %c: empty",'a'+i);
	}
}

void init_dungeon(dungeon* dungeon,int width,int height,int roomNumber,int npc_num, void* mons_temp,int num_mons,void* item_temp,int num_items){

	int** rocks = (int**) malloc(width * sizeof(int*));
	room* rooms = (room*) malloc(DUNGEON_ROOMS_MAX*sizeof(room));
	dungeon->npcs = (c_node*) malloc(npc_num*sizeof(c_node));
	dungeon->items = (c_node*) malloc(ITEM_MAX * sizeof(c_node));
	dungeon->characters = (c_node***)malloc(DUNGEON_WIDTH*sizeof(c_node**));
	player* playerPtr = (player*) malloc(sizeof(player) );
	dungeon->num_items = ITEM_MAX;

	int i,j;
	printf("initializing rocks");
	for (i=0;i<width;i++){
		rocks[i] = (int*) malloc(height * sizeof(int));
		dungeon->characters[i] = (c_node**)malloc(height * sizeof(c_node*));
		for (j=0;j<height;j++){
			dungeon->characters[i][j] = NULL;
			if (i==0 || i == DUNGEON_WIDTH -1 || j==0 || j == DUNGEON_HEIGHT - 1){
				rocks[i][j] = IMMUTE;
			}else{
				rocks[i][j] = SOLID;
			}
		}
		if (i%10 == 0){
			printf(".");
		}
	}
	printf("\n");

	int z;
	printf("adding rooms");
	for (z=0;z<roomNumber;z++){
		room r;
		get_random_room(&r);
		int fails = 0;
		while(can_place_room(rooms,&r,z) == 0 && fails <1000){
			get_random_room(&r);
			fails++;
		}

		if (fails >=1000){
			roomNumber = z;
			break;
		}

		if (z == roomNumber - 1 && roomNumber < DUNGEON_ROOMS_MAX){
			roomNumber++;
		}

		rooms[z] = r;

		printf(".");

	}
	printf("\n");

	printf("adding player");
    init_player(playerPtr,rooms,roomNumber);
	printf("\n");

	dungeon->width = width;
	dungeon->height = height;
	dungeon->roomNumber = roomNumber;
	dungeon->rocks = rocks;
	dungeon->rooms = rooms;
	dungeon->playerPtr = playerPtr;

	printf("adding items\n");
	init_items(dungeon,item_temp,num_items,ITEM_MAX);
	printf("\n");

	printf("adding npcs");
	init_npc_multiple(dungeon->characters,dungeon->npcs,npc_num,rooms,roomNumber,playerPtr,mons_temp,num_mons);
	printf("\n");

	//dungeon->npcs = npcs;
	dungeon->npc_num = npc_num;
	dungeon->npc_max = npc_num;
	dungeon->turn = 0;
	//dungeon->characters = characters;
	printf("adding paths");
	add_rooms(*dungeon);

}

void add_rooms(dungeon dungeon){
	int i,j,z;

	for (z=0;z<dungeon.roomNumber;z++){
		printf(".");
		connect_rooms(dungeon,dungeon.rooms[z],z);
	}
	printf("\n");
	for (z=0;z<dungeon.roomNumber;z++){
		room* r = &dungeon.rooms[z];

		for (i=r->left;i<r->right && i<dungeon.width-1;i++){
			for (j=r->top;j<r->bottom && j<dungeon.height-1;j++){
				if (dungeon.rocks[i][j] != IMMUTE){
					dungeon.rocks[i][j] = OPEN;
				}
			}
		}

		if ((rand() & 1) == 1){
			int s_y = r->top + (rand()%r->height);
			int s_x = r->left + (rand()%r->width);

			dungeon.rocks[s_x][s_y] = DN_STAIR + (rand()&1);
		}
	}

}

void connect_rooms(dungeon dungeon, room r, int placed){
	if (placed == 0){
		return;
	}

	int z, i = r.center[0],j=r.center[1];
	int min = 1000, result = 0;
	int finalDest[2];

	for (z=0;z<placed;z++){
		int* distVector = (int*)malloc(2 * sizeof(int));
		get_distance_vector(&dungeon.rooms[z],&r,distVector);
		result = distance(distVector[0],distVector[1]);
		free(distVector);
		if (result < min){
			min = result;
			finalDest[0] = dungeon.rooms[z].center[0];
			finalDest[1] = dungeon.rooms[z].center[1];
		}

	}

	while ((i != finalDest[0] || j != finalDest[1]) && i > 0 && j > 0 && i<dungeon.width-1 && j<dungeon.height-1 ){
		int next[2] ;
		get_unvisited(dungeon,i,j,finalDest[0],finalDest[1],next);
		i = next[0];
		j = next[1];
		if (dungeon.rocks[i][j] != IMMUTE){
			if (dungeon.rocks[i][j] == OPEN){
				break;
			}
			dungeon.rocks[i][j] = OPEN;
		}
	}
}

void get_unvisited(dungeon dungeon, int x, int y, int destX, int destY, int* choice){
	int min = 1000,i,result;
	for (i=0;i<4;i++){
		int nextX = x + moves[i][0];
		int nextY = y + moves[i][1];
		int distX = nextX - destX;
		int distY = nextY - destY;
		result = distance(distX,distY);

		if (nextX <= 0 || nextX >= DUNGEON_WIDTH-1 || nextY <= 0 || nextY >= DUNGEON_HEIGHT-1 || dungeon.rocks[nextX][nextY] == IMMUTE){
			result += 1000;
		}

		if (result < min){
			min = result;
			choice[0] = nextX;
			choice[1] = nextY;
		}
	}
}

int can_place_room(room* rooms, room* r, int placed){
	int i;
	for (i=0; i<placed; i++){
		if (is_distant(r,&rooms[i]) == 0){
			return 0;
		}
	}

	return 1;
}

void move_player(dungeon* dungeon){
	if (dungeon->playerPtr->turn-- <= 0){
		dungeon->playerPtr->turn = 100/get_speed(dungeon->playerPtr);
		int player_move[2] = {dungeon->playerPtr->x,dungeon->playerPtr->y};
		find_player_move(dungeon,dungeon->playerPtr,player_move);
		check_collision(dungeon,&player_move[0],&player_move[1]);
		dungeon->playerPtr->x = player_move[0];
		dungeon->playerPtr->y = player_move[1];
		if (dungeon->characters[player_move[0]][player_move[1]] && dungeon->characters[player_move[0]][player_move[1]]->i){
			if (add_item(dungeon->playerPtr,dungeon->characters[player_move[0]][player_move[1]]->i) == 0){
				dungeon->characters[player_move[0]][player_move[1]] = NULL;
			}
		}
	}
}

void move_npcs(dungeon* dungeon,q_node** nodes){

	int x = dungeon->playerPtr->x;
	int y = dungeon->playerPtr->y;

	if (x>0 && x<DUNGEON_WIDTH && y>0 && y<DUNGEON_HEIGHT && dungeon->rocks[x][y] == OPEN){

		int v=0;

		for (v=0;v<dungeon->npc_num;v++){
			if (dungeon->npcs[v].n && dungeon->npcs[v].n->alive == 1){
				remove_npc(dungeon,v--);
				continue;
			}

			if (dungeon->npcs[v].turn-- > 0){
				continue;
			}else{
				dungeon->npcs[v].turn = 100 / dungeon->npcs[v].n->speed;
			}

			int best[2] = {dungeon->npcs[v].n->x,dungeon->npcs[v].n->y};
			int dest[2] = {dungeon->npcs[v].n->x,dungeon->npcs[v].n->y};

			update_dest(dungeon->npcs[v].n,dungeon->rocks);
			int is_smart = NPC_CHAR[dungeon->npcs[v].n->type] & NPC_SMART;
			uint8_t seen_player = dungeon->npcs[v].n->dest_x > 0 && dungeon->npcs[v].n->dest_y > 0;
			if (is_smart == 1 &&  seen_player == 1){
				find_best(nodes,dungeon,dungeon->npcs[v].n,dest,best);
			}else if (seen_player == 1) {
				find_shortest(dungeon,dungeon->npcs[v].n,best);
			}else {
				find_random(dungeon,dungeon->npcs[v].n,best);
			}

			dungeon->characters[dungeon->npcs[v].n->x][dungeon->npcs[v].n->y] = NULL;
			dungeon->npcs[v].n->x = best[0];
			dungeon->npcs[v].n->y = best[1];
			dungeon->characters[best[0]][best[1]] = &dungeon->npcs[v];
		}
	}

	refresh_node_array(nodes,dungeon);
}

void init_items(dungeon* dungeon,void *templates[],int num_temps, int item_max){
	int i,j=0;
	while (j<item_max){
		for (i=0;i<dungeon->roomNumber&&j<item_max;i++){
			int x=rand()%dungeon->rooms[i].width + dungeon->rooms[i].left;
			int y=rand()%dungeon->rooms[i].height + dungeon->rooms[i].top;
			//if (!dungeon->characters[x][y]){
				dungeon->items[j].i = (item*)malloc(sizeof(item));
				dungeon->items[j].n = NULL;
				dungeon->items[j].p = NULL;
				item_factory(templates[rand()%num_temps],dungeon->items[j].i);
				dungeon->characters[x][y] = &dungeon->items[j];

				j++;
			//}

		}
	}
}

void init_node_array(q_node** nodes,dungeon* dungeon){
	int i,j;

	for (i=0;i<dungeon->width;i++){
		nodes[i] = (q_node*)malloc(dungeon->height*sizeof(q_node));
		for (j=0;j<dungeon->height;j++){
			nodes[i][j].pri = 3000U;
			nodes[i][j].index = 0U;
			nodes[i][j].p = (path*)malloc(sizeof(path));
		}
	}
}

void free_node_array(q_node** nodes,dungeon* dungeon){
	int i,j;
	for (i=0;i<dungeon->width;i++){
		for (j=0;j<dungeon->height;j++){
			free(nodes[i][j].p);
		}
		free(nodes[i]);
	}
	free(nodes);
}

void refresh_node_array(q_node** nodes,dungeon* dungeon){
	int i,j;
	for (i=0;i<dungeon->width;i++){
		for (j=0;j<dungeon->height;j++){
			nodes[i][j].pri = 3000U;
			nodes[i][j].index = 0U;
		}
	}
}

void find_next(path* cur, q_node** nodes,int* dest,dungeon* dungeon){

	int i;

	for (i=0;i<8;i++){
		int pot[2] = {cur->pos[0] + moves[i][0],cur->pos[1] + moves[i][1]};

		if (pot[0] == dest[0] && pot[1] == dest[1]){
			return;
		}

		if (pot[0] > 0 && pot[0] < DUNGEON_WIDTH && pot[1] > 0 && pot[1] < DUNGEON_HEIGHT && dungeon->rocks[pot[0]][pot[1]] == OPEN ){
			path p;
			p.cost = 1U;
			p.from[0] = 0;
			p.from[1] = 0;
			p.pos[0] = 0;
			p.pos[1] = 0;
			if (nodes[pot[0]][pot[1]].index > 0){
				path* p = nodes[pot[0]][pot[1]].p;

				if (cur->cost+1 < p->cost){
					p->cost = cur->cost+1;
					p->from[0] = cur->pos[0];
					p->from[1] = cur->pos[1];
					nodes[pot[0]][pot[1]].pri = p->cost;
					find_next(p,nodes,dest,dungeon);
				}
			}else {
				p.pos[0] = pot[0];
				p.pos[1] = pot[1];
				p.from[0] = cur->pos[0];
				p.from[1] = cur->pos[1];
				p.cost = cur->cost+1;
				nodes[pot[0]][pot[1]].pri = p.cost;
				*nodes[pot[0]][pot[1]].p = p;
				nodes[pot[0]][pot[1]].index = 1;

				find_next(&p,nodes,dest,dungeon);
			}
		}
	}

	return;
}

void find_best(q_node** nodes,dungeon* dungeon,npc* npc, int* dest, int* best){
	path p;
	p.pos[0] =npc->dest_x;
	p.pos[1] = npc->dest_y;
	p.from[0] = 0;
	p.from[1] = 0;
	p.cost = 1;

	find_next(&p,nodes,dest,dungeon);
	int z;
	uint16_t min = 3000U;;

	for (z=0;z<8;z++){
		int toX,toY;
		toX = npc->x + moves[z][0];
		toY = npc->y + moves[z][1];

		if(toX > 0 && toY > 0 && toX < dungeon->width && toY <dungeon->height && nodes[toX][toY].index>0 && nodes[toX][toY].pri < min){
			min = nodes[toX][toY].pri;
			best[0] = toX;
			best[1] = toY;
		}

		if (toX == npc->dest_x && toY == npc->dest_y){
			best[0] = toX;
			best[1] = toY;
			min = 0;
			break;
		}
	}

	npc_check_collision(dungeon,npc,&best[0],&best[1]);
}

void find_shortest(dungeon* dungeon, npc* npc, int* best){

	int z;
	uint16_t min = 3000U;
	int player[2] = {npc->dest_x,npc->dest_y};

	for (z=0;z<8;z++){
		int toX,toY;
		toX = npc->x + moves[z][0];
		toY = npc->y + moves[z][1];

		if(toX > 0 && toY > 0 && toX < dungeon->width && toY <dungeon->height && dungeon->rocks[toX][toY] == OPEN){
			int dist = distance(toX - player[0],toY - player[1]);
			if (dist < min){
				min = dist;
				best[0] = toX;
				best[1] = toY;
			}
		}

		if (toX == player[0] && toY == player[1]){
			min = 0;
			best[0] = toX;
			best[1] = toY;
			break;
		}
	}

	npc_check_collision(dungeon,npc,&best[0],&best[1]);

}

void find_random(dungeon* dungeon, npc* npc, int* best){

	int test[2] = {npc->x + moves[npc->dir][0],npc->y + moves[npc->dir][1]};
	if (test[0] > 0 && test[0] < dungeon->width && test[1] > 0 && test[1] < dungeon->height && dungeon->rocks[test[0]][test[1]] == OPEN){
		best[0] = test[0];
		best[1] = test[1];

		npc_check_collision(dungeon,npc,&best[0],&best[1]);
		return;
	}

	npc->dir = rand()%4;
}

void find_player_move(dungeon* dungeon, player* pc, int* best){

	int test[2] = {pc->x ,pc->y};

	if (pc->dir >= 0 ){
		test[0] = pc->x + moves[pc->dir][0];
		test[1] = pc->y + moves[pc->dir][1];
	}

	if (test[0] > 0 && test[0] < dungeon->width && test[1] > 0 && test[1] < dungeon->height && dungeon->rocks[test[0]][test[1]] != IMMUTE){
		best[0] = test[0];
		best[1] = test[1];
		if (dungeon->rocks[test[0]][test[1]] > 0){
			dungeon->rocks[test[0]][test[1]] = 0;
		}
	}
}

void check_collision(dungeon* dungeon,int* toX, int* toY){

	if (dungeon->characters[*toX][*toY] && dungeon->characters[*toX][*toY]->n){
		//dungeon->characters[toX][toY]->npc->alive = 1;
		int player_dam = get_damage(dungeon->playerPtr);

		dungeon->characters[*toX][*toY]->n->hp -= player_dam;
		dungeon->playerPtr->last_atk = player_dam;
		if (dungeon->characters[*toX][*toY]->n->hp <=0) {
			remove_npc(dungeon,dungeon->characters[*toX][*toY]->index);
		}else{
			*toX = dungeon->playerPtr->x;
			*toY = dungeon->playerPtr->y;
		}
	}
}

void npc_check_collision(dungeon* dungeon,npc* npc, int* toX, int* toY){

	if (dungeon->characters[*toX][*toY] && dungeon->characters[*toX][*toY]->n){
		*toX = npc->x;
		*toY = npc->y;
		return;
	}

	if (*toX == dungeon->playerPtr->x && dungeon->playerPtr->y == *toY){
		int dam = c_roll(npc->damage);
		int net = get_actual_damage(dungeon->playerPtr,dam);
		take_damage(dungeon->playerPtr,net);
		*toX = npc->x;
		*toY = npc->y;
	}
}

void remove_npc(dungeon* dungeon, int index){
	npc* to_remove = dungeon->npcs[index].n;
	dungeon->npcs[index].n = dungeon->npcs[dungeon->npc_num-1].n;
	dungeon->npcs[index].turn = dungeon->npcs[dungeon->npc_num-1].turn;
	dungeon->npcs[dungeon->npc_num-1].n = to_remove;
	dungeon->npc_num--;
	dungeon->characters[to_remove->x][to_remove->y] = NULL;
	//free(to_remove);
}

void free_dungeon(dungeon* dungeon){
	int i=0;
	for (i=0;i<dungeon->width;i++){
		free(dungeon->rocks[i]);
		free(dungeon->characters[i]);
	}
	free(dungeon->characters);
	free(dungeon->rocks);
	free(dungeon->rooms);

	for (i=0;i<dungeon->npc_max;i++){
		if(dungeon->npcs[i].n){
			free(dungeon->npcs[i].n->description);
			free(dungeon->npcs[i].n->name);
			free(dungeon->npcs[i].n->damage);
			free(dungeon->npcs[i].n);
		}
	}

	for (i=0;i<dungeon->num_items;i++){
		if (dungeon->items[i].i){
			free_item(dungeon->items[i].i);
		}
	}
	free(dungeon->npcs);
	free(dungeon->items);

	free(dungeon->playerPtr->damage);
	free(dungeon->playerPtr);

	free(dungeon);
}