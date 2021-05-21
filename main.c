// Compilation:
//   icc -qopenmp -O0 -std=c99 -mkl main.c citiesReader.c prim2.c
//   icc -qopenmp -O1 -std=c99 -mkl main.c citiesReader.c prim2.c
//   icc -qopenmp -O2 -std=c99 -mkl main.c citiesReader.c prim2.c
//   icc -qopenmp -O3 -std=c99 -mkl main.c citiesReader.c prim2.c
//   gcc main.c citiesReader.c prim.c -lm

// Execution:
//   ./a.out
//   ./a.exe


#include "citiesReader.h"
#include "prim2.h"
#include <mkl.h>

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
  printf("Connect all cities of min pop : press 0\n");
  printf("Connect all cities of min pop in a certain department : press 1\n");
  printf("Big cities only : press 2\n");
  printf("Full connection of Big cities and also Departments of min pop : press 3\n");
  scanf("%i", &choice);

  

  
  ListOfCities* cities;

  FILE* outputFile = NULL;
  outputFile = fopen("resuCities.dat", "w");
  FILE* fileOut = NULL;
  fileOut = fopen("resuGraph.dat", "w");

  // connect all min pop cities
  if (choice == 0) 
  { 
    printf("Minimal population? ");
    scanf("%i", &popMin);
    cities = citiesReader(popMin, dep, false, 0,outputFile);
  }
  // connect all min pop cities in a department
  else if (choice == 1)
  {
    printf("Which department? ");
    scanf("%i", &dep);
    printf("Minimal population? ");
    scanf("%i", &popMin);
    cities = citiesReader(popMin, dep, false, 0,outputFile);
    for (int i =0; i < cities->number; i++){
      printf("CURRENT CITY  =====  %s %i %f %f %i\n", cities->name[i], cities->pop[i], cities->lon[i], cities->lat[i], cities->dep[i]);
    }
  }

  // calculate big cities only
  else if (choice == 2)
  {
    cities = citiesReader(popMin, dep, true, 0, outputFile);   
  }



  if (choice == 0 || choice == 1 || choice == 2)
  {
    // Start CHRONO
    const double timeBegin = dsecnd();

    int* parent = primalgo2(cities);

    // End CHRONO
    const double timeEnd = dsecnd();
    double timeTotal = timeEnd-timeBegin;

    printf("\n\nThe elapsed time is %f seconds\n\n", timeTotal);
    printf("Total weight of the tree: %d\n", parent[0]);
    printf("Total number of cities: %d\n", cities->number);
    printf("___________________________________________\n");

    //-----------------------------------------------------------------
    //--- COMPUTING graph
    //-----------------------------------------------------------------

    
    for(int i=1; i < cities->number; i++){
      fprintf(fileOut, "%i %i\n", i, parent[i]);
    }

    

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

  }

  // ---------------------------------------------------------------------
  // COMBINE SOLUTION OF BIG CITIES AND EACH DEPARTMENT
  // ---------------------------------------------------------------------
  else if (choice == 3)
  {
    printf("Minimal population? ");
    scanf("%i", &popMin);
    printf("Choice 3\n");
    // lets do the big cities first 
    ListOfCities* cities = citiesReader(popMin, dep, true, 0, outputFile);  
    int nb_bigCities = cities->number;
    
    // Start CHRONO
    const double timeBegin = dsecnd();

    int* parent = primalgo2(cities);

    int totalWeight = parent[0];   // the weight of the entirety of the graph

 
    // printf("Total weight of the tree: %d\n", parent[0]);
    // printf("___________________________________________\n");

    //-----------------------------------------------------------------
    //--- COMPUTING graph
    //-----------------------------------------------------------------

    FILE* fileOut = NULL;
    fileOut = fopen("resuGraph.dat", "w");
    for(int i=1; i < cities->number; i++){
      fprintf(fileOut, "%i %i\n", i, parent[i]);
    }


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

    // now lets run through each department
    int offset = nb_bigCities;
    int total_nbCitites = 0;   // total number of cities used in the tree

    for (int dep = 1; dep < 96; dep++){
      if (dep != 20){
        printf("DEPARTMENT : %i, current offset = %i\n\n", dep, offset);
        ListOfCities* cities = citiesReader(popMin, dep, false, offset, outputFile);  
        int nb_currDep = cities->number;
        int* parent = primalgo2(cities);

        totalWeight += parent[0]; // adding the weight of each tree
        total_nbCitites += cities->number;  // adding the number of cities in each departement

    
        // printf("Total weight of the tree: %d\n", parent[0]);
        // printf("___________________________________________");

        //-----------------------------------------------------------------
        //--- COMPUTING graph
        //-----------------------------------------------------------------

        // the offset is important for writing the file !!!
        for(int i = 1 ; i < nb_currDep; i++){
          fprintf(fileOut, "%i %i\n", i + offset, parent[i] + offset);
        }
        offset += nb_currDep;
        
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
      }
    }

    // End CHRONO after all the departement
    const double timeEnd = dsecnd();
    double timeTotal = timeEnd-timeBegin;

    printf("\n\nThe elapsed time is %f seconds\n\n", timeTotal);
    printf("Total weight of the tree: %d\n", totalWeight);
    
  }
  fclose(fileOut);
  fclose(outputFile);

  return 0;
}
