#ifndef SGR_H
#define SGR_H

#include "LoadFiles.h"


typedef struct sgr_struct *SGR;


/**
 * @brief Free allocated memory from SGR structure.
 * 
 * @param sgr Target structure.
 */
void free_sgr(SGR sgr);


/**
 * @brief 1. Dado o caminho para os 3 ficheiros (Users, Businesses, e Reviews), ler o seu 
 * conteúdo e carregar as estruturas de dados correspondentes. Durante a interação com o
 * utilizador (no menu da aplicação), este poderá ter a opção de introduzir os paths
 * manualmente ou, opcionalmente, assumir um caminho por omissão. Note-se que qualquer
 * nova leitura destes ficheiros deverá de imediato reiniciar e refazer as estruturas de 
 * dados em memória como se de uma reinicialização se tratasse.
 *
 * Call free_sgr() before load_sgr()!!
 * 
 * @param users filepath to users.csv
 * @param businesses filepath to businesses.csv
 * @param reviews filepath to reviews.csv
 * @return SGR Loaded databases or NULL in case of failure.
 */
SGR load_sgr(char *users, char *businesses, char *reviews);

int load_dictionary(SGR sgr);

gint compareBusinessRating(gconstpointer p1, gconstpointer p2, gpointer data);
GHashTable* get_users_hash_table(SGR sgr);
GHashTable* get_businesses_hash_table(SGR sgr);
GTree *getBusNamesTree(SGR sgr);
GTree *getCityTree(SGR sgr);
GTree *getCategoryTree(SGR sgr);
GHashTable *getDictionary(SGR sgr);
gboolean isDictionaryLoadedMem(SGR sgr);
void setDictionaryLoadedMem(SGR sgr, gboolean s);
char *getRevFilepath(SGR sgr);
GHashTable *initDictionary(SGR sgr);
#endif