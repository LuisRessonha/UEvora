#include <stdbool.h>

typedef struct list_ list;

list *list_new();
bool list_empty(list *list);
bool list_insert(list *list, int value);
bool list_remove(list *list, int value);
void list_print(list *list);
int list_find(list *list, int value);
void list_destroy(list *list);
int list_length(list *list);
int list_nth(list *list, int n);
