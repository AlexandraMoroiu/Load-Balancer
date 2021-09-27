// Copyright 2021 Alexandra Moroiu

#include <stdlib.h>
#include <string.h>

#include "load_balancer.h"

#define SERVER_MAX 100000
#define HMAX 200

struct load_balancer {
	doubly_linked_list_t *hashring;
};

load_balancer* init_load_balancer() {
	load_balancer *main_load = malloc(sizeof(load_balancer));
	DIE(main_load == NULL, "malloc failed");

	main_load->hashring = dll_create(sizeof(server_memory));
	return main_load;
}

void
loader_store(load_balancer* main, char* key, char* value, int* server_id) {
	unsigned int key_hash = hash_function_key(key);

	// search the server to store the key-value pair on
	unsigned int pos = hr_get_pos(main->hashring, key_hash);
	dll_node_t *current = dll_get_nth_node(main->hashring, pos);
	*server_id = ((server_memory *)(current->data))->id;

	// the server is a replica
	if (*server_id >= SERVER_MAX) {
		*server_id = *server_id % SERVER_MAX;
	}

	server_store(current->data, key, value);
}

char* loader_retrieve(load_balancer* main, char* key, int* server_id) {
	unsigned int key_hash = hash_function_key(key);

	unsigned int pos = hr_get_pos(main->hashring, key_hash);
	dll_node_t *current = dll_get_nth_node(main->hashring, pos);
	*server_id = ((server_memory *)(current->data))->id;

	// the server is a replica
	if (*server_id >= SERVER_MAX) {
		*server_id = *server_id % SERVER_MAX;
	}

	return server_retrieve(current->data, key);
}

void loader_add_server(load_balancer* main, int server_id) {
	for(int i = 0; i < 3; i++) {
		// i > 0 => server replicas
		unsigned int id = i * SERVER_MAX + server_id;
		hr_add_node(main->hashring, id);

		unsigned int new_hash = hash_function_servers(&id);
		unsigned int pos = hr_get_pos(main->hashring, new_hash);
		dll_node_t *new = dll_get_nth_node(main->hashring, pos);

		// check items of the next server
		reassign(new->next, new);
	}
}

void loader_remove_server(load_balancer* main, int server_id) {
	for(int i = 2; i >= 0; i--) {
		unsigned int id = i * SERVER_MAX + server_id;

		dll_node_t *ptr = hr_remove_node(main->hashring, id);
		// relocate items belonging to the removed server
		reassign(ptr, ptr->next);

		// the server is not a replica
		if(i == 0) {
			free_server_memory(ptr->data);
		}

		free(ptr->data);
		free(ptr);
	}
}

void free_load_balancer(load_balancer* main) {
	dll_node_t *current = main->hashring->head;
	do {
		// the server is not a replica
		if (((server_memory *)(current->data))->id < SERVER_MAX) {
			free_server_memory(current->data);
		}
		current = current->next;
	} while (current != main->hashring->head);

	dll_free(&(main->hashring));
	free(main);
}
