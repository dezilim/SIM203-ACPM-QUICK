#ifndef CITIES_READER
#define CITIES_READER
#include <stdbool.h>

typedef struct ListOfCities ListOfCities;

struct ListOfCities
{
  int    number;  // number of cities in the list
  char** name;    // name of the cities
  int*   pop;     // population of the cities
  float* lon;     // longitude
  float* lat;     // latitude
  int* dep;       // departement
};

ListOfCities* citiesReader(int popMin,int dep, bool bigOnly);

#endif
