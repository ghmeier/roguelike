#ifndef ITEM_TEMPLATE_H
#define ITEM_TEMPLATE_H

#ifdef __cplusplus
extern "C"{
#endif
#include "stdint.h"
#include "roll.h"

class item_template{
    private:
        char *name;
        char *desc;
        std::vector<char*> type;
        uint8_t color;
        roll hit_bonus,weight,damage,attr,val,dodge,defense,speed;

    public:
        item_template();
        ~item_template();
        void set_name(char* n);
        void set_desc(const char* d);
        void set_color(uint8_t c);
        void set_item_roll(int base,int num, int mod, roll &ro);
        void add_type(const char* s);
        roll& get_hit_bonus();
        roll& get_weight();
        roll& get_damage();
        roll& get_attr();
        roll& get_val();
        roll& get_dodge();
        roll& get_defense();
        roll& get_speed();
        char* get_name();
        char* get_desc();
        std::vector<char*> get_type();
        uint8_t get_color();
        void print_item_template();
};

#ifdef __cplusplus
}
#endif

#endif