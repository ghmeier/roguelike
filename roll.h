#ifndef ROLL_H
#define ROLL_H

#ifdef __cplusplus
extern "C"{
#endif

class roll{
    int base;
    int num;
    int mod;

    public:
        roll();
        roll(int base,int num, int mod);
        ~roll();
        int make_roll();
        void print_roll();

};

#ifdef __cplusplus
}
#endif

#endif