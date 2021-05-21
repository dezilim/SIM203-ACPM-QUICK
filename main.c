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
  printf("Connect all cities of min pop : press 0; \nConnect all cities of min pop in a certain department : press 1;\nBig cities only : press 2\nFull connection of Big cities and also Departments of min pop : press 4\n");
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
    for (int i =0; i < cities->number; i++) 
    {
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
    // to store the execution time of code
    double time_spent = 0.0;
    clock_t begin = clock();

    int* parent = primalgo2(cities);

    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
 
    printf("Total weight of the graph: %d\n", parent[0]);
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
    printf("Choice 4\n");
    // lets do the big cities first 
    ListOfCities* cities = citiesReader(popMin, dep, true, 0, outputFile);  
    int nb_bigCities = cities->number;
    int* parent = primalgo2(cities);

 
    printf("Total weight of the graph: %d\n", parent[0]);
    printf("___________________________________________\n");

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

    for (int dep = 1; dep < 96; dep++)
    {
      if (dep != 20)
      {
        printf("DEPARTMENT : %i, current offset = %i\n\n", dep, offset);
        ListOfCities* cities = citiesReader(popMin, dep, false, offset, outputFile);  
        int nb_currDep = cities->number;
        int* parent = primalgo2(cities);

    
        printf("Total weight of the graph: %d\n", parent[0]);
        printf("___________________________________________");

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
    






  }
  fclose(fileOut);
  fclose(outputFile);

  

  return 0;
}
