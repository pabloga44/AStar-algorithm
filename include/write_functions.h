#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct {
	unsigned long id; // Node identification
	char *name;
	double lat, lon; // Node position
	unsigned short nsucc; // Number of node successors; i. e. length of successors
	unsigned long *successors;
} node;

void counter(FILE *stream, size_t max_len, unsigned long *nnodes, unsigned long *nways);
void Constructing_vector_nodes (FILE *stream, size_t max_len, unsigned long nnodes, node *nodes);
bool binarysearch(unsigned long ident, node l[],unsigned long n, unsigned long *position);
void NodesConnect(unsigned long previous_pos, unsigned long present_pos, node l[]);
void Create_arrows(FILE *stream, size_t max_len, unsigned long nnodes, node *nodes);
void Check_valence (unsigned long nnodes, node *nodes);
void WriteBinaryFile (char *argv1, unsigned long nnodes, node *nodes);
void ExitError(const char *miss, int errcode);

#endif
