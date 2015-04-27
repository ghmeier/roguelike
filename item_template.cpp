#include <iostream>
#include "stdint.h"
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

#include "item_template.h"

using namespace std;

item_template::item_template(){
    char end ='\0';
    this->name = (char*)malloc(sizeof(char));
    *this->name = end;
    this->desc = (char*)malloc(sizeof(char));
    *this->desc = end;
    this->color = 0;
}

roll& item_template::get_hit_bonus(){
    return this->hit_bonus;
}

roll& item_template::get_weight(){
    return this->weight;
}

roll& item_template::get_damage(){
    return this->damage;
}

roll& item_template::get_attr(){
    return this->attr;
}

roll& item_template::get_val(){
    return this->val;
}

roll& item_template::get_dodge(){
    return this->dodge;
}

roll& item_template::get_defense(){
    return this->defense;
}

roll& item_template::get_speed(){
    return this->speed;
}

char* item_template::get_name(){
    return this->name;
}

char* item_template::get_desc(){
    return this->desc;
}

vector<char*> item_template::get_type(){
    return this->type;
}

uint8_t item_template::get_color(){
    return this->color;
}

void item_template::set_name(char* n){
    free(this->name);
    this->name = strdup(n);
}

void item_template::set_desc(const char* d){
    free(this->desc);
    this->desc = strdup(d);
}

void item_template::set_color(uint8_t c){
    this->color = c;
}

void item_template::set_item_roll(int base,int num, int mod,roll &ro){
    roll r(base,num,mod);
    ro = r;

}

void item_template::add_type(const char* s){
    char *temp = strdup(s);
    this->type.push_back(temp);
    //free(temp);
}

void item_template::print_item_template(){
    cout << this->name << endl;
    cout << this->desc;

    if (this->type.size() > 0){
        uint8_t i;
        for (i=0;i<this->type.size();i++){
            cout << this->type[i] << " ";
        }
        cout << endl;
    }

    cout << this->color << endl;

    this->hit_bonus.print_roll();
    this->damage.print_roll();
    this->dodge.print_roll();
    this->defense.print_roll();
    this->weight.print_roll();
    this->speed.print_roll();
    this->attr.print_roll();
    this->val.print_roll();
    cout << endl;

}

item_template::~item_template(){

    free(this->desc);
    free(this->name);

    if (this->type.size() > 0){
        uint8_t i;
        for (i=0;i<this->type.size();i++){
            free(this->type[i]);
        }
    }
}