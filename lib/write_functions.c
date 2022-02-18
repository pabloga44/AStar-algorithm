# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <time.h>
# include "../include/write_functions.h"

/*  Counter
 *
 *  Input:
 *      *stream -> file which we are going to iterate
 *       max_len -> lengh of the longest line inside the file
 *      *nnodes, *nways -> parameters passed by reference.
 *   Counter function iterates over the lines of the file and
 *   counts the number of nodes and ways in the file, changing
 *   that way the content of variables nnodes and nways.
 *
 *  Return: number of nodes (nnodes) and number of ways (nways)
 */

void counter(FILE *stream, size_t max_len, unsigned long *nnodes, unsigned long *nways){

    clock_t counter_start = clock();
    printf("Counting the number of nodes and ways...\n\n");

    char *line = NULL;
    unsigned long counter = 0ul;
    unsigned short skippable = 3;

     while (getline(&line, &max_len, stream) != -1) {
        if (counter++ < skippable) continue;
    	char *token = strsep(&line, "|");
        if (strcmp(token, "node") == 0) (*nnodes)++;
        else if (strcmp(token, "way") == 0) (*nways)++;
        else break;
        line = NULL;
    }
    rewind(stream); // Restart reading the file
    printf("\tNumber of nodes: %lu\n", *nnodes);
    printf("\tNumber of ways: %lu\n", *nways);
    clock_t counter_end = clock();
    double read_secs = ((double) (counter_end - counter_start)) / CLOCKS_PER_SEC;
    printf("\tNodes and ways successfully counted in %.2f seconds\n\n", read_secs);
    /* --------------------------------------- */
    return;
}
/*  Constructing_vector_nodes
 *
 *  Constructing_vector_nodes iterates over nodes' field and stores
 *  each node's id, name, latitude and longitude inside the vector
 *
 *  Input:
 *      *stream -> file which we are going to iterate
 *       max_len -> lengh of the longest line inside the file
 *       nnodes -> number of nodes
 *      *nodes -> vector of node structures
 *
 *  Return: vector of node structures with id, name, lat and lon fields filled.
 */

void Constructing_vector_nodes (FILE *stream, size_t max_len, unsigned long nnodes, node *nodes){

    clock_t vectornodes_start = clock();
    printf("Constructing the vector of nodes...\n\n");

    unsigned short skippable = 3;
    unsigned long linecounter = 0ul, iter = 0ul;
    size_t i = 0;
    char *line = NULL;
    while (linecounter < nnodes && getline(&line, &max_len, stream) != -1) {

        // Skip 3 first lines
        if (iter++ < skippable)  continue;

        // Loop inside a line
        char *token = NULL;
        int n_field = 0;
        while((token = strsep(&line, "|")) != NULL){
            if (n_field == 1) nodes[i].id = strtoul(token, NULL, 10); // Id
            if (n_field == 2){ // Name
                nodes[i].name = (char *) malloc((strlen(token) + 1) * sizeof(char)) ;
                strcpy(nodes[i].name, token);
            }
            if (n_field == 9) nodes[i].lat = atof(token); // Latitude
            if (n_field == 10) nodes[i].lon = atof(token); // Longitude
            n_field++;
        }
        // Starting nsucc at 0
        nodes[i].nsucc = 0;

        i++;
        linecounter++;
        free(line);
    }

     printf("\tNodes stored: %d\n",linecounter);
     clock_t vectornodes_end = clock();
     double read_secs = ((double) (vectornodes_end - vectornodes_start)) / CLOCKS_PER_SEC;
     printf("\tVector of nodes successfully built in %.2f seconds\n\n", read_secs);
    /* --------------------------------------- */
     return;
}

/* binarysearch
 *
 * binarysearch searches the position of a node in the vector's node
 * given its id and stores it in *position
 *
 *  Input:
 *      ident -> id of the node from which we want to know its position
 *      *nodes -> vector struct of nodes
 *      n -> number of nodes
 *      *position -> parameter passed by reference.
 *
 *  Return: a bool, being true if node appears in the list of nodes,
 *  or false if it is not there. If true, returns its position in the vector
 *  of nodes
 */

bool binarysearch(unsigned long ident, node *nodes, unsigned long n, unsigned long *position){
    unsigned long first = 0; // starting position of the vector's node
    unsigned long last = (n - 1); // last position of the vector's node
    unsigned long middle = (first+last)/2 ; // middle position of the vector's node

    while (first <= last) {
        if (nodes[middle].id < ident)  // If node's id is bigger than middle node's id
            first = middle + 1; // then node must be searched in the right subarray

        else if (nodes[middle].id == ident) {  // If node's id is the same as middle's id
            *position = middle; // then fill its position
            return true;
        }
        else // node must be searched in the left subarray
            last = middle - 1;
            middle = (first + last)/2;
    }
    return false;
}

/* NodesConnect
 *
 * NodesConnect connect two given nodes A -> B, storing B
 * inside the succesors of A.
 *  Input:
 *      previous_pos -> position of a node A
 *      present_pos -> position of node B
 *      *nodes -> vector struct of nodes
 */

void NodesConnect(unsigned long previous_pos, unsigned long present_pos, node *nodes){
    unsigned short N = nodes[previous_pos].nsucc;
    int i;
     for (i = 0; i < N; i++) {
        if (nodes[previous_pos].successors[i] == present_pos) return; // exit if A and B are already connected.
    }

    if (N == 0){ // if there are still no successors
        nodes[previous_pos].successors = (unsigned long *) malloc(sizeof(unsigned long)) ; // malloc memory for one succ
    }
    else{ // realloc memory for one more succ
        nodes[previous_pos].successors = (unsigned long *) realloc(nodes[previous_pos].successors, (N + 1)*sizeof(unsigned long)) ;
    }
    nodes[previous_pos].successors[N] = present_pos; // connect A to B
    nodes[previous_pos].nsucc++; // add one more succesor

    return;
}

/* Create_arrows
 *
 * Create_arrows iterates over way's lines, and links each node
 * with its successors, constructing the graph that way.
 *  Input:
 *      *stream -> file which we are going to iterate
 *       max_len -> lengh of the longest line inside the file
 *       nnodes -> number of nodes
 *      *nodes -> vector of node structures
 */

void Create_arrows(FILE *stream, size_t max_len, unsigned long nnodes, node *nodes) {
    clock_t arrows_start = clock();
    printf("Creating arrows between nodes...\n\n");

    char *line = NULL;
    while (getline(&line, &max_len, stream) != -1){ // loop inside ways' lines.
    // no need to rewind since we already called Constructing_vector_nodes;

        char *token = NULL;
        int n_field = 0;
        bool oneway = false;
        char *present_id = NULL;
        unsigned long present_pos = 0ul;
        unsigned long previous_pos = ULONG_MAX;

        // Check if we start at way's field
        token = strsep(&line, "|");
        if (strcmp(token, "way") != 0) break;
        n_field++;

        // Read the rest of the line
        while ((token = strsep(&line, "|")) != NULL) {
            if (n_field == 7 && strcmp(token, "oneway") == 0) // see if there is oneway
                oneway = true;
            else if (n_field > 8) { // Conect the nodes
                if (binarysearch(strtoul(token, NULL, 10), nodes, nnodes, &present_pos)) { // if the node exists in the vector
                if (previous_pos != ULONG_MAX) {// If there is a previous
                        NodesConnect(previous_pos, present_pos, nodes); //always connect A --> B
                        if (!oneway) NodesConnect(present_pos, previous_pos, nodes); // if no oneway then also B --> A
                    }
                    previous_pos = present_pos; // Update
                }
            }
            n_field++;
        }
        free(line); // free memory
    }
    clock_t arrows_end = clock();
    double read_secs = ((double) (arrows_end - arrows_start)) / CLOCKS_PER_SEC;
    printf("\tGraph successfully created in %.2f seconds\n\n", read_secs);
    /* --------------------------------------- */
    return;
}
/* Check_valence
 *
 * Check_valence counts how many nodes have a
 * specific number of successors, and prints the result.
 * Imput:
 *       nnodes -> number of nodes
 *      *nodes -> vector of node structures
 */

void Check_valence(unsigned long nnodes, node *nodes) {
    clock_t valence_start = clock();
    // Counts the number of nodes with each number of succesors
    printf("Checking the valence...\n\n");

    unsigned long *valence;
    int N = 10; // Valence goes from 0 to 9
    int nsucc, k;
    size_t j;

    valence = (unsigned long*) calloc(N, sizeof(unsigned long)); // alloc memory for valence
    if (!valence) {
        perror("Couldn't allocate memory for valence\n");
        exit(1);
    }

    for (j = 0; j < nnodes; j++){
        nsucc = nodes[j].nsucc; // Get the number of successors for that node
        *(valence + nsucc) += 1; // add 1 node having nsucc successors.
    }

    for (k = 0; k < N; k++) // print the result
        printf("\tValence = %i, nnodes = %lu\n", k, valence[k]);

    printf("\n");
    free(valence);
    clock_t valence_end = clock();
    double read_secs = ((double) (valence_end - valence_start)) / CLOCKS_PER_SEC;
    printf("\tValence checked in %.2f seconds\n\n", read_secs);
    return;
}

void WriteBinaryFile(char *argv1, unsigned long nnodes, node *nodes){
    clock_t binary_start = clock();
    printf("Writing the binary file...\n\n");

    /* Computing the total number of successors */
    unsigned long ntotnsucc = 0UL;
    int i = 0;
    for(i=0; i < nnodes; i++) ntotnsucc += nodes[i].nsucc;

    /* Setting the name of the binary file */
    char name[256];
    strcpy(name, argv1);
    //strcpy(strrchr(name, '\0'), ".bin"); // Adding .bin at the end of name

    FILE *fin;
    if ((fin = fopen (name, "wb")) == NULL)
        ExitError("the output binary data file cannot be opened", 31);

    /* Global data - header */
    if (fwrite(&nnodes, sizeof(unsigned long), 1, fin) +
        fwrite(&ntotnsucc, sizeof(unsigned long), 1, fin) != 2)
            ExitError("when initializing the output binary data file", 32);

    /* Writing all nodes */
    if (fwrite(nodes, sizeof(node), nnodes, fin) != nnodes)
        ExitError("when writing nodes to the output binary data file", 32);

    /* Writing sucessors in blocks */
    for (i = 0; i < nnodes; i++)
        if (nodes[i].nsucc)
            if (fwrite(nodes[i].successors, sizeof(unsigned long), nodes[i].nsucc, fin) != nodes[i].nsucc)
                ExitError("when writing edges to the output binary data file", 32);

    fclose(fin);
    clock_t binary_end = clock();
    double read_secs = ((double) (binary_end - binary_start)) / CLOCKS_PER_SEC;
    printf("\tBinary file successfully written in %.2f seconds\n\n", read_secs);
    return;
}

void ExitError(const char *miss, int errcode) {
fprintf (stderr, "nnERROR: %s.nnStopping...nnnn", miss); exit(errcode);
}