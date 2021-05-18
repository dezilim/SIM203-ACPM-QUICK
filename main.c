// Compilation:
//   icc -std=c99 main.c citiesReader.c prim.c
//   gcc main.c citiesReader.c prim.c -lm

// Execution:
//   ./a.out
//   ./a.exe


#include "citiesReader.h"
#include "prim2.h"
#include <time.h>       // for clock_t, clock(), CLOCKS_PER_SEC
#include <unistd.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define R 6378
#define p 0.017453292519943295 // pi/180

int main() {

//-----------------------------------------------------------------
//--- READING cities
//-----------------------------------------------------------------

  int popMin; int choice;
  int dep = 0;

  // if big cities only, minimal population is used for the sub cities
  printf("Connect all cities of min pop : press 0; \nConnect all cities of min pop in a certain department : press 1;\nBig cities only : press 2\n");
  scanf("%i", &choice);

  

  ListOfCities* cities;
  //int** ListOfParents = malloc(94*sizeof(int*));

  // connect all min pop cities
  if (choice == 0) 
  { 
    printf("Minimal population? ");
    scanf("%i", &popMin);
    cities = citiesReader(popMin, dep, false);
  }
  // connect all min pop cities in a department
  else if (choice == 1)
  {
    printf("Which department? ");
    scanf("%i", &dep);
    printf("Minimal population? ");
    scanf("%i", &popMin);
    cities = citiesReader(popMin, dep, false);
  }

  // calculate big cities only
  else if (choice == 2)
  {
    cities = citiesReader(popMin, dep, true);   
  }

  // to store the execution time of code
  double time_spent = 0.0;
  clock_t begin = clock();
  

  //int* parent = malloc(V*sizeof(*parent));

  // float graph_test[5][5] = {{0,2,0,6,0},{2,0,3,8,5},{0,3,0,0,7},{6,8,0,0,9},{0,5,7,9,0}};
  // float *graph_test2[5] = {graph_test[0], graph_test[1], graph_test[2], graph_test[3], graph_test[4]};
  // int* parent = primalgo(V, graph_test2);

  int* parent = primalgo2(cities);

  

 
  clock_t end = clock();
  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
 
  printf("\n\nThe elapsed time is %f seconds\n\n", time_spent);
  printf("Total weight of the graph: %d\n", parent[0]);
  printf("___________________________________________");

//-----------------------------------------------------------------
//--- COMPUTING graph
//-----------------------------------------------------------------

  FILE* fileOut = NULL;
  fileOut = fopen("resuGraph.dat", "w");
  for(int i=1; i < cities->number; i++){
    fprintf(fileOut, "%i %i\n", i, parent[i]);
  }

  fclose(fileOut);

//-----------------------------------------------------------------
//--- DEALLOCATE arrays
//-----------------------------------------------------------------

  free(cities->name);
  free(cities->pop);
  free(cities->lon);
  free(cities->lat);
  free(cities->dep);
  free(cities);
  free(parent);

  return 0;
}
