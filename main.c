
#include "main.h"

int main(int argc, char *argv[]){

	/*Command line argument parsing*/
	short load = 0;
	short save = 0;
	int npc_num = 100;
	int i;

	for (i=1;i<argc;i++){
		/* set the flags to 0 if they exist, and exit if we have an invalid argument*/
		if (strncmp(argv[i],"--nummon=xx",9) == 0){
			char* first = strtok(argv[i],"=");
			first = strtok(NULL,"=");
			npc_num = atoi(first);
		}else {
			fprintf(stderr, "%s\n",argv[i] );
			fprintf(stderr,"Invalid argument. Accepted arguments are --nummon=<num>.\n");
			return -1;
		}
	}

	clock_t seed = clock();
	srand(time(NULL));

	FILE* f = (FILE*)malloc(sizeof(*f));
	dungeon *d;
	d = (dungeon*)malloc(sizeof(*d));
	if (load == 0 && load_dungeon(f,d,npc_num) == 0){
		printf("loading...\n");
	}else{
		init_dungeon(d,DUNGEON_WIDTH,DUNGEON_HEIGHT,DUNGEON_ROOMS_MIN,npc_num);
	}

	initscr();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr,TRUE);
	start_color();

    init_pair(1, COLOR_BLUE, COLOR_WHITE);
    init_pair(2,COLOR_RED,COLOR_WHITE);
    init_pair(3,COLOR_BLACK,COLOR_WHITE);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);

	q_node** nodes =(q_node**)malloc(d->width*sizeof(q_node*));
	init_node_array(nodes,d);

	int ch = 0,is_control=0, c_x = d->playerPtr->x, c_y = d->playerPtr->y;

	print_dungeon(d,&c_x,&c_y);
	refresh();

	for (;d->playerPtr->alive == 0 && d->npc_num > 0 && is_control != IS_END;d->turn++){
		if (is_control == IS_CONTROL){
			if (is_player_move(d->playerPtr)){
				ch = getch();
				resolve_action(d,&is_control,ch,&c_x,&c_y);
			}
			if (is_control == IS_CONTROL){
				move_player(d);
				move_npcs(d,nodes);

				c_x = d->playerPtr->x;
				c_y = d->playerPtr->y;
			}
		}else if(is_control == IS_LOOK){
			ch = getch();
			resolve_action(d,&is_control,ch,&c_x,&c_y);
		}else if(is_control == IS_DN_STAIR && d->rocks[d->playerPtr->x][d->playerPtr->y] == DN_STAIR){
			refresh_node_array(nodes,d);
			free_dungeon(d);
			d = (dungeon*)malloc(sizeof(*d));
			init_dungeon(d,DUNGEON_WIDTH,DUNGEON_HEIGHT,DUNGEON_ROOMS_MIN,npc_num);
			clear();
			c_x = d->playerPtr->x;
			c_y = d->playerPtr->y;
			d->rocks[c_x][c_y] = UP_STAIR;
			is_control = IS_CONTROL;
		}else if (is_control==IS_UP_STAIR && d->rocks[d->playerPtr->x][d->playerPtr->y] == UP_STAIR){
			refresh_node_array(nodes,d);
			free_dungeon(d);
			d = (dungeon*)malloc(sizeof(*d));
			init_dungeon(d,DUNGEON_WIDTH,DUNGEON_HEIGHT,DUNGEON_ROOMS_MIN,npc_num);
			clear();
			c_x = d->playerPtr->x;
			c_y = d->playerPtr->y;
			d->rocks[c_x][c_y] = DN_STAIR;
			is_control = IS_CONTROL;
		}else{
			is_control = IS_CONTROL;
			d->playerPtr->dir = -1;
		}

		clear();
		print_dungeon(d,&c_x,&c_y);
		refresh();
		i++;
	}

	free_node_array(nodes,d);

	endwin();

	save_dungeon(f,d);
	free(f);
	if (d->playerPtr->alive == 0 && d->npc_num <=0){
		printf("YOU WON! CONGRATS CAUSE THIS GAME IS HARD AF RIGHT NOW.\n");
	}else if (d->playerPtr->alive == 1){
		printf("YOU LOST, IT'S ALRIGHT, THIS GAME IS REALLY HARD.\n");
	}else{
		if (save == 0 && d->playerPtr->alive == 0){
			printf("Saved\n");
		}
	}

	printf("Finished after %d turns.\n",d->turn);
	free_dungeon(d);


	float diff = (((float)clock() - (float)seed)/1000000.0F ) * 1000;
	printf("Done in ~%.0fms\n",diff);

	return 0;
}

void resolve_action(dungeon* dungeon, int* is_control, int action, int* c_x, int* c_y){
	switch(action){
		case(UL):
		case(UL_ALT):
			if (*is_control == 0){
				dungeon->playerPtr->dir = MUL;
			}else{
				*c_x = *c_x - 1;
				*c_y = *c_y - 1;
			}
			break;
		case(U):
		case(U_ALT):
			if (*is_control == 0){
				dungeon->playerPtr->dir = MU;
			}else {
				*c_y = *c_y - 1;
			}
			break;
		case(UR):
		case(UR_ALT):
			if (*is_control == 0){
				dungeon->playerPtr->dir = MUR;
			}else {
				*c_x = *c_x + 1;
				*c_y = *c_y - 1;
			}
			break;
		case(R):
		case(R_ALT):
			if (*is_control == 0){
				dungeon->playerPtr->dir = MR;
			}else{
				*c_x = *c_x + 1;
			}
			break;
		case(DR):
		case(DR_ALT):
			if (*is_control == 0){
				dungeon->playerPtr->dir = MDR;
			}else{
				*c_x = *c_x + 1;
				*c_y = *c_y + 1;
			}
			break;
		case(D):
		case(D_ALT):
			if (*is_control == 0){
				dungeon->playerPtr->dir = MD;
			}else{
				*c_y = *c_y + 1;
			}
			break;
		case(DL):
		case(DL_ALT):
			if (*is_control == 0){
				dungeon->playerPtr->dir = MDL;
			}else{
				*c_x = *c_x - 1;
				*c_y = *c_y + 1;
			}
			break;
		case(L):
		case(L_ALT):
			if (*is_control == 0){
				dungeon->playerPtr->dir = ML;
			}else{
				*c_x = *c_x - 1;
			}
			break;
		case(UP_S):
		case(UP_S_ALT):
			*is_control = IS_UP_STAIR;
			break;
		case(DN_S):
		case(DN_S_ALT):
			*is_control = IS_DN_STAIR;
			break;
		case(LOOK):
			*is_control = 1;
			break;
		case(CTRL):
			*is_control = 0;
			*c_x = dungeon->playerPtr->x;
			*c_y = dungeon->playerPtr->y;
			break;
		case(SAVE):
			*is_control = 2;
			break;
		case(SKIP):
		default:
			dungeon->playerPtr->dir = -1;
			break;
	}

	if (*c_x < H_WIN_WIDTH){
		*c_x = H_WIN_WIDTH;
	}

	if (*c_x > DUNGEON_WIDTH - H_WIN_WIDTH){
		*c_x = DUNGEON_WIDTH - H_WIN_WIDTH;
	}

	if (*c_y < H_WIN_HEIGHT){
		*c_y = H_WIN_HEIGHT;
	}

	if (*c_y > DUNGEON_HEIGHT - H_WIN_HEIGHT){
		*c_y = DUNGEON_HEIGHT - H_WIN_HEIGHT;
	}
}

int save_dungeon(FILE *f, dungeon* d){
	struct stat st = {0};
	int len = strlen(getenv("HOME")) + strlen(DIR) + strlen(FILE_NAME);
	char* dir = (char*)malloc(sizeof(char)*len+1);
	strcpy(dir,getenv("HOME"));
	dir = strcat(dir,DIR);;

	if (stat(dir, &st) == -1) {
		mkdir(dir, 0700);
	}

	char* file_name = strcat(dir,FILE_NAME);
	f = fopen(file_name,"w");
	free(dir);
	if (!f){
		fprintf(stderr, "Couldn't open file\n");
		return -1;
	}

	if (d->playerPtr->alive == 1 || d->npc_num <= 0){
		return -1;
	}

	fprintf(f, FILETYPE);

	uint32_t version = htobe32(VERSION);
	fwrite(&version,1,sizeof(uint32_t),f);
	//add file length once this happens....
	uint32_t size = htobe32(2 + d->width * d->height * 5 + d->roomNumber * 4 + 2 + 4 + 4 + 2 + d->npc_num * 36);
	uint32_t next_size = size - 14;
	uint32_t* sp = &size;
	fwrite(&next_size,1,sizeof(uint32_t),f);
	fwrite(sp,1,sizeof(uint32_t),f);
	int i,j,z;
	for (j=0;j<d->height;j++){
		for (i=0;i<d->width;i++){
			uint8_t open,room,corridor,hardness=0,stair;
			hardness = d->rocks[i][j];
			if (d->rocks[i][j] <= 0){
				open = 1;
				room = 0;
				corridor = 1;
				stair = 0;
				if (d->rocks[i][j] == UP_STAIR){
					stair = 2;
				}else if(d->rocks[i][j] == DN_STAIR){
					stair = 1;
				}
				for (z=0;z<d->roomNumber;z++){
					if (i >= d->rooms[z].left && i<=d->rooms[z].right && j>=d->rooms[z].top && j<=d->rooms[z].bottom){
						room = 1;
						corridor = 0;
						break;
					}
				}
			}else {
				open = 0;
				room = 0;
				corridor = 0;
			}
			fwrite(&open,1,sizeof(uint8_t),f);
			fwrite(&room,1,sizeof(uint8_t),f);
			fwrite(&corridor,1,sizeof(uint8_t),f);
			fwrite(&hardness,1,sizeof(uint8_t),f);
			fwrite(&stair,1,sizeof(uint8_t),f);
		}
	}

	uint16_t roomNumber = htobe16(d->roomNumber);
	fwrite(&roomNumber,1,sizeof(roomNumber),f);

	for (z=0;z<d->roomNumber;z++){
		uint8_t top =d->rooms[z].top;
		uint8_t left = d->rooms[z].left;
		uint8_t width =d->rooms[z].width;
		uint8_t height = d->rooms[z].height;

		fwrite(&left,1,sizeof(uint8_t),f);
		fwrite(&top,1,sizeof(uint8_t),f);
		fwrite(&width,1,sizeof(uint8_t),f);
		fwrite(&height,1,sizeof(uint8_t),f);
	}

	uint8_t pc_x = d->playerPtr->x,pc_y = d->playerPtr->y;
	fwrite(&pc_x,1,sizeof(uint8_t),f);
	fwrite(&pc_y,1,sizeof(uint8_t),f);

	uint32_t turn = htobe32(d->turn),sequence_num = htobe32(d->npc_max);
	uint16_t npc_num = htobe16(d->npc_num);
	fwrite(&turn,1,sizeof(uint32_t),f);
	fwrite(&sequence_num,1,sizeof(uint32_t),f);
	fwrite(&npc_num,1,sizeof(uint16_t),f);

	for (i=0;i<d->npc_num;i++){
		uint8_t telep,smart;
		uint32_t index = htobe32(i),turn = htobe32(d->npcs[i].turn);
		telep = NPC_CHAR[d->npcs[i].npc->type] & NPC_TELEP;
		smart = NPC_CHAR[d->npcs[i].npc->type] & NPC_SMART;

		fwrite(&d->npcs[i].npc->out,1,sizeof(char),f);
		fwrite(&d->npcs[i].npc->x,1,sizeof(uint8_t),f);
		fwrite(&d->npcs[i].npc->y,1,sizeof(uint8_t),f);
		fwrite(&d->npcs[i].npc->speed,1,sizeof(uint8_t),f);
		fwrite(&smart,1,sizeof(uint8_t),f);
		fwrite(&telep,1,sizeof(uint8_t),f);
		fwrite(&d->npcs[i].npc->dest_x,1,sizeof(uint8_t),f);
		fwrite(&d->npcs[i].npc->dest_y,1,sizeof(uint8_t),f);
		fwrite(&index,1,sizeof(uint32_t),f);
		fwrite(&turn,1,sizeof(uint32_t),f);

		uint8_t zero = 0;
		for (j=0;j<20;j++){
			fwrite(&zero,1,sizeof(uint8_t),f);
		}
	}

	fclose(f);

	return 0;
}

int load_dungeon(FILE *f,dungeon* d,int npc_num){
	int len = strlen(getenv("HOME")) + strlen(DIR) + strlen(FILE_NAME);
	char* dir = (char*)malloc(sizeof(char)*len+1);
	strcpy(dir,getenv("HOME"));

	dir = strcat(dir,DIR);
	char* file_name = strcat(dir,FILE_NAME);
	f = fopen(file_name,"rb");
	free(dir);
	if (!f){
		fprintf(stderr, "Couldn't open file\n");
		return -1;
	}

	char fileType[6] =" ";
	fread(fileType,sizeof(char),6,f);

	if (strcmp(FILETYPE,fileType) != 0){
		printf("Incorrect file type, expected: %s, was %s.\n",FILETYPE,fileType);
		return -1;
	}

	uint32_t version = 0;

	if (!feof(f)){
		fread(&version,sizeof(version),1,f);
	}else {
		return -1;
	}
	version = be32toh(version);
	uint32_t size = 0,next_size;
	if(!feof(f)){
		fread(&next_size,sizeof(next_size),1,f);
		fread(&size,sizeof(size),1,f);
	}else {
		return -1;
	}
	size = be32toh(size);
	next_size = be32toh(next_size);

	int** rocks = (int**) malloc(DUNGEON_WIDTH* sizeof(int*));
	d->characters = (c_node***)malloc(DUNGEON_WIDTH* sizeof(c_node**));
	int i=0,j=0;
	uint8_t rock_props[5];

		for(j=0;j<DUNGEON_HEIGHT;j++){

			for (i=0;i<DUNGEON_WIDTH;i++){
				if (j==0){
					rocks[i] = (int*) malloc(DUNGEON_HEIGHT * sizeof(int));
					d->characters[i] = (c_node**)malloc(DUNGEON_HEIGHT * sizeof(c_node*));
				}
				d->characters[i][j] = NULL;

			if (!feof(f)){
				fread(&rock_props,sizeof(uint8_t),5,f);
				if (rock_props[0] != 0){
					rocks[i][j] = 0;
					if (rock_props[4] == 1){
						rocks[i][j] = DN_STAIR;
					}else if (rock_props[4] == 2){
						rocks[i][j] = UP_STAIR;
					}
				}else{
					rocks[i][j] = rock_props[3];
				}
			}else{
				return -1;
			}
		};
	}


	uint16_t roomNumber = 0;
	if (!feof(f)){
		fread(&roomNumber,sizeof(roomNumber),1,f);
	}else{
		return -1;
	}
	roomNumber = be16toh(roomNumber);
	d->rooms = (room*) malloc(roomNumber*sizeof(room));
	for (i=0;i<roomNumber;i++){
		uint8_t room_props[4];

		if (!feof(f)){
			fread(&room_props,sizeof(uint8_t),4,f);
		}else{
			return -1;
		}

		room r;
		make_room(&r,room_props[0],room_props[1],room_props[2],room_props[3]);
		d->rooms[i]=r;
	}

	d->npcs = (c_node*) malloc(npc_num*sizeof(c_node));
	player* playerPtr = (player*) malloc(sizeof(player) );
	init_player(playerPtr,d->rooms,roomNumber);
	uint8_t pc[2];
	if (!feof(f)){
		fread(&pc,sizeof(uint8_t),2,f);
		playerPtr->x = pc[0];
		playerPtr->y = pc[1];
	}else{
		return -1;
	}

	if (!feof(f)){
		uint32_t turn =0;
		fread(&turn,sizeof(uint32_t),1,f);
		turn = be32toh(turn);
		d->turn = turn;
	}else{
		return -1;
	}

	if (!feof(f)){
		uint32_t sequence_num =0;
		uint16_t npc_num;
		fread(&sequence_num,sizeof(uint32_t),1,f);
		fread(&npc_num,sizeof(uint16_t),1,f);
		sequence_num = be32toh(sequence_num);
		npc_num = be16toh(npc_num);
		d->npc_max = sequence_num;
		d->npc_num = npc_num;
	}else{
		return -1;
	}

	for (i=0;i<d->npc_num;i++){
		uint8_t telep,smart;

		d->npcs[i].npc = (npc*) malloc(sizeof(npc));
		d->npcs[i].npc->dest_x = -1;
		d->npcs[i].npc->dest_y = -1;
		init_npc(d->npcs[i].npc,d->rooms[0],d->playerPtr);
		fread(&d->npcs[i].npc->out,sizeof(char),1,f);
		fread(&d->npcs[i].npc->x,sizeof(uint8_t),1,f);
		fread(&d->npcs[i].npc->y,sizeof(uint8_t),1,f);
		fread(&d->npcs[i].npc->speed,sizeof(uint8_t),1,f);
		fread(&smart,sizeof(uint8_t),1,f);
		fread(&telep,sizeof(uint8_t),1,f);
		fread(&d->npcs[i].npc->dest_x,sizeof(uint8_t),1,f);
		fread(&d->npcs[i].npc->dest_y,sizeof(uint8_t),1,f);
		if (d->npcs[i].npc->dest_x == 255){
			d->npcs[i].npc->dest_x = -1;
		}

		if (d->npcs[i].npc->dest_y == 255){
			d->npcs[i].npc->dest_y = -1;
		}

		fread(&d->npcs[i].index,sizeof(uint32_t),1,f);
		fread(&d->npcs[i].turn,sizeof(uint32_t),1,f);

		d->npcs[i].index = i;
		d->npcs[i].turn = be32toh(d->npcs[i].turn) % d->npcs[i].npc->speed;
		d->npcs[i].npc->type = 0;
		if (smart !=0){
			d->npcs[i].npc->type++;
		}
		if (telep != 0){
			d->npcs[i].npc->type +=2;
		}

		d->npcs[i].npc->player = playerPtr;
		d->npcs[i].npc->alive = 0;
		d->npcs[i].npc->dir = rand() % 8;
		d->characters[d->npcs[i].npc->x][d->npcs[i].npc->y] = &d->npcs[i];

		uint8_t zero = 0;
		for (j=0;j<20;j++){
			fread(&zero,1,sizeof(uint8_t),f);
		}
	}


	fclose(f);


	//init_npc_multiple(d->characters,d->npcs,d->npc_num,d->rooms,roomNumber,playerPtr);

	d->width = DUNGEON_WIDTH;
	d->height = DUNGEON_HEIGHT;
	d->roomNumber = roomNumber;
	d->rocks = rocks;
	d->playerPtr = playerPtr;
	return 0;
}