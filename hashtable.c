// Copyright 2021 Alexandra Moroiu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "utils.h"

int compare_function_ints(void *a, void *b) {
	int int_a = *((int *)a);
	int int_b = *((int *)b);

	if (int_a == int_b) {
		return 0;
	} else if (int_a < int_b) {
		return -1;
	} else {
		return 1;
	}
}

int compare_function_strings(void *a, void *b) {
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

unsigned int
hash_function_servers(void *a) {
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

unsigned int
hash_function_key(void *a) {
	unsigned char *puchar_a = (unsigned char *) a;
	unsigned int hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
}

hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*)) {
	hashtable_t *ht = malloc(sizeof(hashtable_t));
	DIE(ht == NULL, "malloc failed");

	ht->hash_function = hash_function;
	ht->compare_function = compare_function;
	ht->size = 0;
	ht->hmax = hmax;

	ht->buckets = malloc(sizeof(linked_list_t*) * hmax);
	DIE(ht->buckets == NULL, "malloc failed");

	for(unsigned int i = 0; i < hmax; i++) {
		ht->buckets[i] = ll_create(sizeof(info));
	}

	return ht;
}

ll_node_t *find_node(hashtable_t *ht, void *key) {
	unsigned int slot = ht->hash_function(key) % ht->hmax;

	ll_node_t *current = ht->buckets[slot]->head;

	for(unsigned int i = 0; i < ht->buckets[slot]->size; i++) {
		if (ht->compare_function(key, ((info*)current->data)->key) == 0) {
			return current;
		}
		current = current->next;
	}

	// could not find the node
	return NULL;
}

void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size) {
	unsigned int slot = ht->hash_function(key) % ht->hmax;

	ll_node_t *node = find_node(ht, key);

	info *entry = malloc(sizeof(info));
	DIE(entry == NULL, "malloc failed");

	// could not find the node
	if (node == NULL) {
		entry->key = malloc(key_size);
		DIE(entry->key == NULL, "malloc failed");
		memcpy(entry->key, key, key_size);

		entry->value = malloc(value_size);
		DIE(entry->value == NULL, "malloc failed");
		memcpy(entry->value, value, value_size);

		ll_add_nth_node(ht->buckets[slot], 0, entry);
		ht->size++;

	} else {
		// update the value
		free(((info*)node->data)->value);
		((info*)node->data)->value = malloc(value_size);
		DIE(((info*)node->data)->value == NULL, "malloc failed");
		memcpy(((info*)node->data)->value, value, value_size);
	}

	free(entry);
}

void *ht_get(hashtable_t *ht, void *key) {
	unsigned int slot = ht->hash_function(key) % ht->hmax;
	ll_node_t *current = ht->buckets[slot]->head;

	while (current != NULL) {
		if (ht->compare_function(key, ((info*)current->data)->key) == 0) {
			return ((info*)current->data)->value;
		}
		current = current->next;
	}
	return NULL;
}

void
ht_remove_entry(hashtable_t *ht, void *key) {
	unsigned int slot = ht->hash_function(key) % ht->hmax;
	unsigned int count = 0;

	ll_node_t *current;
	current = ht->buckets[slot]->head;
	while (ht->compare_function(key, ((info*)current->data)->key)
			&& count < ht->buckets[slot]->size) {
		count++;
		current = current->next;
	}

	if (current) {
		current = ll_remove_nth_node(ht->buckets[slot], count);
		free(((info*)current->data)->key);
		free(((info*)current->data)->value);
		free(current->data);
		free(current);
		ht->size--;
	}
}

void
ht_free(hashtable_t *ht) {
	for(unsigned int i = 0; i < ht->hmax; i++) {
		ll_node_t *node;
		while (ht->buckets[i]->size > 0) {
			node = ll_remove_nth_node(ht->buckets[i], 0);
			free(((info*)node->data)->key);
			free(((info*)node->data)->value);
			free(node->data);
			free(node);
		}
		free(ht->buckets[i]);
		ht->buckets[i] = NULL;
	}
	free(ht->buckets);
	ht->buckets = NULL;
	free(ht);
	ht = NULL;
}
