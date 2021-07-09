#include "../includes/sgr.h"

struct sgr_struct{
    char *reviews_filepath;     // Indicates reviews file (.csv) path (used in query 9).
    GHashTable* businesses;     // key=bus_id,   value=name,city,state,stars,#reviews
    GTree* bus_names;           // key=letter,   value=list_of_bus_id
    GTree* categories;          // key=category, value=list_of_bus_id
    GTree* cities;              // key=city,     value=list_of_bus_id
    GHashTable* users;          // key=usr_id,   value=usr_name,list_of_reviews_id
    gboolean dictionary_loaded; // Is the dictionary loaded into memory?
    GHashTable *dictionary;     // Dictionary structure => key=word, value=list_of_reviews_id
};

/*
 * Read & write SGR structures functions defined bellow.
 */

/* Used to lookup values in balanced binary tree. */
gint compare_int(gconstpointer p1, gconstpointer p2) {
    int i1 = GPOINTER_TO_INT(p1);
    int i2 = GPOINTER_TO_INT(p2);
    return i1 == i2 ? 0 : i1 > i2 ? 1 : -1;
}

void free_str(gpointer p1, gpointer p2){
    g_free(p1);
}

gboolean free_list(gpointer p1, gpointer p2, gpointer p3){
    g_list_free_full((GList*)p2, g_free);
    return FALSE; // traverse untill the end of the tree
}

void free_hash_table(gpointer key, gpointer value, gpointer user_data){
    g_list_free_full((GList*) value, g_free);
}

/*Free all of sgr */
void free_sgr(SGR sgr){
    if(!sgr) return; // sgr no longer exists

    g_free(sgr->reviews_filepath);

    g_hash_table_destroy(sgr->businesses);

    g_tree_foreach(sgr->bus_names, free_list, NULL);
    g_tree_destroy(sgr->bus_names);

    g_tree_foreach(sgr->cities, free_list, NULL);
    g_tree_destroy(sgr->cities);

    g_tree_foreach(sgr->categories, free_list, NULL);
    g_tree_destroy(sgr->categories);

    g_hash_table_destroy(sgr->users);

    if(sgr->dictionary_loaded){
        g_hash_table_foreach(sgr->dictionary, free_hash_table, NULL);
        g_hash_table_destroy(sgr->dictionary);
    }

    free(sgr);
}

SGR init_sgr(char* reviews){
    SGR sgr = malloc(sizeof(struct sgr_struct));

    // Save reviews filepath (used in query 9).
    sgr->reviews_filepath = g_strdup(reviews);

    // Set dictionary_loaded to FALSE - not loaded
    sgr->dictionary_loaded  = FALSE;

    // Initiate glib structures for business database
    sgr->businesses = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_bus_ht_value);
    sgr->bus_names = g_tree_new(compare_int);
    sgr->cities = g_tree_new_full((GCompareDataFunc)g_strcmp0, NULL, g_free, NULL);
    sgr->categories = g_tree_new_full((GCompareDataFunc)g_strcmp0, NULL, g_free, NULL);
    
    // Initiate glib structures for users database 
    sgr->users = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_users_ht_value);
  
  return sgr;
}

/* Compare two business id's ratings. */
gint compareBusinessRating(gconstpointer p1, gconstpointer p2, gpointer data){
    /* Get business rating. */
    float r1 = get_stars_business(g_hash_table_lookup((GHashTable*) data, (char*) p1));
    float r2 = get_stars_business(g_hash_table_lookup((GHashTable*) data, (char*) p2));
    return r1 == r2 ? 0 : r1 > r2 ? 1 : -1;
}

typedef struct sorting_tree_data{
    GHashTable *businesses;
    GList *values;    // list of lists of values.
    GList *keys;     // list of keys of the respective values 
}SORT_DATA;

SORT_DATA *init_sorting_tree_data(GHashTable *bus){
    SORT_DATA *data = malloc(sizeof(struct sorting_tree_data));
    data->businesses = bus;
    data->values = NULL;
    data->keys = NULL;
    return data;
}

gboolean sortListsOfTree(gpointer key, gpointer value, gpointer data_arg){
    /* Parse arguments. */
    SORT_DATA *data = data_arg;
    
    /* Sort list of id's by rating. */
    GList *sorted_list = g_list_sort_with_data(value, compareBusinessRating, data->businesses);

    /* Save this sorted list of values & respective key. */
    data->values = g_list_prepend(data->values, sorted_list);
    data->keys = g_list_prepend(data->keys, g_strdup(key));

    return FALSE;
}

void sortTree(GTree *tree, GHashTable *businesses){
    /* Allocate space and set data for sorting the tree. */
    SORT_DATA *data = init_sorting_tree_data(businesses);

    /* Traverse the tree and save sorted lists into SORT_DATA structure. */
    g_tree_foreach(tree, sortListsOfTree, data);

    /* Update cities tree with the new sorted values. */
    GList *list_of_keys   = data->keys;
    GList *list_of_values = data->values;
    while(list_of_keys != NULL){
        /* Replace key-value in tree of cities. */
        g_tree_replace(tree, g_strdup(list_of_keys->data), list_of_values->data);

        /* Update list pointers. */
        list_of_keys   = g_list_next(list_of_keys);
        list_of_values = g_list_next(list_of_values);
    }

    /* Free data structure. */
    g_list_free_full(data->keys, g_free);
    g_list_free(data->values);
    free(data);
}

/* Querie 1 - Load all data structures from the given files. */
SGR load_sgr(char* users, char* businesses, char* reviews) {
    /*
        *We are going to create new structures from scratch.
        * Previous allocated structures must be freed before calling load_sgr.
    */

    SGR sgr = init_sgr(reviews); // new sgr structure

    // First load businesses table - we needn't the file again.
    if(load_business_file(fopen(businesses, "r"),
        sgr->businesses,
        sgr->bus_names,
        sgr->cities,
        sgr->categories)){
        free_sgr(sgr);
        return NULL;
    }

    printf("\e[1;1H\e[2J");
    printf("[#####] 25%% -> business file loaded\n");

    
    // Then, process the users file - we needn't the file again.
    if(load_users_file(fopen(users, "r"), sgr->users)){
        free_sgr(sgr);
        return NULL;
    }
    
    printf("\e[1;1H\e[2J");
    printf("[###########] 55%% -> users file loaded\n");

    // Load the reviews file.
    if(load_reviews_file(fopen(reviews, "r"), sgr->users, sgr->businesses)){
        free_sgr(sgr);
        return NULL;
    }

    printf("\e[1;1H\e[2J");
    printf("[################] 80%% -> reviews file loaded\n");
    
    /* Sort lists in city tree. */
    printf("[###################] 95%% -> Sorting Results\n");
    sortTree(sgr->cities, sgr->businesses);

    printf("\e[1;1H\e[2J");
    printf("[####################] 100%% -> all files loaded\n");

    return sgr;
}

/* Init dictionary. */
GHashTable *initDictionary(SGR sgr){
    sgr->dictionary = g_hash_table_new(g_direct_hash, g_direct_equal);
    return sgr->dictionary;
}

/* Getters */
GTree *getBusNamesTree(SGR sgr){return sgr->bus_names;}
GTree *getCityTree(SGR sgr){return sgr->cities;}
GTree *getCategoryTree(SGR sgr){return sgr->categories;}
GHashTable* get_users_hash_table(SGR sgr) { return sgr->users; }
GHashTable* get_businesses_hash_table(SGR sgr) { return sgr->businesses; }
GHashTable *getDictionary(SGR sgr){ return sgr->dictionary;}
gboolean isDictionaryLoadedMem(SGR sgr){return sgr->dictionary_loaded;}
void setDictionaryLoadedMem(SGR sgr, gboolean s){sgr->dictionary_loaded = s;}
char *getRevFilepath(SGR sgr){return sgr->reviews_filepath;}
