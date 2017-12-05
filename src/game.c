#include <SDL.h>
#include <stdio.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_draw.h"
#include "hashmap.h"

void help()
{
	printf("To delete the hashmap, type '!delete'\n");
	printf("To quit the terminal, type '!quit'\n");
	printf("To display all available keys, type '!keys'\n");
	printf("To switch to key-value entry, type '!add'\n");
	printf("To switch to getting values from keys, type '!get'\n");
	printf("To display these tips again, type '!help'\n");
}

void free_char_array(char *char_array)
{
	if (!char_array) return;
	free(char_array);
}

char *string_new(Uint32 max)
{
	char *rtn;
	if (max == 0)
		return NULL;
	rtn = (char *)malloc(sizeof(char)*max);
	if (!rtn)
		return NULL;
	memset(rtn, 0, sizeof(char)*max);
	return rtn;
}

int main(int argc, char * argv[])
{
	char *input;
	char *value;
	char *received;
	HashMap *hashmap;

	bool get = false;

	hashmap = hashmap_new(2, 4);
	help();

	while (true)
	{
		if (get == false)
		{
			input = NULL; value = NULL;
			input = string_new(256);
			printf("Enter a key: ");
			scanf("%s", input);
			if (strcmp(input, "!delete") == 0)
			{
				if (hashmap == NULL) continue;
				hashmap_delete(hashmap, free_char_array);
				hashmap = NULL;
				printf("hashmap deleted...\n");
				if (input != NULL)
					free(input);
				continue;
			}
			if (strcmp(input, "!quit") == 0)
			{
				if (input != NULL)
					free(input);
				break;
			}
			if (strcmp(input, "!help") == 0)
			{
				help();
				if (input != NULL)
					free(input);
				continue;
			}
			if (strcmp(input, "!keys") == 0)
			{
				hashmap_print_keys(hashmap);
				if (input != NULL)
					free(input);
				continue;
			}
			if (strcmp(input, "!get") == 0)
			{
				get = true;
				if (!input != NULL)
					free(input);
				continue;
			}
			value = string_new(256);
			printf("Enter a value: ");
			scanf("%s", value);
			printf("%s %s is your key-value pair!\n", input, value);
			hashmap = hashmap_insert(input, value, hashmap);
			printf("Successfully inserted!\n");
		}
		else
		{
			printf("Enter a key to print out its value: ");
			input = NULL; received = NULL;
			input = string_new(256);
			if (!input)
				break;
			scanf("%s", input);
			if (strcmp(input, "!add") == 0)
			{
				get = false;
				if (input != NULL)
					free(input);
				continue;
			}
			if (strcmp(input, "!delete") == 0)
			{
				if (hashmap == NULL) continue;
				hashmap_delete(hashmap, free_char_array);
				hashmap = NULL;
				if (input != NULL)
					free(input);
				continue;
			}
			if (strcmp(input, "!quit") == 0)
			{
				if (input != NULL)
					free(input);
				break;
			}
			if (strcmp(input, "!help") == 0)
			{
				help();
				if (input != NULL)
					free(input);
				continue;
			}
			if (strcmp(input, "!keys") == 0)
			{
				hashmap_print_keys(hashmap);
				if (input != NULL)
					free(input);
				continue;
			}
			received = (char *)hashmap_get_value(input, hashmap);
			if (input != NULL)
				free(input);
			if (!received)
			{
				printf("This key is not in the hashmap. Try again.\n");
				continue;
			}
			printf("Value: %s\n", received);
		}
	}
}
/*eol@eof*/
