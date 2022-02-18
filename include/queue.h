#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef struct element element;
typedef struct queue queue;

queue *create_queue();
void enqueue(queue *q, void *data, double priority);
bool dequeue(queue *q, void **data);
void print_queue(queue *q);
int get_queue_size(queue *q);


#endif