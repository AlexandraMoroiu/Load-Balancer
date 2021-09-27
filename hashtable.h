/* Copyright 2021 Alexandra Moroiu */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "linked_list.h"

typedef struct info {
	void *key;
	void *value;
} info;

typedef struct hashtable_t hashtable_t;
struct hashtable_t {
	linked_list_t **buckets;
	unsigned int size;
	unsigned int hmax;
	unsigned int (*hash_function)(void*);
	int (*compare_function)(void*, void*);
};

/**
 * Comparing functions for the keys
 */
int compare_function_ints(void *a, void *b);

int compare_function_strings(void *a, void *b);

/**
 * Hashing functions for the keys
 */
unsigned int hash_function_servers(void *a);

unsigned int hash_function_key(void *a);

/**
 * ht_create() - Allocates memory for the hashtable
 * @arg1: Maximum number of entries
 * @arg2: Hashing function
 * @arg3: Comparing function
 *
 * Return: Newly created hashtable
 */
hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*));

/**
 * find_node() - Searches a node in the hashtable based on its key
 * @arg1: Hashtable which performs the task
 * @arg2: Node's key
 *
 * Returns: Node or NULL if it does not exist
 */
ll_node_t *find_node(hashtable_t *ht, void *key);

/**
 * ht_put() - Adds a new entry in the hashtable or updates it
 * @arg1: Hashtable which performs the task
 * @arg2: Key that needs to be added or updated
 * @arg3: Key data size
 * @arg4: Value associated to the key
 * @arg5: Value data size
 */
void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size);

/**
 * ht_get() - Retrieves the value associated to a specific key
 * @arg1: Hashtable which performs the task
 * @arg2: Key that we want to look up in the hashtable
 *
 * Return: Searched value
 */
void *
ht_get(hashtable_t *ht, void *key);

/**
 * ht_remove_entry() - Removes an entry from the hashtable
 * @arg1: Hashtable which performs the task
 * @arg2: Key that we want to remove from the hashtable alongside its value
 */
void
ht_remove_entry(hashtable_t *ht, void *key);

/**
 * ht_free() - Frees the memory occupied by the entire hashtable
 * @arg1: Hashtable which performs the task
 */
void
ht_free(hashtable_t *ht);

#endif  /* HASHTABLE_H_ */
