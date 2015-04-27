#include "queue.h"

void init_queue(queue* q, uint16_t size){
	q->heap = (q_node**)malloc(size*sizeof(q_node*));
	q->max = size;
	q->cur = 0;
}

void free_queue(queue* q){
	int i=1;
	while (i<q->cur){
		if(q->heap[i]->p){
			//free(q->heap[i].p);
		}
		i++;
	}
	free(q->heap);
	free(q);
}

void clear_queue(queue* q){
	q->cur = 0;	
}

void add_with_priority(queue* q, q_node* p, uint16_t dist){

	//add to size if it's less
	if (q->cur >= q->max-1){
		q->max = q->max * 2;
		q->heap = realloc(q->heap, q->max * sizeof(q_node*));
		//printf("This is bad\n");
	}

	q->cur++;
	int i = q->cur,j;
	while ((j = i/2) && q->heap[j]->pri > dist){
		q->heap[i] = q->heap[j];
		q->heap[i]->index = i;
		i = j;
	}
	q->heap[i] = p;
	q->heap[i]->pri = dist;
	q->heap[i]->index = i;
}

int pop_min(queue* q, q_node* qn){

	//no item to pop
	if (q->cur < 1){
		return 1;
	}

	qn->p= q->heap[1]->p;
	qn->pri = q->heap[1]->pri;

	int i = 1,j;	

	--q->cur;
	while ((j = i * 2) && j < q->cur){
		//if right leaf is more, select that one
		if (j+1 < q->cur && q->heap[j]->pri > q->heap[j+1]->pri){
			j++;
		}

		//if current pri is less than next, we're done here
		if (q->heap[q->cur]->pri < q->heap[j]->pri){
			break;
		}
		q->heap[i] = q->heap[j];
		q->heap[i]->index = i;
		i = j;
	}
	q->heap[i] = q->heap[q->cur+1];
	q->heap[i]->index = i;
	return 0;
}

int get_path(queue* q, int* pos){
	int i=1;
	for(i=1;i<=q->cur;i++){
		if (q->heap[i]->p->pos[0] == pos[0] && q->heap[i]->p->pos[1] == pos[1]){
			return i;
		}
	}
	return -1;
}	


void print_queue(queue* q){
	int i=1;
	while (i <= q->cur){
		printf("%d pos: %d,%d from: %d,%d| ",q->heap[i]->pri,q->heap[i]->p->pos[0],q->heap[i]->p->pos[1],q->heap[i]->p->from[0],q->heap[i]->p->from[1]);
		i++;
	}
	printf("\n");
}