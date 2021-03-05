#include <stdio.h>

#include "trie.h"

char *words[] = {
  "acabais",
  "acocora",
  "amuareis",
  "aramada",
  "arpoavam",
  "azoto",
  "barceiro",
  "barco",
  "cabulais",
  "coragem",
  "corasse",
  "cuja",
  "embarace",
  "ensaquem",
  "enxuguem",
  "festeiro",
  "laringes",
  "mar",
  "propugno",
  "remem",
  "salteada",
  "repudiei",
  "reactive",
  "festa",
  "leque",
  "festarola",
  "achar",
  "enteados",
  "marujar",
  "vinte",
  NULL
};

char *prefixos[] = {
  "a", 
  "ac", 
  "mar", 
  "por", 
  "vinte",
  NULL
};

/*
   Fazer um programa que:

   1. insere todas as palavras em words[] numa trie

   2. confirma que trie_find() as encontra todas

   3. diz quantas palavras existem na trie

   4. mostra as palavras na trie com prefixos "a" "ac" "mar" "por" "vinte"

   5. remove 5 palavras da trie

   6. mostra todas as palavras na trie, antes e depois de remover palavras
*/

int main(void)
{
  // Criar a trie
  struct trie *t;
  t = trie_new();


  // 1. Inserir todas as palavras em word[] numa trie
  int i = 0;

  while (words[i] != NULL)
  {
    trie_insert(t, words[i]);
    i++;
  }


  // 2. Procurar todas as words
  i = 0;
  printf("+ a encontrar palavras...\n");
  while (words[i] != NULL)
  {
    if (!trie_find(t, words[i]))
      printf("ERRO: palavra %s não encontrada\n", words[i]);

    i++;
  }

  // Verificar se não estão na trie
  if (!trie_find(t, "vin"))
    printf("+ %s não encontrado, ainda bem!\n", "vin");

  if (!trie_find(t, "cujado"))
    printf("+ %s não encontrado, ainda bem!\n", "cujado");



  // 3. quantas palavras estão na trie?
  printf("+ estão %d palavras na trie.\n", trie_count(t));


  // 4. print de palavras dos prefixos

  for (int i = 0; prefixos[i] != NULL; i++)
  {
    trie_print_completions(t, prefixos[i]);
  }


  trie_destroy(t);



  return 0;
}
