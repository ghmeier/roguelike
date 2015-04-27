#include <string>
#include <cstdlib>
#include <iostream>

#include "roll.h"
#include "middle.h"

int c_roll(void* ro){
    if (ro){
        return ((roll*)ro)->make_roll();
    }

    return roll(4,1,0).make_roll();

}

void get_default_roll(void* ro){
    roll *r = new roll(4,1,0);
    ro = (void*)malloc(sizeof(roll));
    *((roll*)ro) = *r;
    delete r;
}

roll::roll(){
    this->base = 0;
    this->num = 0;
    this->mod = 0;
}

roll::roll(int base,int num, int mod){
    this->base = base;
    this->num = num;
    this->mod = mod;
}

int roll::make_roll(){
    int i=0;

    int final = this->mod;

    for (i=0;i<this->num;i++){
        if (this->base > 0){
            final+=rand()%this->base + 1;
        }
    }

    return final;
}

void roll::print_roll(){

    std::cout << this->mod << "+" << this->num << "d" << this->base << std::endl;
}


roll::~roll(){

}