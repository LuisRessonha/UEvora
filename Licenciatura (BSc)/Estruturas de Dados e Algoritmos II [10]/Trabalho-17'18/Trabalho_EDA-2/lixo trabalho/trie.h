/*#include <stdbool.h>
#include "user.h"
#include "list.h"

typedef struct trie Trie;

Trie *trie_new();
void trie_destroy(Trie *trie);

bool trie_empty(Trie *trie);

bool trie_insert(Trie *trie, User *user);
User *trie_find(Trie *trie, char nick[NICK_SIZE]);
lista *trie_find_segue(Trie *trie, char nick[NICK_SIZE]);

void read_message(Trie *trie, lista *lista);

Trie *trie_read();
void trie_write(Trie *trie);
bool trie_apagar(Trie *trie, char nick[NICK_SIZE]);

void info(Trie *trie, char nick[NICK_SIZE]);
*/