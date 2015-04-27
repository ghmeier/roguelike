#ifndef ITEM_H
#define ITEM_H

#define NUM_OBJ_TYPES 19

# define OBJ_WEAPON        0
# define OBJ_OFFHAND       1
# define OBJ_RANGED        2
# define OBJ_ARMOR         3
# define OBJ_HELMET        4
# define OBJ_CLOAK         5
# define OBJ_GLOVES        6
# define OBJ_BOOTS         7
# define OBJ_RING          8
# define OBJ_AMULET        9
# define OBJ_LIGHT         10
# define OBJ_SCROLL        11
# define OBJ_BOOK          12
# define OBJ_FLASK         13
# define OBJ_GOLD          14
# define OBJ_AMMO          15
# define OBJ_FOOD          16
# define OBJ_WAND          17
# define OBJ_CONTAINER     18

#ifdef __cplusplus
extern "C"{
#endif

extern const char *OBJ_TYPES[];
extern int OBJ_VALS[];
extern char OBJ_CHAR[];

extern const char* spice[];

typedef struct item item;
struct  item{
        char *name,*desc;
        uint8_t type,color;
        uint16_t hit_bonus,weight,attr,val,dodge,defense,speed;
        void* damage;
};

void init_item(item* i);
void item_factory(void* i_ptr,item* i);
void free_item(item* i);
void print_item(item* i);

#ifdef __cplusplus
}
#endif

#endif