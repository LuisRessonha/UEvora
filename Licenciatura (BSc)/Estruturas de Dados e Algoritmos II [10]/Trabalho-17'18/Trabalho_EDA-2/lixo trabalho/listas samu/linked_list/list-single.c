#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "list.h"

typedef struct node_ {
	int element;
	struct node_ *next;
} node;

node *node_new()
{
	node *n = malloc(sizeof(node));
	
	n->element = 0;
	n->next = NULL;

	return n;
}

void node_destroy(node *node)
{
	free(node);
}

typedef struct list_ {
	node *head;
	int size;
} list;

list *list_new()
{
	list *l = malloc(sizeof(list));

	l->head = node_new();
	l->size = 0;

	return l;
}

bool list_empty(list *list)
{
	return list->size == 0;
}

bool list_insert(list *list, int value)
{
	node *n = node_new();
	n->element = value;

	n->next = list->head->next;
	list->head->next = n;

	list->size = list->size + 1;

	return true;	// Quando é que list_insert dá false?
					// R: quando nao tem espaço na memória
}

bool list_remove(list *list, int value)
{
	node *node_c = list->head;

	while (node_c->next != NULL && node_c->next->element != value)
		node_c = node_c->next;

	if (node_c->next->element == value)
	{
		node *remove = node_c->next;
		node_c->next = remove->next;

		list->size = list->size - 1;
		node_destroy(remove);
		return true;
	}
	else
		return false;
}

void list_print(list *list)
{
	node *node_c = list->head;

	printf("[ ");
	while (node_c->next != NULL)
	{
		printf("%d ", node_c->next->element);
		node_c = node_c->next;
	}
	printf("]");
}

int list_find(list *list, int value)
{
	node *node_c = list->head->next;
	int pos = 0;

	while (node_c != NULL && node_c->element != value)
	{
		node_c = node_c->next;
		pos++;
	}

	if (node_c == NULL)
		return -1;
	else
		return pos;
}

void list_destroy(list *list)
{
	node *node_remove = list->head;
	node *node_next;

	while (node_remove->next != NULL)
	{
		node_next = node_remove->next;
		node_destroy(node_remove);
		node_remove = node_next;
	}
	node_destroy(node_remove);

	free(list);
}

int list_length(list *list)
{
	return list->size;
}

int list_nth(list *list, int n)
{
	node *node_c = list->head;
	int pos = -1;

	while(pos != n)
	{
		pos++;
		node_c = node_c->next;
	}

	return node_c->element;
}