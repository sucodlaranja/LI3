#ifndef LOADFILES_H
#define LOADFILES_H

#include "glibWarningAvoid.h"
#include <stdio.h>

#define BLOCK_SIZE 1048576 * 0.5 // 512 KBytes

// Macros for business.csv file.
#define ID_SIZE 32
#define NAME_SIZE 128
#define CITY_SIZE 64
#define STATE_SIZE 32
#define MAX_CATEGORIES 64
#define CATEGORY_SIZE 64
#define FLOAT_SIZE 16
#define N_REVIEWS_SIZES 16
#define WORD_SIZE 32

typedef struct bus_ht_value BUS_HT_VALUE;
typedef struct users_ht_value USERS_HT_VALUE;


/**
 * @brief                Load business file (.csv).
 * 
 * @param bus_file       Input File pointer.
 * @param business_table HashTable where 
 *                          key   = bus_id
 *                          value = struct_of_business_info
 * @param business_names GTree where
 *                          key   = first_letter_of_bus_name
 *                          value = list_of_business_ids
 * @param cities         GTree where
 *                          key   = city
 *                          value = list_of_business_ids
 * @param categories     GTree where
 *                          key   = category
 *                          value = list_of_business_ids
 * @return int           0 -> OK
 *                       1 -> file_not_loaded.   
 */
int load_business_file( FILE       *bus_file,
                        GHashTable *business_table,
                        GTree      *business_names,
                        GTree      *cities,
                        GTree      *categories);

/* Destroy allocated structures for business.csv. */
void free_bus_ht_value(gpointer p);
gboolean free_list(gpointer p1, gpointer p2, gpointer p3);

/**
 * @brief             Load users file (.csv).
 * 
 * @param usr_file    Input File pointer.
 * @param users_table HashTable where 
 *                      key   = usr_id
 *                      value = struct_of_usr_info
 * @return int        0 -> OK
 *                    1 -> file_not_loaded. 
 */
int load_users_file(FILE *usr_file, GHashTable *users_table);

/**
 * @brief                Load users file (.csv).
 * 
 * @param rev_file       Input File pointer.
 * @param users_table    HashTable where 
 *                          key   = usr_id
 *                          value = struct_of_usr_info
 * @param business_table HashTable where 
 *                          key   = bus_id
 *                          value = struct_of_business_info
 * @return int           0 -> OK
 *                       1 -> file_not_loaded.
 */
int load_reviews_file(FILE* rev_file, GHashTable* users_table, GHashTable* business_table);

/* Destroy allocated structures for users.csv. */
void free_users_table(GHashTable* users_table);
void free_users_ht_value(gpointer p);
GList* get_list_users(USERS_HT_VALUE* users_value);

char* get_name_business(BUS_HT_VALUE* business_table);
char* get_state_business(BUS_HT_VALUE* value);
char *get_city_business(BUS_HT_VALUE* business_table);
float get_stars_business(BUS_HT_VALUE* business_table);
int get_number_reviews_business(BUS_HT_VALUE* business_table);

#endif