#ifndef FILE_READER_H
#define FILE_READER_H

#ifdef __cplusplus
extern "C"{
#endif

#include "roll.h"
#include "monster.h"
#include "item_template.h"

#define BEGIN_M "BEGIN MONSTER"
#define NAME "NAME"
#define SYMBOL "SYMB"
#define COLOR "COLOR"
#define DESC "DESC"
#define SPEED "SPEED"
#define DAMAGE "DAM"
#define HEALTH "HP"
#define ABILITY "ABIL"
#define ATTR "ATTR"
#define DODGE "DODGE"
#define VALUE "VAL"
#define DEFENSE "DEF"
#define HIT "HIT"
#define WEIGHT "WEIGHT"
#define TYPE "TYPE"

#define END_M "END"
#define BEGIN_I "BEGIN OBJECT"

extern const char* NPC_ABILITIES[];
extern const char* NPC_COLORS[];

class file_reader{
    char *file_name;
    char *m_file_name;
    public:
        file_reader();
        file_reader(const char* file,const char* m_file);
        ~file_reader();
        void print_file();
        void get_items_objects(std::vector<monster*> &mons,std::vector<item_template*> &items);
        void parse_monster(std::ifstream &file,std::string &line,monster &m);
        void parse_color(monster &m);
        void parse_name(monster &m);
        void parse_symbol(monster &m);
        void parse_desc(monster &m, std::ifstream &file,std::string &line);
        void parse_attrib(int* base, int* num, int* mod);
        void parse_characteristics(monster &m);
        void parse_item_template(std::ifstream &file, std::string &line,item_template &i);
        void parse_name(item_template &it);
        void parse_color(item_template &it);
        void parse_item_type(item_template &i);
        void parse_desc(item_template &it, std::ifstream &file,std::string &line);
};


#ifdef __cplusplus
}
#endif

#endif