#ifndef PRIM2_H
#define PRIM2_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


int minNodeIndex(int distTo[], bool inSet[], int V, int INF);

void printList_float(float myList[], int V);

void printList_int(int myList[], int V);

void printList_bool(bool myList[], int V);

int* primalgo2(ListOfCities* cities);

float x_ij(ListOfCities* cities, int i, int j);


#endif