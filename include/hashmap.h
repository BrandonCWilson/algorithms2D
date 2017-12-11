#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include "gf2d_list.h"
#include "gf2d_vector.h"

typedef struct KeyValue_s
{
	char *key;
	void *value;
} KeyValue;

typedef struct HashMap_s
{
	KeyValue *entries;
	int maxentries;

	int chunksize;
	int numchunks;

	int numentries;
} HashMap;

typedef struct SpaceHash_s
{
	List **entries;
	int maxentries;
} SpaceHash;

int hash(char *key);

HashMap *hashmap_new(Uint32 numchunks, Uint32 chunksize);

HashMap *hashmap_insert(char *key, void *data, HashMap *hashmap, int(*hash)());

void *hashmap_get_value(char *key, HashMap *hashmap);

void hashmap_print_keys(HashMap *hashmap);

void hashmap_delete(HashMap *hashmap, void(*free_value)(void *value));

int *spacehash(Vector2D position, double max_x);

SpaceHash *spacehash_new(Uint32 max);

int *spacehash_insert(Vector2D position, void *data, SpaceHash *hashmap, double max_x);

/*
void *spacehash_get_value(char *key, SpaceHash *hashmap);

void spacehash_print_keys(SpaceHash *hashmap);
*/
void spacehash_delete(SpaceHash *hashmap);

#endif