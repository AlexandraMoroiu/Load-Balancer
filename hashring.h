/* Copyright 2021 Alexandra Moroiu */

#ifndef HASHRING_H_
#define HASHRING_H_

#include "circular_doubly_linked_list.h"
#include "server.h"
#include "utils.h"

/**
 * hr_get_pos() - Searches the closest server with a bigger
 * 				or equal id than @arg2
 * @arg1: Hashring stored as a cdll
 * @arg2: The id/hash used for comparison
 *
 * Return: Wanted server's position in the hashring
 */
unsigned int hr_get_pos(doubly_linked_list_t* list, unsigned int id);

/**
 * hr_add_node() - Adds new server in the hashring
 * @arg1: Hashring stored as a cdll
 * @arg2: New server's id
 */
void
hr_add_node(doubly_linked_list_t* list, unsigned int id);

/**
 * reassign() - Relocates the key-value pairs when a server is added/removed
 * @arg1: The source node
 * @arg2: The destination node
 *
 * The sources's hashtable is parsed and the key-value pairs are relocated
 * accordingly to the destination server or remain on the source server.
 * A key-value pair belongs to destination if it is located
 * between destination->prev and destination in the hashring
 */
void reassign(dll_node_t *source, dll_node_t *destination);

/**
 * hr_remove_node() - Removes a server from the hashring
 * @arg1: Hashring stored as a cdll
 * @arg2: Server's id
 *
 * Return: Pointer to the removed node
 */
dll_node_t*
hr_remove_node(doubly_linked_list_t* list, unsigned int n);

#endif  /* HASHRING_H_ */
