#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct path path;
struct path{
	int from[2];
	int pos[2];
	uint16_t cost;
};

typedef struct q_node q_node;
struct q_node{
	uint16_t pri;
	uint16_t index;
	path* p;
};

typedef struct queue queue;
struct queue{
	q_node** heap;
	uint16_t max;
	uint16_t cur;
};

void add_with_priority(queue* queue,q_node* node, uint16_t dist);
int pop_min(queue* q,q_node* qn);
void decrease_priority(queue* queue, q_node* node, uint16_t dist);
int get_path(queue* q,int* pos);
void init_queue(queue* q, uint16_t size);
void print_queue(queue* q);
void free_queue(queue* q);
void clear_queue(queue* q);

#endif