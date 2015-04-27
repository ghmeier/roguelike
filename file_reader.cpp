#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "stdint.h"

#include "file_reader.h"
#include "item.h"

const char* NPC_ABILITIES[] = {"SMART","TELEP"};
const char* NPC_COLORS[] = {"RED","GREEN","BLUE","YELLOW","CYAN","MAGENTA","WHITE","BLACK"};

    file_reader::file_reader(){
        this->file_name = (char*)malloc(sizeof(char));
        this->file_name = '\0';
    }

    file_reader::file_reader(const char *file,const char *m_file){
        this->file_name = strdup(file);
        this->m_file_name = strdup(m_file);
    }

    void file_reader::get_items_objects(std::vector<monster*> &mons,std::vector<item_template*> &items){
        std::string line;
        std::ifstream file(this->file_name);

        if (file.is_open()){
            while (!file.eof()){
                getline(file,line, '\n');
                if (strcmp(line.c_str(),BEGIN_I) == 0){
                    item_template *i = new item_template();
                    parse_item_template(file,line,*i);
                    items.push_back(i);
                }
            }
            file.close();

        }else {
            std::cout << "some error" << std::endl;
        }

        std::ifstream m_file(this->m_file_name);

        if (m_file.is_open()){
            while (!m_file.eof()){
                getline(m_file,line, '\n');
                if(strcmp(line.c_str(),BEGIN_M) == 0){
                    monster *m = new monster();
                    parse_monster(m_file,line,*m);
                    mons.push_back(m);
                }
            }
            m_file.close();

        }else {
            std::cout << "some error" << std::endl;
        }
    }

    void file_reader::print_file(){
        std::string line;
        std::ifstream file(this->file_name);

        if (file.is_open()){
            while (!file.eof()){
                getline(file,line, '\n');
                if (strcmp(line.c_str(),BEGIN_M) == 0){
                    //parse_monster(file,line);
                }else if(strcmp(line.c_str(),BEGIN_I) == 0){
                    //parse_item_template(file,line);
                }
            }
            file.close();

        }else {
            std::cout << "some error" << std::endl;
        }
    }

    void file_reader::parse_monster(std::ifstream &file,std::string &line,monster &m){

        while (strcmp(line.c_str(),END_M) != 0){

            getline(file,line,'\n');
            char *c_line = strdup(line.c_str());
            char *attrib = strtok(c_line," ");
            if (attrib != NULL){
                if (strcmp(COLOR,attrib)==0) {
                    parse_color(m);
                }else if(strcmp(NAME,attrib) == 0){
                    parse_name(m);
                }else if (strcmp(SYMBOL,attrib) == 0){
                    parse_symbol(m);
                }else if (strcmp(DESC,attrib) == 0){
                    parse_desc(m,file,line);
                }else if (strcmp(SPEED,attrib) == 0){
                    int base,num,mod;
                    parse_attrib(&base,&num,&mod);
                    m.set_speed(base,num,mod);
                }else if (strcmp(DAMAGE,attrib) == 0){
                    int base,num,mod;
                    parse_attrib(&base,&num,&mod);
                    m.set_damage(base,num,mod);
                }else if(strcmp(HEALTH,attrib) == 0){
                    int base,num,mod;
                    parse_attrib(&base,&num,&mod);
                    m.set_hp(base,num,mod);
                }else if (strcmp(ABILITY,attrib) == 0){
                    parse_characteristics(m);
                }else if(strcmp(END_M,attrib) == 0){
                    free(c_line);
                    break;
                }
            }

            free(c_line);
        }
    }

    void file_reader::parse_item_template(std::ifstream &file, std::string &line,item_template &i){

        while(strcmp(line.c_str(),END_M) != 0){
            getline(file,line,'\n');

            char *c_line = strdup(line.c_str());
            char *attrib = strtok(c_line," ");
            int base,num,mod;

            if (strcmp(COLOR,attrib) == 0){
                parse_color(i);
            }else if (strcmp(NAME,attrib) == 0){
                parse_name(i);
            }else if (strcmp(DESC,attrib) == 0){
                parse_desc(i,file,line);
            }else if (strcmp(SPEED,attrib) == 0){
                parse_attrib(&base,&num,&mod);
                i.set_item_roll(base,num,mod,i.get_speed());
            }else if(strcmp(DAMAGE,attrib) == 0){
                parse_attrib(&base,&num,&mod);
                i.set_item_roll(base,num,mod,i.get_damage());
            }else if(strcmp(HIT,attrib) == 0){
                parse_attrib(&base,&num,&mod);
                i.set_item_roll(base,num,mod,i.get_hit_bonus());
            }else if(strcmp(WEIGHT,attrib) == 0){
                parse_attrib(&base,&num,&mod);
                i.set_item_roll(base,num,mod,i.get_weight());
            }else if(strcmp(ATTR,attrib) == 0){
                parse_attrib(&base,&num,&mod);
                i.set_item_roll(base,num,mod,i.get_attr());
            }else if(strcmp(DODGE,attrib) == 0){
                parse_attrib(&base,&num,&mod);
                i.set_item_roll(base,num,mod,i.get_dodge());
            }else if(strcmp(VALUE,attrib) == 0){
                parse_attrib(&base,&num,&mod);
                i.set_item_roll(base,num,mod,i.get_val());
            }else if (strcmp(DEFENSE,attrib) == 0){
                parse_attrib(&base,&num,&mod);
                i.set_item_roll(base,num,mod,i.get_defense());
            }else if(strcmp(TYPE,attrib) == 0){
                parse_item_type(i);
            }else{
                free(c_line);
                break;
            }

            free(c_line);
        }
    }

    void file_reader::parse_color(monster &m){
        char *color = strtok(NULL," ");
        uint8_t is_color = 0;
        uint8_t i;
        for (i=0;i<8;i++){
            if (strcmp(color,NPC_COLORS[i]) == 0){
                is_color = i;
                break;
            }
        }

        m.set_color(is_color);

    }

    void file_reader::parse_color(item_template &it){
        char *color = strtok(NULL," ");
        uint8_t is_color = 0;
        uint8_t i;
        for (i=0;i<8;i++){
            if (strcmp(color,NPC_COLORS[i]) == 0){
                is_color = i;
                break;
            }
        }

        it.set_color(is_color);

    }

    void file_reader::parse_attrib(int* base, int* num, int* mod){
        char *roll_str = strtok(NULL,"\n");

        char *smod = strtok(roll_str,"+");
        char *snum = strtok(NULL,"d");
        char *sbase = strtok(NULL,"\n");

        *base = atoi(sbase);
        *num = atoi(snum);
        *mod = atoi(smod);

    }

    void file_reader::parse_characteristics(monster &m){
        char * at;
        while ((at = strtok(NULL," "))){
            m.push_charac(at);
        }
    }

    void file_reader::parse_desc(monster &m, std::ifstream &file,std::string &line){
        std::string desc;
        while (strcmp(line.c_str(),".") != 0){
            getline(file,line,'\n');
            if (strcmp(line.c_str(),".") == 0){
                break;
            }
            desc = desc + line + '\n';
        }
        m.set_description(desc.c_str());
    }

    void file_reader::parse_desc(item_template &it, std::ifstream &file,std::string &line){
        std::string desc;
        while (strcmp(line.c_str(),".") != 0){
            getline(file,line,'\n');
            if (strcmp(line.c_str(),".") == 0){
                break;
            }
            desc = desc + line + '\n';
        }
        it.set_desc(desc.c_str());
    }

    void file_reader::parse_name(monster &m){
        char *name = strtok(NULL,"\n");
        m.set_name(name);
    }

    void file_reader::parse_name(item_template &it){
        char *name = strtok(NULL,"\n");
        it.set_name(name);
    }

    void file_reader::parse_symbol(monster &m){
        char *symb = strtok(NULL,"\n");
        m.set_symbol(*symb);
    }

    void file_reader::parse_item_type(item_template &i){
        char * at;
        while ((at = strtok(NULL," "))){
            i.add_type(at);
        }
    }

    file_reader::~file_reader(){
        free(this->file_name);
        free(this->m_file_name);
    }