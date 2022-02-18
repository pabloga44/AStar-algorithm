#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../include/astar_functions.h"

unsigned long binarysearch(unsigned long ident, node l[],unsigned long n){
    // Searches the position of a node in the vector given its id and stores it in *position
    unsigned long first = 0;
    unsigned long last = (n - 1);
    unsigned long middle = (first+last)/2 ;
    unsigned long position;

    while (first <= last) {
        if (l[middle].id < ident)
            first = middle + 1; //buscamos en la segunda mitad
        else if (l[middle].id == ident) {
            position = middle;
            return position; //Ya sabemos la posici�n del nodo
        }
        else //Buscamos en la primera mitad
            last = middle - 1;
            middle = (first + last)/2;
    }
    return -1;
}

void ExitError(const char *miss, int errcode) {
    fprintf (stderr, "nnERROR: %s.nnStopping...nnnn", miss); exit(errcode);
}

double haversine(node *n1, node *n2) {
    double lat1 = n1 -> lat, lon1 = n1 -> lon, lat2 = n2 -> lat, lon2 = n2 -> lon;

    const double R = 6371e3;
    const double phi1 = lat1 * M_PI / 180.0, phi2 = lat2 * M_PI / 180.0;
    const double lam1 = lon1 * M_PI / 180.0, lam2 = lon2 * M_PI / 180.0;

    const double dphi = phi2 - phi1;
    const double dlam = lam2 - lam1;

    const double a = sin(dphi / 2.0) * sin(dphi / 2.0) + cos(phi1) * cos(phi2) * sin(dlam / 2.0) * sin(dlam / 2.0);
    const double c = 2 * atan2(sqrt(a), sqrt(1.0 - a));
    return R * c;
}

double spherical_cos(node *n1, node *n2){
    double lat1 = n1 -> lat, lon1 = n1 -> lon, lat2 = n2 -> lat, lon2 = n2 -> lon;

    const double R = 6371e3;
    const double phi1 = lat1 * M_PI / 180.0, phi2 = lat2 * M_PI / 180.0;
    const double lam1 = lon1 * M_PI / 180.0, lam2 = lon2 * M_PI / 180.0;

    const double dlam = lam2 - lam1;
    
    return acos( sin(phi1)*sin(phi2) + cos(phi1)*cos(phi2) * cos(dlam) ) * R;
}

double equirect_aprox(node *n1, node *n2){
    double lat1 = n1 -> lat, lon1 = n1 -> lon, lat2 = n2 -> lat, lon2 = n2 -> lon;

    const double R = 6371e3;
    const double phi1 = lat1 * M_PI / 180.0, phi2 = lat2 * M_PI / 180.0;
    const double lam1 = lon1 * M_PI / 180.0, lam2 = lon2 * M_PI / 180.0;

    const double x = (lam2-lam1) * cos((phi1+phi2)/2);
    const double y = (phi2-phi1);

    return sqrt(x*x + y*y) * R;
}