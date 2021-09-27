// Copyright 2021 Alexandra Moroiu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashring.h"

#define SERVER_MAX 100000
#define HMAX 200

unsigned int
hr_get_pos(doubly_linked_list_t* list, unsigned int id) {
	dll_node_t *current = list->head;
	unsigned int server_id = ((server_memory *)(current->data))->id;
	unsigned int count = 0;

	do {
		// found the closest server with a bigger id
		if (hash_function_servers(&server_id) >= id) {
			break;
		}
		current = current->next;
		server_id = ((server_memory *)(current->data))->id;
		count++;
	} while (current != list->head);

	return count;
}

void
hr_add_node(doubly_linked_list_t *list, unsigned int id) {
	server_memory *data;

	if(id < SERVER_MAX) {
		// the server is not a replica
		data = init_server_memory();
	} else {
		data = malloc(sizeof(server_memory));
		DIE(data == NULL, "malloc failed");

		// find the original server's position
		unsigned int original_id = id % SERVER_MAX;
		unsigned int original_hash = hash_function_servers(&original_id);
		unsigned int pos = hr_get_pos(list, original_hash);

		dll_node_t *original = dll_get_nth_node(list, pos);
		data->ht = ((server_memory *)original->data)->ht;
	}

	data->id = id;

	if(list->size == 0) {
		// the hashring is empty
		dll_add_nth_node(list, 0, data);
	} else {
		unsigned int pos = hr_get_pos(list, hash_function_servers(&id));
		dll_add_nth_node(list, pos, data);
	}

	free(data);
}

void reassign(dll_node_t *source, dll_node_t *destination) {
	server_memory *src_server = source->data;
	server_memory *dest_server = destination->data;
	server_memory *prev_server = destination->prev->data;

	unsigned int prev_hash = hash_function_servers(&prev_server->id);
	unsigned int dest_hash = hash_function_servers(&dest_server->id);

	// the servers are replicas of the same server
	if (src_server->id % SERVER_MAX == dest_server->id % SERVER_MAX) {
		return;
	}

	// parse the source's hashtable
	for (unsigned int i = 0; i < HMAX; i++) {
		ll_node_t *current = src_server->ht->buckets[i]->head;
		while (current != NULL) {
			info *data = current->data;
			unsigned int key_hash = hash_function_key(data->key);
			current = current->next;

			// the pair is not assigned correctly
			if ((prev_hash < key_hash && key_hash < dest_hash) ||
				(prev_hash > dest_hash &&
				(prev_hash < key_hash || key_hash < dest_hash))) {
				// store key-value pair on the new server
				server_store(dest_server, data->key, data->value);

				// remove key-value pair from the old server
				server_remove(src_server, data->key);
			}
		}
	}
}

dll_node_t*
hr_remove_node(doubly_linked_list_t* list, unsigned int id) {
	unsigned int pos = hr_get_pos(list, hash_function_servers(&id));
	dll_node_t *ptr = dll_remove_nth_node(list, pos);

	return ptr;
}
