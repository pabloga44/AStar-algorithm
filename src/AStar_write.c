# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <time.h>
# include "../include/write_functions.h"

int main(int argc, char *argv[]) {
    clock_t total_start = clock();

    // Open the file
    FILE *f;
    f = fopen("data/spain.csv", "r");
    if (f == NULL) {
        perror("Could not open the file.\n");
        return(1);
    }

    // Count number of nodes and ways
    size_t max_len = 79857;
    unsigned long nways = 0;
    unsigned long nnodes = 0;
    counter(f, max_len, &nnodes, &nways);

    // Construct the vector of nodes

    node *nodes = (node *) malloc(nnodes*sizeof(node)); //allocating memory for this vector
    // Check allocation of nodes
    if (nodes == NULL) {
        printf("Error in the memory allocation of nodes.\n");
        return(2);
    }

    Constructing_vector_nodes (f, max_len, nnodes, nodes);

    // Construct the ways
    Create_arrows(f, max_len, nnodes, nodes);

    // Check the valence
    Check_valence (nnodes, nodes);

    fclose(f);

    // Write the binary file
    if (argc > 1) WriteBinaryFile(argv[1], nnodes, nodes);
    clock_t total_end = clock();
    double total_secs = ((double) (total_end - total_start)) / CLOCKS_PER_SEC;
    printf("Total time of the first part: %.2f seconds\n\n\n", total_secs);

    return 0;
}