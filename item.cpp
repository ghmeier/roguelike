#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include "stdint.h"

#include "item.h"
#include "item_template.h"

#define NUM_SPICE 8

using namespace std;

int OBJ_VALS[] = {OBJ_WEAPON, OBJ_OFFHAND, OBJ_RANGED, OBJ_ARMOR, OBJ_HELMET, OBJ_CLOAK, OBJ_GLOVES, OBJ_BOOTS, OBJ_RING, OBJ_AMULET, OBJ_LIGHT, OBJ_SCROLL, OBJ_BOOK, OBJ_FLASK, OBJ_GOLD, OBJ_AMMO, OBJ_FOOD, OBJ_WAND, OBJ_CONTAINER};
const char *OBJ_TYPES[] = {"WEAPON", "OFFHAND", "RANGED", "ARMOR", "HELMET","CLOAK", "GLOVES", "BOOTS", "RING", "AMULET", "LIGHT", "SCROLL", "BOOK", "FLASK", "GOLD", "AMMO", "FOOD", "WAND", "CONTAINER"};
char OBJ_CHAR[] = {'|',')','}','[',']','(','{','\\','=','\"','_','~','?','!','$','/',',','-','%','&'};

void init_item(item* i){
    char end ='\0';
    i->name = (char*)malloc(sizeof(char));
    *i->name = end;
    i->desc = (char*)malloc(sizeof(char));
    *i->desc = end;
    i->hit_bonus = i->weight = i->attr = i->val = i->dodge = i->defense = i->speed = 0;
}

void item_factory(void* i_ptr,item* i){
    item_template* it = (item_template*)i_ptr;
    i->name = strdup(it->get_name());
    i->desc = strdup(it->get_desc());
    i->color = it->get_color();
    vector<char*> types = it->get_type();
    i->type = 0;
    uint32_t p;
    for (p=0;p<types.size();p++){
        uint32_t j;
        for (j=0;j<NUM_OBJ_TYPES;j++){
            if (strcmp(types[p],OBJ_TYPES[j]) == 0){
                i->type = OBJ_VALS[j];
                break;
            }
        }
    }
    i->hit_bonus = it->get_hit_bonus().make_roll();
    i->damage = (roll*)malloc(sizeof(roll));
    *((roll*)i->damage) = it->get_damage();
    i->dodge = it->get_dodge().make_roll();
    i->defense = it->get_defense().make_roll();
    i->weight = it->get_weight().make_roll();
    i->speed = it->get_speed().make_roll();
    i->attr = it->get_attr().make_roll();
    i->val = it->get_val().make_roll();
}

void print_item(item* i){
    cout << i->name << endl;
    cout << i->desc;
    cout << i->color << endl;

    cout <<i->hit_bonus << endl;
    roll* temp = (roll*)(i->damage);
    temp->print_roll();
    cout << i->dodge << endl;
    cout << i->defense << endl;
    cout << i->weight << endl;
    cout << i->speed << endl;
    cout << i->attr << endl;
    cout << i->val << endl;
    cout << endl;
}

void free_item(item* i){
    free(i->name);
    free(i->desc);
    free(i->damage);
    free(i);
}