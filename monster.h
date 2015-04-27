#ifndef MONSTER_H
#define MONSTER_H

#ifdef __cplusplus
extern "C"{
#endif

#include "roll.h"


class monster{
    private:
        uint8_t color;
        roll *damage;
        roll *hp;
        roll *speed;
        std::vector<char*> characteristics;
        char* description;
        char* name;
        char symbol;

    public:
        monster();
        ~monster();
        void print_monster();
        void set_color(uint8_t col);
        void set_damage(int base,int num,int mod);
        void set_hp(int base,int num,int mod);
        void set_speed(int base,int num,int mod);
        void set_characteristics(std::vector<char*>);
        void set_description(const char * desc);
        void set_name(char *name);
        void set_symbol(char symbol);
        void push_charac(char* charac);
        roll* get_damage(){ return this->damage;}
        roll* get_hp(){ return this->hp;}
        roll* get_speed(){ return this->speed;}
        uint8_t get_color(){return this->color;}
        char* get_description(){return this->description;}
        char* get_name(){ return this->name;}
        char  get_symbol(){return this->symbol;}
        std::vector<char*> get_characteristics(){return this->characteristics;}
};


#ifdef __cplusplus
}
#endif

#endif