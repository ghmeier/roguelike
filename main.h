#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C"{
#endif

#define DUNGEON_ROOMS_MIN 12
#define DUNGEON_ROOMS_MAX 48
#define DUNGEON_WIDTH 160
#define DUNGEON_HEIGHT 96
#define VERSION 1
#define FILETYPE "RLG229"
#define DIR "/.rlg229/"
#define M_DIR "/.rlg229/monster_desc.txt"
#define IT_DIR "/.rlg229/object_desc.txt"
#define FILE_NAME "dungeon"

#define UL 		067
#define UL_ALT	0171
#define U 		070
#define U_ALT 	0153
#define UR 		071
#define UR_ALT 	0165
#define R 		066
#define R_ALT 	0154
#define DR 		063
#define DR_ALT 	0156
#define D 		062
#define D_ALT 	0152
#define DL 		061
#define DL_ALT 	0142
#define L 		064
#define L_ALT 	0150
#define UP_S	074
#define UP_S_ALT 054
#define DN_S 	076
#define DN_S_ALT 056

#define SKIP 	040
#define LOOK	0114
#define CTRL	033
#define SAVE 	0123

#define WEAR    0167
#define REMV    0164
#define DROP    0144
#define DEST    0170

#define IS_CONTROL 0
#define IS_LOOK 1
#define IS_END 2
#define IS_UP_STAIR 3
#define IS_DN_STAIR 4

#include <time.h>
#include <stdio.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <endian.h>
#include <ncurses.h>
#include "room.h"
#include "dungeon.h"
#include "player.h"
#include "npc.h"
#include "file_reader.h"

int save_dungeon(FILE* f, dungeon* d);
int load_dungeon(FILE* f, dungeon* d,int npc_num);
void resolve_action(dungeon* d,int* is_control, int action, int* c_x, int* c_y);
void drop_item(dungeon* d);
void destroy_item(dungeon* d);
void equip_item(dungeon* d);
void remove_item(dungeon* d);

#ifdef __cplusplus
}
#endif

#endif