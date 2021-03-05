#include "user.h"
#include "trie.h"

typedef struct seguidores Seguidores;

Seguidores *new_seguidores();
void destroy_seguidores(Seguidores *s);

void add_seguidor(Seguidores *seg, char nick[NICK_SIZE]);
void remove_seguidor(Seguidores *seg, char nick[NICK_SIZE]);

void desativar_seguidores(Seguidores *seg, Trie *trie);
int numero_seguidores(Seguidores *seg);

void write_seguidores(Seguidores *seg, FILE *fd);
Seguidores *read_seguidores(Seguidores *seg, FILE *fd);
Seguidores *read_segSize(Seguidores *seg, FILE *fd);
