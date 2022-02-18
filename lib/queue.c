#include "../include/queue.h"
# include <stdio.h>
# include <string.h>
# include <stdlib.h>

typedef struct element {
    void *data;
    double priority;
    struct element *parent, *left_child, *right_child;
    int nleft, nright, n_children; 
} element;

typedef struct queue {
    element *root;
    unsigned long length;
} queue;


queue *create_queue() {
    queue *q = (queue *) malloc(sizeof(queue));
    if (!q) {
        perror("Failed to malloc the queue\n");
        exit(1);
    }

    q -> root = NULL;
    q -> length = 0ul;
    return q;
}

element *create_element(void *data, double priority) {
    element *elm = (element *) malloc(sizeof(element));
    if (!elm) {
        perror("Failed to malloc the element\n");
        exit(1);
    }

    elm -> data = data;
    elm -> priority = priority;
    elm -> parent = NULL;
    elm -> right_child = NULL;
    elm -> left_child = NULL;
    elm -> n_children = 0;
    elm -> nleft = 0;
    elm -> nright = 0;

    return elm;
}

void preorder_print_recursive(element *node, int depth) {
    // For printing a queue, the node argument must be the root node
    if (node == NULL) return;

    // Visit (print) node
    int i;
    for (i = 0; i < depth; i++)  printf("\t");
    printf("P: %.2f,  %d hijos, nleft: %d, nright: %d\n\n", node -> priority, node -> n_children, node -> nleft, node -> nright);

    // Visit left
    preorder_print_recursive(node ->left_child, depth + 1);

    //Visit right
    preorder_print_recursive(node ->right_child, depth + 1);

    return;
}

void print_queue(queue *q) {
    printf("\n");
    preorder_print_recursive(q -> root, 0);
    printf("\n");
}

bool is_hierarchy_preserved(element *parent, element *child){
    if (parent -> priority < child -> priority) return true;
    return false;
}

void enqueue(queue *q, void *data, double priority) {
    // Search for the position of the new node (shape consistency)
    element *new_node = create_element(data, priority);
    if (!q -> root) {
        q -> root = new_node;
        q -> length++;
        return;
    }
    element *node = q -> root; // The first node is the root

    while (node->n_children == 2) {
        //printf("%.2f has %d left and %d right\n", node -> priority, node->nleft, node->nright);
        // Go to the next child
        if (node -> nleft > node -> nright) {
            node->nright++;
            node = node -> right_child;
        } 
        else if (node -> nleft == node -> nright) {
            node->nleft++;
            node = node -> left_child;
        } 
        else{
            perror("Enqueue: Error in queue, n right greater than n left\n");
            exit(2);
        }
    }
    // Append child
    new_node -> parent = node;
    q -> length++;

    if (node -> left_child == NULL)  {
        node -> left_child = new_node;
        node->nleft++;
        //printf("Put %.2f at left of %.2f\n", priority, node -> priority);
    }
    else {
        node -> right_child = new_node;
        node->nright++;
        //printf("Put %.2f at right of %.2f\n", priority, node -> priority);
    }
    
    node -> n_children++;
    

    // Check and modify ordenation (value consistency)
    element *dad = node;
    element *son = new_node;
    while (dad != NULL) {
        if(!is_hierarchy_preserved(dad, son)) {
            // Change the information of the element, not the elements themselves
            // WARNING: This could be a problem if we use pointers outside this functions
            // because now they are pointing a different element than before
            void *dad_data = dad->data;
            double dad_priority = dad->priority;

            dad->data = son->data;
            dad->priority = son->priority;
            son->data = dad_data;
            son->priority = dad_priority;
        }
        // Update
        son = dad;
        dad = dad->parent;
    }
}

bool dequeue(queue *q, void **data) {
    // Get the top element
    element *top = q -> root;

    // Check if the queue is empty
    if (!top) {
        printf("Empty queue\n");
        return false;
    }

    // Write the top's data
    *data = top->data;
    //printf("Priority of dequeued: %.2f\n", top->priority);

    // Rearrange the heap
    // First search for the element we need to put at the top
    // The search is the same as in enqueue but reversed
    element *node = top;
    while (node->n_children > 0) {
        // Go to the next child
        if (node -> nleft > node -> nright)  {
            node -> nleft--;
            node = node -> left_child;
        }
        else if (node -> nleft == node -> nright)  {
            node -> nright--;
            node = node -> right_child;
        }
        else{
            perror("Dequeue: Error in queue, n right greater than n left\n");
            exit(2);
        }
    }
    if (node == top) { // If there is only one element
        q -> root = NULL;
        q -> length--;
        return true;
    }
    // Put the information of the leaf node on the top
    top -> data = node -> data;
    top -> priority = node -> priority;

    
    // Remove the arrow of the parent
    element *dad = node -> parent;
    if (node == dad ->left_child) dad->left_child = NULL;
    else if (node == dad ->right_child) dad->right_child = NULL;
    else{
        perror("Inconsistancy in queue\n");
        exit(3);
    }
    dad -> n_children--;
    q -> length--;
    free(node);

    // Check for value inconsistancies from up to down
    // Look for the smaller child and swap
    node = top;
    while ((node -> n_children) > 0){
        //printf("Priority: %.2f\n",node->priority);
        //print_queue(q);

        element *best_child = node -> left_child;
        element *other_child = node -> right_child;
        if ((other_child != NULL) && ( !is_hierarchy_preserved(best_child, other_child)) ){
            best_child = other_child;
        }
        // Check if we need to change
        if ( !is_hierarchy_preserved(node, best_child) ) {
            void *dad_data = node->data;
            double dad_priority = node->priority;

            node->data = best_child->data;
            node->priority = best_child->priority;
            best_child->data = dad_data;
            best_child->priority = dad_priority;
        }
        else { 
            return true;
        }
        // Update
        node = best_child;
    }
    // If we arrived to a leaf node
    return true;
}

int get_queue_size(queue *q){
    return q -> length;
}