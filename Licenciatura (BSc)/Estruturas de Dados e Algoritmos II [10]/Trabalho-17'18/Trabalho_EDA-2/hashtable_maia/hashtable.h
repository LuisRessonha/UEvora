//
// Created by Daniel on 10/06/2018.
//

#ifndef TRABALHOV6_HASHTABLE_H
#define TRABALHOV6_HASHTABLE_H

struct seguidores{
    char nick[6];
    int ult_mens;
};

struct users{
    char nick[6];
    char nome[26];
    int mensagem;
    int seguido_por;
    short n_seguidores;
    struct seguidores seguidor[100];
};

struct data
{
    long file_ptr;
    char nick[6];
};

struct hashtable_item
{

    int flag;
    /*
     * flag = 0 : data not present
     * flag = 1 : some data already present
     * flag = 2 : data was present,but deleted
    */
    struct data item;

};

const int max;
int size;
int prime;

struct hashtable_item *array;

int hashcode1(unsigned long long key);
int hashcode2(unsigned long long key);

unsigned long long ht_calc_hash (char* key);

int find(unsigned long long key, char nick[6]);

#endif //TRABALHOV6_HASHTABLE_H
