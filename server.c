// Copyright 2021 Alexandra Moroiu
#include <stdlib.h>
#include <string.h>

#include "server.h"

#define HMAX 200

server_memory* init_server_memory() {
	server_memory *server = malloc(sizeof(server_memory));
	DIE(server == NULL, "malloc failed");
	server->ht = ht_create(HMAX, hash_function_key,
				compare_function_strings);
	return server;
}

void server_store(server_memory* server, char* key, char* value) {
	ht_put(server->ht, key, strlen(key) + 1, value, strlen(value) + 1);
}

void server_remove(server_memory* server, char* key) {
	ht_remove_entry(server->ht, key);
}

char* server_retrieve(server_memory* server, char* key) {
	return (char *)ht_get(server->ht, key);
}

void free_server_memory(server_memory* server) {
	ht_free(server->ht);
}
