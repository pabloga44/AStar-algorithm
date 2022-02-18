#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdbool.h>
#include <math.h>

#define EARTH_RADIUS 6371008.8 //mean earth radius (meters)
#define PI 3.14159

//void *(*my_function) (double, unsigned long, const char *)

typedef struct {
	unsigned long id; // Node identification
	char *name;
	double lat, lon; // Node position
	unsigned short nsucc; // Number of node successors; i. e. length of successors
	unsigned long *successors;
} node;

unsigned long binarysearch(unsigned long ident, node l[],unsigned long n);
void ExitError(const char *miss, int errcode);
double haversine(node *n1, node *n2);
double spherical_cos(node *n1, node *n2);
double equirect_aprox(node *n1, node *n2);

#endif
