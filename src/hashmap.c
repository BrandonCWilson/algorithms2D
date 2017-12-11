#include "hashmap.h"
#include <stdio.h>
#include "gf2d_vector.h"
#include "simple_logger.h"

int hash(char *key)
{
	int i = 0;
	int rtn = 0;
	while (key[i] != '\0')
	{
		if (rtn != 0)
			rtn *= key[i];
		else
			rtn += key[i];
		i++;
	}
	// don't return a negative array index
	if (rtn < 0)
		rtn *= -1;
	return rtn;
}

int *spacehash(Vector2D position, double max_x)
{
	int *rtn;
	int i;
	double x, y;
	int dim = 3;

	rtn = (int *)malloc(sizeof(int) * 9);
	if (!rtn)
	{
		slog("Unable to malloc the return array for the spacehash!");
		return NULL;
	}

	x = position.x - dim; y = position.y - dim;
	
	// create an array of the 9 different sections that a shape might overlap with based on its center position
	// this algorithm assumes that the shape's width or height is absolutely never larger than 20 for the sake of simplicity
	for (i = 0; i < 9; i++)
	{
		if (x >= 0 && y >= 0)
		{
			rtn[i] = (int)(y / dim) * (int)max_x + (int)(x / dim);
		}
		else
			rtn[i] = -1;
		y += dim;
		if (i % 3 == 2)
		{
			x += dim;
			y = position.y - dim;
		}
	}

	return rtn;
}

SpaceHash *spacehash_new(Uint32 max)
{
	SpaceHash *rtn;
	int i;
	if (max == 0)
	{
		slog("Cannot allocate a space hash of 0 memory");
		return NULL;
	}
	rtn = (SpaceHash *)malloc(sizeof(SpaceHash));
	if (!rtn)
	{
		printf("Unable to malloc a new hashmap!\n");
		return NULL;
	}
	memset(rtn, 0, sizeof(SpaceHash));
	rtn->entries = (List **)malloc(sizeof(List *)*max);
	if (!rtn->entries)
	{
		free(rtn);
		printf("Unable a malloc the entries for a new hashmap of size %ui!\n", max);
		return NULL;
	}
	memset(rtn->entries, 0, sizeof(List *)*max);
	rtn->maxentries = max;

	for (i = 0; i < max; i++)
	{
		rtn->entries[i] = gf2d_list_new();
		if (rtn->entries[i] == NULL)
		{
			slog("Failed to properly allocate lists for the space hash at %i in the array", i);
		}
	}
	//slog("I made a spacehash!");
	return rtn;
}

int *spacehash_insert(Vector2D position, void *data, SpaceHash *hashmap, double max_x)
{
	//SpaceHash *target = NULL;
	int addr, i;
	int *keyhash, enthash;
	// we should never need to expand our hashmap since we're using a list based solution
	keyhash = spacehash(position, max_x);
	if (!keyhash)
	{
		slog("Received NULL from the keyhash! Cannot insert your data!");
		return NULL;
	}
	for (i = 0; i < 9; i++)
	{
		if (keyhash[i] == -1)
			continue;
		addr = keyhash[i] % hashmap->maxentries;
		keyhash[i] = addr;
		hashmap->entries[addr] = gf2d_list_append(hashmap->entries[addr], data);
	}
	return keyhash;
}

void spacehash_delete(SpaceHash *hashmap)
{
	int i;
	//slog("Deleting spacehash");
	if (!hashmap)
		return;
	if (hashmap->entries != NULL)
	{
		for (i = 0; i < hashmap->maxentries; i++)
		{
			if (hashmap->entries[i] != NULL)
			{
				//slog("deleting one of the lists in the hashmap");
				gf2d_list_delete(hashmap->entries[i]);
				//slog("the list is free");
				hashmap->entries[i] = NULL;
			}
		}
		//slog("freeing the hashmap's entries block");
		free(hashmap->entries);
		//slog("hashmap entries block is free");
	}
	memset(hashmap, 0, sizeof(SpaceHash));
	//slog("freeing the hashmap in its entirety");
	free(hashmap);
	//slog("the hashmap is free");
}

HashMap *hashmap_new(Uint32 numchunks, Uint32 chunksize)
{
	HashMap *rtn;
	Uint32 max;
	if ((numchunks == 0)||(chunksize == 0))
	{
		printf("Unable to make a hashmap of 0 capacity!\n");
		return NULL;
	}
	max = numchunks * chunksize;
	rtn = (HashMap *)malloc(sizeof(HashMap));
	if (!rtn)
	{
		printf("Unable to malloc a new hashmap!\n");
		return NULL;
	}
	memset(rtn, 0, sizeof(HashMap));
	rtn->entries = (KeyValue *)malloc(sizeof(KeyValue)*max);
	if (!rtn->entries)
	{
		free(rtn);
		printf("Unable a malloc the entries for a new hashmap of size %ui!\n", max);
		return NULL;
	}
	memset(rtn->entries, 0, sizeof(KeyValue)*max);
	rtn->maxentries = max;
	rtn->chunksize = chunksize;
	rtn->numchunks = numchunks;
	return rtn;
}

void hashmap_delete(HashMap *hashmap, void(*free_value)(void *value))
{
	int i;
	if (!hashmap)
		return;
	if (hashmap->entries != NULL)
	{
		for (i = 0; i < hashmap->maxentries; i++)
		{
			if ((hashmap->entries[i].value != NULL)&&(free_value != NULL))
			{
				free_value(hashmap->entries[i].value);
				hashmap->entries[i].value = NULL;
			}
		}
		free(hashmap->entries);
	}
	memset(hashmap, 0, sizeof(HashMap));
	free(hashmap);
}

HashMap *hashmap_expand(HashMap *hashmap, int(*hash)())
{
	HashMap *new_hashmap = NULL;
	int i;
	if (!hashmap)
	{
		printf("Cannot expand a null hashmap!\n");
		return;
	}
	if (hashmap->maxentries == 0)
		printf("Your maxentries is 0???\n");
	printf("Expanding. Original max: %i\n", hashmap->maxentries);
	printf("Expanding the hashmap to size: %i\n", hashmap->maxentries * 2);
	new_hashmap = hashmap_new(hashmap->numchunks * 2, hashmap->chunksize);
	if (!new_hashmap)
		printf("unable to make a new hashmap!\n");
	printf("Hashmap has a new maxentries of: %i\n", new_hashmap->maxentries);
	for (i = 0; i < hashmap->maxentries; i++)
	{
		if (hashmap->entries[i].key == NULL)
			continue;
		hashmap_insert(hashmap->entries[i].key, hashmap->entries[i].value, new_hashmap, hash);
	}
	hashmap_delete(hashmap, NULL);
	return new_hashmap;
}

HashMap *hashmap_insert(char *key, void *value, HashMap *hashmap, int(*hash)())
{
	HashMap *target = NULL;
	int addr, counter;
	int keyhash, enthash;
	if (hashmap->numentries == hashmap->maxentries)
	{
		printf("Calling expand. Original size: %i\n", hashmap->maxentries);
		target = hashmap_expand(hashmap, hash);
		if (!target)
		{
			// report error
			printf("Error expanding the hashmap, could not insert your key-value pair!\n");
			return NULL;
		}
		printf("Expanded. New size: %i\n", target->maxentries);
	}
	else
		target = hashmap;
	keyhash = hash(key);
	addr = (keyhash % target->numchunks) * target->chunksize;
	counter = 0;
	while (target->entries[addr].key != NULL)
	{
		// if we reach the end of a chunk, expand the hashmap and try again
		if (counter >= target->chunksize)
		{
			target = hashmap_expand(target, hash);
			if (target == NULL)
			{
				// report error
				return NULL;
			}
			addr = hash(key);
			addr = (addr % target->maxentries) * target->chunksize;
			counter = 0;
			continue;
		}
		// if we match a key which is already in the hashmap
		else if (strcmp(key, target->entries[addr].key) == 0)
		{
			target->entries[addr].value == value;
			return target;
		}
		addr += 1;
		counter += 1;
	}
	target->entries[addr].key = key;
	target->entries[addr].value = value;
	target->numentries += 1;
	return target;
}

void hashmap_print_keys(HashMap *hashmap)
{
	int i;
	bool empty = true;
	if (!hashmap)
	{
		printf("No hashmap to print the keys from!\n");
		return;
	}
	for (i = 0; i < hashmap->maxentries; i++)
	{
		if (hashmap->entries[i].key != NULL)
		{
			empty = false;
			printf("key: %s\n", hashmap->entries[i].key);
		}
		//else
		//	printf("%i : EMPTY STORAGE.\n", i);
	}
	if (empty)
	{
		printf("There were no keys to print!\n");
	}
}

void *hashmap_get_value(char *key, HashMap *hashmap)
{
	int addr, counter;
	int keyhash, enthash;
	if (!key || !hashmap)
	{
		printf("Cannot return a value. Hashmap or key are NULL.\n");
		return NULL;
	}
	keyhash = hash(key);
	addr = (keyhash % hashmap->numchunks) * hashmap->chunksize;
	counter = 0;
	while (counter < hashmap->chunksize)
	{
		// if there's a key, compare it and see if it's the same as ours
		if (hashmap->entries[addr].key != NULL)
		{
			// if it's the same as ours, we found it!
			if (strcmp(hashmap->entries[addr].key, key) == 0)
				break;
		}
		addr++;
		counter++;
	}
	return hashmap->entries[addr].value;
}