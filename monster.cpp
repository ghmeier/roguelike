#include <string>
#include <cstdlib>
#include "stdint.h"
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>

#include "npc.h"
#include "monster.h"
#include "middle.h"

void npc_factory(void* mon, npc* npc){
    monster* m = (monster*)mon;

    npc->out = m->get_symbol();
    npc->color = m->get_color();
    npc->description = strdup(m->get_description());
    npc->name = strdup(m->get_name());
    npc->damage = (void*)malloc(sizeof(roll));
    *((roll*)npc->damage) = *m->get_damage();
    npc->speed = m->get_speed()->make_roll();
    npc->hp = m->get_hp()->make_roll();
    uint8_t temp = 0;
    std::vector<char*> types = m->get_characteristics();

    uint32_t i;
    for (i=0;i<types.size();i++){
        if (strcmp("SMART",types[i]) == 0){
            temp = temp | NPC_SMART;
        }else if (strcmp("TELEP",types[i])==0){
            temp = temp | NPC_TELEP;
        }
    }

    npc->type = temp;

}

monster::monster(){
    char end = '\0';
    this->color = 0;
    this->description = (char*)malloc(sizeof(char));
    *this->description = end;
    this->name = (char*)malloc(sizeof(char));
    *this->name = end;
    this->symbol = end;
}

void monster::print_monster(){
    std::cout << this->name << std::endl;
    std::cout << this->description;
    std::cout << this->symbol << std::endl;
    std::cout << this->color << std::endl;
    this->speed->print_roll();

    uint32_t i;
    for (i=0;i<this->characteristics.size();i++){
        std::cout << this->characteristics[i] << " ";
    }

    std::cout << std::endl;
    this->hp->print_roll();
    this->damage->print_roll();

}

void monster::set_color(uint8_t col){
    this->color = col;
}

void monster::set_description(const char* desc){
    free(this->description);
    this->description = strdup(desc);
}

void monster::set_name(char* name){
    free(this->name);
    this->name = strdup(name);
}

void monster::set_symbol(char symbol){
    this->symbol = symbol;
}

void monster::set_speed(int base, int num, int mod){
    roll *r = new roll(base,num,mod);
    this->speed = r;
}

void monster::set_hp(int base, int num, int mod){
    roll *r = new roll(base,num,mod);
    this->hp = r;
}

void monster::set_damage(int base, int num, int mod){
    roll *r = new roll(base,num,mod);
    this->damage = r;
}

void monster::push_charac(char *charac){
    char *temp = strdup(charac);
    this->characteristics.push_back(temp);
}

monster::~monster(){
    free(this->description);
    free(this->name);
    delete this->hp;
    delete this->speed;
    delete this->damage;

    uint32_t i;
    for(i=0;i<this->characteristics.size();i++){
        free(this->characteristics[i]);
    }
}