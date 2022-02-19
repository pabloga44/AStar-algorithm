#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include "../include/astar_functions.h"
#include "../include/queue.h"
#include <time.h>

//typedef double (*heuristic) (node *, node *);
#define HEURISTIC haversine

typedef struct QueueElementstruct {
    unsigned long v;
    struct QueueElementstruct *next;
} QueueElement;

typedef QueueElement * PriorityQueue; //Apunta al primero de la queu que defines

typedef struct a_node {
    double g, h;
	bool isOpen;
    node *my_node;
	struct a_node *parent;
} a_node;

bool AStar(node *nodes, unsigned long NNODES, unsigned long node_start, unsigned long node_goal, a_node **result, double (*fdistance)(node *, node *));

int main (int argc, char *argv[]) {
    //Start measuring the time
    clock_t total_start = clock();
    clock_t read_start = clock();

    // Read the binary file
    FILE *fin;
    unsigned long nnodes;
    unsigned long ntotnsucc = 0UL;
    unsigned long *allsuccessors;
    node *nodes;
    if ((fin = fopen (argv[1], "rb")) == NULL)
        ExitError("the data file does not exist or cannot be opened", 11);

    /* Global data −−− header */
    if( fread(&nnodes, sizeof(unsigned long), 1, fin) +
        fread(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2 )
        ExitError("when reading the header of the binary data file", 12);

    /* getting memory for all data */
    if((nodes = (node *) malloc(nnodes*sizeof(node))) == NULL)
        ExitError("when allocating memory for the nodes vector", 13);
    if((allsuccessors = (unsigned long *) malloc(ntotnsucc*sizeof(unsigned long))) == NULL)
        ExitError("when allocating memory for the edges vector", 15);

    /* Reading all data from file */
    if( fread(nodes, sizeof(node), nnodes, fin) != nnodes )
        ExitError("when reading nodes from the binary data file", 17);
    if(fread(allsuccessors, sizeof(unsigned long), ntotnsucc, fin) != ntotnsucc)
        ExitError("when reading sucessors from the binary data file", 18);
    fclose(fin);

    /* Setting pointers to successors */
    size_t i = 0;
    for(i=0; i < nnodes; i++) if(nodes[i].nsucc) {
        nodes[i].successors = allsuccessors; allsuccessors += nodes[i].nsucc;
    }

    clock_t read_end = clock();
    double read_secs = ((double) (read_end - read_start)) / CLOCKS_PER_SEC;
    printf("Binary file successfully read in %.2f seconds\n\n", read_secs);

    /* --------------------------------------- */

    // Calcule the optimal route with A*

    //Start measuring time of A*
    clock_t astar_start = clock();
    unsigned long node_start = binarysearch(771979683, nodes , nnodes); // Girona
    unsigned long node_goal = binarysearch(429854583, nodes , nnodes); // Lleida

    a_node *result;
    bool r = AStar(nodes, nnodes, node_start, node_goal, &result, HEURISTIC);
    if(!r) ExitError("no solution found in AStar", 7);

    clock_t astar_end = clock();
    double astar_secs = ((double) (astar_end - astar_start)) / CLOCKS_PER_SEC;

    /* --------------------------------------- */

    // Write in a file if a file is passed as argument
    if (argc > 2){
        a_node *iterator = result + node_goal;
        FILE *to_write = fopen(argv[2], "wb");
        while (iterator) {
            fprintf(to_write, "Id: %lu, distance: %f\n", iterator -> my_node -> id, iterator -> g);
            iterator = iterator -> parent;
        }
        fclose(to_write);
    }
    
    // Longitud, latitud file
    if (argc > 3){
        a_node *iterator = result + node_goal;
        FILE *to_write2 = fopen(argv[3], "wb");
        fprintf(to_write2, "Longitude, latitude\n\n");
        while (iterator) {
            fprintf(to_write2, "%f, %f\n", iterator -> my_node -> lon, iterator -> my_node -> lat);
            iterator = iterator -> parent;
        }
        fclose(to_write2);
    }

    /* --------------------------------------- */

    // Print the results
    printf("Total distance: %.2f\n", (result + node_goal) -> g);

    printf("Path found successfully in %.2f seconds\n", astar_secs);
    clock_t total_end = clock();
    double total_secs = ((double) (total_end - total_start)) / CLOCKS_PER_SEC;
    printf("Total time of the second part: %.2f seconds\n\n", total_secs);

    return 0;
}

bool AStar(node *nodes, unsigned long NNODES, unsigned long node_start, unsigned long node_goal, a_node **result, double (*fdistance)(node *, node *)){

    register unsigned i;
    queue *open = create_queue();
    a_node *a_nodes = (a_node *) malloc(NNODES * sizeof(a_node));

    if(!a_nodes) ExitError("when allocating memory for the AStar Control Data vector", 73);

    for(i=0; i < NNODES; i++) {
        a_nodes[i].isOpen = false;
        a_nodes[i].g = DBL_MAX;
        a_nodes[i].h = DBL_MAX;
        a_nodes[i].parent = NULL;
        a_nodes[i].my_node = nodes + i;
    }

    a_node *start = a_nodes + node_start, *goal = a_nodes + node_goal;
    start -> g = 0.0;
    start -> h = fdistance(start -> my_node, goal -> my_node);

    enqueue(open, start, start -> g);

    void *to_deq_curr;
    a_node *current = NULL;
    while(dequeue(open, &to_deq_curr) && (current = (a_node *) to_deq_curr) != goal){

        //if (current -> isOpen) continue;
        for(i=0; i < current -> my_node -> nsucc; i++) {
            a_node *successor = a_nodes + current -> my_node -> successors[i];
            double g = current -> g + fdistance(current -> my_node, successor -> my_node); // lo q llevamos + cost(actual, sucesor)
            if (g >= successor -> g || successor -> isOpen) continue;
            if (successor -> h == DBL_MAX) successor -> h = fdistance(successor -> my_node, goal -> my_node);

            successor -> g = g;
            successor -> parent = current;
            enqueue(open, successor, g + successor -> h);
        }
        current -> isOpen = true;
    }
    bool success = current == goal;
    free(open);
    if (result) *result = a_nodes;

    return success;
}




