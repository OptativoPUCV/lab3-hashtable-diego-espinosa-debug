#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  long posicion = hash(key,map->capacity);
  
  while(map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL ){
    posicion = (posicion + 1) % map->capacity;
  }

  map->buckets[posicion] = createPair(key,value);
  map->size++;
  map->current = posicion;
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair** auxBuckets = (Pair**)calloc(map->capacity, sizeof(Pair*));
  int cont = 0;
  for(int i = 0; i < map->capacity; i++)
  {
    if(map->buckets[i] != NULL || map->buckets[i]->key != NULL)
    {
      auxBuckets[cont] = map->buckets[i];
    }

    cont++;
  }
  
  
  map->buckets = (Pair**)calloc(map->capacity * 2, sizeof(Pair*));
  map->capacity = map->capacity * 2;
  map->size = 0;

  for(int i = 0; i < map->capacity; i++)
  {
    insertMap(map,auxBuckets[i]->key,auxBuckets[i]->value);
  }

}


HashMap * createMap(long capacity) {
  HashMap * mapa = (HashMap*) malloc(sizeof(HashMap));

  mapa->buckets = (Pair**)calloc(capacity, sizeof(Pair*));
  mapa->size = 0;
  mapa->capacity = capacity;
  mapa->current = -1;

  return mapa;
}

void eraseMap(HashMap * map,  char * key) {    
  long posicion = hash(key,map->capacity);

  while(strcmp(map->buckets[posicion]->key,key) != 0)
  {
    posicion = (posicion + 1) % map->capacity;
    if(map->buckets[posicion] == NULL)
    {
      return;
    }
  }

  map->buckets[posicion]->key = NULL;
  map->size--;

}

Pair * searchMap(HashMap * map,  char * key) {   
  long posicion = hash(key,map->capacity);
  
  while(strcmp(map->buckets[posicion]->key,key) != 0)  
  {
    posicion = (posicion + 1) % map->capacity;
    if(map->buckets[posicion] == NULL)
    {
      return NULL;
    }
  }

  map->current = posicion;
  
  return map->buckets[posicion];
}

Pair * firstMap(HashMap * map) {
  long posicion = 0;

  while(map->buckets[posicion] == NULL || map->buckets[posicion]->key == NULL){
    posicion = (posicion + 1) % map->capacity;
  }

  map->current = posicion;
  
  return map->buckets[posicion];
}

Pair * nextMap(HashMap * map) {
  long posicion = map->current + 1;

  while(map->buckets[posicion] == NULL || map->buckets[posicion]->key == NULL){
    posicion = posicion + 1;
    if(posicion >= map->capacity)
    {
      return NULL;
    }
  }

  map->current = posicion;
  
  return map->buckets[posicion];
}
