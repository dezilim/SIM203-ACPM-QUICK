#include "citiesReader.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


ListOfCities* citiesReader(int popMin, int dep, bool bigOnly){

  ListOfCities* cities = malloc(sizeof(*cities));

//--------------------------------------------------------------------
//--- READING cities with population greater than or equal to 'popMin'
//--------------------------------------------------------------------

  printf("== Reading cities with population >= %i from 'citiesList.csv' ==\n", popMin);

  FILE* inputFile = NULL;
  inputFile = fopen("citiesList.csv", "r");
  if(inputFile != NULL){

    char line[512];
    const char s[2] = ",";
    char *token;


    // Count the number of cities with population greater than or equal to 'popMin'
    cities->number = 0;
    

    int prevDep = 0; 
    while(fgets(line, 512, inputFile) != NULL){
      token = strtok(line, s);

      for(int i=0; i<1;  i++) token = strtok(NULL, s);
      int myDep = atof(token);

      for(int i=0; i<13; i++) token = strtok(NULL, s);
      int myPop = atoi(token);

      if(bigOnly && myDep <= 95)
      {
        if(myDep != prevDep)
        {
          cities->number++;
        }
        prevDep = myDep;
      }
      else if( !bigOnly && (myPop >= popMin) && (((dep != 0) && (myDep == dep)) || (dep == 0)))
      {
        // restrict only to the departments if dep is not null (0)
        cities->number++;
        //cities->dep[index] = myDep;
      }
    } 
    fseek(inputFile, 0, SEEK_SET);
    // expect 94 total cities since department 20 is excluded 
    printf("JUUST CHECKKKING , CITIES NUMBER : = %i\n\n\n", cities->number);

    // Allocate arrays
    cities->name = malloc(cities->number*sizeof(char*));
    cities->pop  = malloc(cities->number*sizeof(int));
    cities->lon  = malloc(cities->number*sizeof(float));
    cities->lat  = malloc(cities->number*sizeof(float));
    cities->dep  = malloc(cities->number*sizeof(int));
    
    
    // Reading/Saving data
    int index= 0; 

    prevDep = 0; 
    char *myName = malloc(32*sizeof(char));    



    
    // read every line
    while(fgets(line, 512, inputFile) != NULL){
      
      token = strtok(line, s);

      for(int i=0; i<1;  i++) token = strtok(NULL, s);  
      int myDep = atof(token);

      for(int i=0; i<2;  i++) token = strtok(NULL, s);
      strncpy(myName, token, 32);

      for(int i=0; i<11; i++) token = strtok(NULL, s);
      int myPop = atoi(token);

      for(int i=0; i<5;  i++) token = strtok(NULL, s);
      float myLon = atof(token);

      for(int i=0; i<1;  i++) token = strtok(NULL, s);
      float myLat = atof(token);

      
      
      //if((myPop >= popMin) && (((dep != 0) && (myDep == dep)) || (dep == 0)))
      //if(myPop >= popMin) 
      /*
      if(bigOnly && myDep <= 95)
      {
        if(myDep == prevDep)
        {
          // if you find a bigger ville, overwrite the data
          if  (myPop > myPop_dep)
          {
            //printf("Prev big pop : %i, New big pop : %i\n", myPop_dep, myPop);
            myPop_dep = myPop;
            myLon_dep = myLon;
            myLat_dep = myLat; 
            myDep_dep = myDep;
            //myName_dep = myName;
            //printf("Biggest city : %s\n", myName_dep);
            strncpy(myName_dep, myName, 32);
            //printf("************************************\n");
          }
            

          
        }
        // once we have finished running through the departments, store the max one 
        else if(myDep != prevDep)
        {
          //printf("CHANGE DEPARTMENT ===========================\n\n");
          // store the data of the previous dep we ran through
          if (prevDep != 0)
          {
            cities->name[index] = (char*) malloc(32*sizeof(char));
            strncpy(cities->name[index], myName_dep, 32);

            cities->pop[index] = myPop_dep;
            cities->lon[index] = myLon_dep;
            cities->lat[index] = myLat_dep;
            cities->dep[index] = myDep_dep;
        
            index++;     
          }
          // initialise the maximum population of the department as the first one 
          myPop_dep = myPop;  
          myLon_dep = myLon;
          myLat_dep = myLat; 
          myDep_dep = myDep; 
          //myName_dep = myName;
          strcpy(myName_dep, myName);

        }

        prevDep = myDep;
      } */

      
      if(bigOnly && myDep <= 95)
      {
        // initialise for a department
        
        //cities->pop[index] = myPop;
        
        if(myDep == prevDep)
        {
          // if you find a bigger ville, overwrite the data
          if  (myPop > cities->pop[index])
          {
            //printf("Prev big pop : %i, New big pop : %i\n", myPop_dep, myPop);
            cities->pop[index] = myPop;
            cities->lon[index] = myLon;
            cities->lat[index] = myLat;
            cities->dep[index] = myDep;
            //printf("Biggest city : %s\n", myName_dep);
            strncpy(cities->name[index], myName, 32);
            //printf("************************************\n");
          }
            

          
        }
        // keep track of finishing running through a department. Begin with this, allocate space for name and initialise values
        else if(myDep != prevDep) 
        {
          // allocate more space if we are going on to the next department 
          if (prevDep != 0) index++;
          // initialise the values for each department. To be overwritten if we find something better. 
          cities->name[index] = (char*) malloc(32*sizeof(char));
          cities->pop[index] = myPop;
          cities->lon[index] = myLon;
          cities->lat[index] = myLat;
          cities->dep[index] = myDep;
          strncpy(cities->name[index], myName, 32);
          
        }
        
        prevDep = myDep;
      }
      
      else if(!bigOnly && (myPop >= popMin) && (((dep != 0) && (myDep == dep)) || (dep == 0))){
        cities->name[index] = (char*) malloc(32*sizeof(char));
        strncpy(cities->name[index], myName, 32);
        cities->pop[index] = myPop;
        cities->lon[index] = myLon;
        cities->lat[index] = myLat;
        cities->dep[index] = myDep;
        
        index++;
      }
      
    }
    free(myName);
    fclose(inputFile);
  }
  
//--------------------------------------------------------------------
//--- WRITING cities with population greater than or equal to 'popMin'
//--------------------------------------------------------------------

  printf("== Writing cities with population >= %i in 'resuCities.dat' ==\n", popMin);

  FILE* outputFile = NULL;
  outputFile = fopen("resuCities.dat", "w");
  if(outputFile != NULL){
    for(int i=0; i<cities->number; i++){
      fprintf(outputFile, "%i %f %f %i\n", cities->pop[i], cities->lon[i], cities->lat[i], cities->dep[i]);
    }
    fclose(outputFile);
  }

  return cities;
}
