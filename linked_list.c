// Copyright 2021 Alexandra Moroiu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"

linked_list_t*
ll_create(unsigned int data_size) {
	linked_list_t *list = malloc(sizeof(linked_list_t));
	DIE(list == NULL, "malloc failed");

	list->head = NULL;
	list->size = 0;
	list->data_size = data_size;

	return list;
}

void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data) {
	ll_node_t *new_node;
	unsigned int count = 0;

	new_node = malloc(sizeof(ll_node_t));
	DIE(new_node == NULL, "malloc failed");

	new_node->data = malloc(list->data_size);
	DIE(new_node->data == NULL, "malloc failed");

	memcpy(new_node->data, new_data, list->data_size);

	if (list->size == 0 || n == 0) {
		// the list is empty or the new node is added at the beginning
		new_node->next = list->head;
		list->head = new_node;
	} else {
		if (list->size < n) {
			n = list->size;
		}

		ll_node_t *current = list->head;

		while(count < n - 1) {
			count++;
			current = current->next;
		}

		new_node->next = current->next;
		current->next = new_node;
	}
	list->size++;
}

ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
	ll_node_t *ptr;
	unsigned int count = 0;

	if(n == 0) {
		// the removed node was the first in the list
		ptr = list->head;
		list->head = ptr->next;
	} else {
		if(list->size - 1 < n) {
			n = list->size - 1;
		}

		ll_node_t *current = list->head;

		while(count < n - 1) {
			count++;
			current = current->next;
		}

		ptr = current->next;
		current->next = current->next->next;
	}

	list->size--;

	return ptr;
}

void
ll_free(linked_list_t** pp_list)
{
	ll_node_t *tmp;

	while((*pp_list)->head != NULL) {
		tmp = (*pp_list)->head;
		(*pp_list)->head = (*pp_list)->head->next;

		free(tmp->data);
		tmp->data = NULL;

		free(tmp);
		tmp = NULL;
	}

	free(*pp_list);
	*pp_list = NULL;
}
