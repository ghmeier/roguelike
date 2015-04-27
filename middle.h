#ifndef MIDDLE_H
#define MIDDLE_H

#include "npc.h"

#ifdef __cplusplus
extern "C"{
#endif

void npc_factory(void* monster,npc* npc);
int c_roll(void* ro);
void get_default_roll(void* ro);

#ifdef __cplusplus
}
#endif

#endif