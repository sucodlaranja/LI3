#include "../includes/Queries.h"

/**
 * @brief From lists to a TABLE. Used in several queries.
 *        We need to prepend the indexes/titles to each list.
 * 
 * @param lists Array of lists.
 * @param numLists Number of lists.
 * @param sizes Maximum size of each element of each list.
 * @return TABLE 
 */
TABLE listsToTable(GList**lists, int numLists, int list_length){
    /* Safety first. */
    if(lists == NULL || lists[0] == NULL) return NULL;
 
    /* Allocate memory to TABLE. */
    TABLE table_struct = initTable(numLists, list_length);

    /* Get matrix. */
    char ***table = get_tbl(table_struct);

    int col;
    for(col = 0; col < list_length; ++col){
        /* Copy from list to TABLE & increment list pointer */
        int l;
        for(l = 0; l < numLists; ++l){
            table[l][col] = g_strdup(lists[l]->data);
            lists[l] = g_list_next(lists[l]);
        }
    }

    return table_struct;
}

/* Helps to deep_copy glist. */
gpointer copy_list(gconstpointer value, gpointer data){
    return g_strdup(value);
}

TABLE businesses_started_by_letter(SGR sgr, char letter){
    /* Get Business Table where key = bus_id, value = BUS_HT_VALUE */
    GHashTable *bus_table = get_businesses_hash_table(sgr);

    /* Get Business Name Tree where key = letter, value = list_of_ids. */
    GTree *bus_name_tree = getBusNamesTree(sgr);

    /* Get list of id's from business whose name starts with the given letter. */
    GList *list_of_ids_old = g_tree_lookup(bus_name_tree, GINT_TO_POINTER(g_ascii_toupper(letter)));

    /* Make a deep copy of list_of_ids. */
    GList *list_of_ids = g_list_copy_deep(list_of_ids_old, copy_list, NULL);

    /* Get names list. */
    GList *list_of_names = NULL;
    GList *ptr = list_of_ids;
    int col = 1;
    for(col = 1; ptr != NULL; ptr = g_list_next(ptr), col++){
        char *name = get_name_business(g_hash_table_lookup(bus_table, ptr->data));
        list_of_names = g_list_prepend(list_of_names, g_strdup(name));
    }

    /* Indexes/Title of each list. */
    list_of_ids   = g_list_prepend(list_of_ids, g_strdup("business_id"));
    list_of_names = g_list_prepend(list_of_names, g_strdup("name"));

    /* Group lists into array of GList*. */
    GList **lists = malloc(sizeof(GList*) * 2);
    lists[0] = list_of_ids; lists[1] = list_of_names;

    /* Get table. */
    TABLE table_struct = listsToTable(lists, 2, col);

    /* Free lists array. */
    g_list_free_full(list_of_ids, g_free);
    g_list_free_full(list_of_names, g_free);

    free(lists);

    return table_struct;
}

TABLE businesses_info(SGR sgr, char *business_id){
    /* Get Value from business table. Value includes all the info necessary. */
    gpointer value = g_hash_table_lookup(get_businesses_hash_table(sgr), business_id);

    /* Invalid id. */
    if(!value) return NULL;

    /* No need to use listsToTable() due to this query's simplicity. */
    TABLE table_struct = initTable(5, 2);
    char ***table = get_tbl(table_struct);
    
    table[0][0] = g_strdup("name");
    table[0][1] = g_strdup(get_name_business(value));

    table[1][0] = g_strdup("city");
    table[1][1] = g_strdup(get_city_business(value));

    table[2][0] = g_strdup("state");
    table[2][1] = g_strdup(get_state_business(value));

    table[3][0] = g_strdup("stars");
    table[3][1] = malloc(sizeof(char) * FLOAT_SIZE);
    sprintf(table[3][1], "%f", get_stars_business(value));

    table[4][0] = g_strdup("#reviews");
    table[4][1] = malloc(sizeof(char) * N_REVIEWS_SIZES);
    sprintf(table[4][1], "%d", get_number_reviews_business(value));

    return table_struct;
}

TABLE businesses_reviewed(SGR sgr, char* user_id) {
    /* Get business table. */
    GHashTable *business_table = get_businesses_hash_table(sgr);

    /* Find list of businesses that the user reviewed. */
    USERS_HT_VALUE* users_value = g_hash_table_lookup(get_users_hash_table(sgr), user_id);
    GList* list_of_ids_old = get_list_users(users_value);

    /* Deep copy list_of_ids. */
    GList *list_of_ids = g_list_copy_deep(list_of_ids_old, copy_list, NULL);

    /* Create list o business names. */
    GList *list_of_names = NULL;
    int col = 1;
    GList *ptr = g_list_last(list_of_ids);
    for(; ptr != NULL; ptr = g_list_previous(ptr), col++){
        /* Get business info. */
        BUS_HT_VALUE *bus_info = g_hash_table_lookup(business_table, ptr->data);
        
        /* Prepend name to list_of_names. */
        list_of_names = g_list_prepend(list_of_names, g_strdup(get_name_business(bus_info)));
    }

    /* Indexes/Titles of each list. */
    list_of_ids   = g_list_prepend(list_of_ids, g_strdup("business_id"));
    list_of_names = g_list_prepend(list_of_names, g_strdup("name"));

    /* Create array of lists. */
    GList **lists = malloc(sizeof(GList*) * 2);
    lists[0] = list_of_ids;
    lists[1] = list_of_names;

    /* Get TABLE structure. */
    TABLE table_struct = listsToTable(lists, 2, col);

    /* Free allocated space. */
    g_list_free_full(list_of_ids, g_free);
    g_list_free_full(list_of_names, g_free);
    free(lists);

    return table_struct;
}

TABLE businesses_with_stars_and_city(SGR sgr, float min_stars, char* city) {
    /* Get city tree where key = city_name, value = list_of_bus_ids */
    GTree *city_tree = getCityTree(sgr);

    /* Get list of business id's that belong to the city. */
    GList *list_of_ids = g_list_last(g_tree_lookup(city_tree, city));

    /* Get Business Table where key = bus_id, value = BUS_HT_VALUE */
    GHashTable *bus_table = get_businesses_hash_table(sgr);

    /* Get rating of business (lookup bus_table).
     * If rating > min_stars continue.
     * Get business name (lookup bus_table or save previous lookup result).
     * Add valid id&name to the new lists.
     */
    GList *new_list_of_ids = NULL;
    GList *list_of_names = NULL;
    int numBus = 1; 
    // Iterate from last to first (sorting order)
    for(; list_of_ids != NULL; list_of_ids = g_list_previous(list_of_ids)){
        /* Get value of business (name, city, state, rating, n_reviews...) */
        BUS_HT_VALUE *value = g_hash_table_lookup(bus_table, list_of_ids->data);

        /* If rating of business >= min_stars then continue */    
        if(get_stars_business(value) >= min_stars){
            /* Add name of business to list_of_names. */
            list_of_names = g_list_prepend(list_of_names, g_strdup(get_name_business(value)));
            /* Add business id to the new id list. */
            new_list_of_ids = g_list_prepend(new_list_of_ids, g_strdup(list_of_ids->data));
            ++numBus;
        }
        else{
            break; // the list is sorted by business rating
        }
    }

    /* Prepend indexes/titles of each list. */
    new_list_of_ids = g_list_prepend(new_list_of_ids, g_strdup("business_id"));
    list_of_names   = g_list_prepend(list_of_names, g_strdup("name"));

    /* Create array of lists. */
    GList **lists = malloc(sizeof(GList*) * 2);
    lists[0] = new_list_of_ids;
    lists[1] = list_of_names;

    /* Get table. */
    TABLE table_struct = listsToTable(lists, 2, numBus);
    
    /* Free lists. */
    g_list_free_full(new_list_of_ids, g_free);
    g_list_free_full(list_of_names, g_free);
    free(lists);

    return table_struct;
}

typedef struct query_6_data{
    short cities_sorted;
    GList *list_of_cities;
    GList *list_of_ids;
    GList *list_of_names;
    GList *list_of_ratings;
    int top;
    int table_size;
    GHashTable *business;
}DATA_Q6;

DATA_Q6 *init_dataq6(SGR sgr, int top){
    DATA_Q6 *data_struct = malloc(sizeof(struct query_6_data));
    data_struct->top = top;
    data_struct->table_size = 1;
    data_struct->business = get_businesses_hash_table(sgr);
    data_struct->list_of_cities = NULL;
    data_struct->list_of_ids = NULL;
    data_struct->list_of_names = NULL;
    data_struct->list_of_ratings = NULL;
    return data_struct;
}

void free_dataq6(DATA_Q6 *data_struct){
    // We had to allocate space to the ratings list, let's free it
    g_list_free_full(data_struct->list_of_ratings, g_free);
    
    g_list_free_full(data_struct->list_of_cities, g_free);
    g_list_free_full(data_struct->list_of_ids, g_free);
    g_list_free_full(data_struct->list_of_names, g_free);

    free(data_struct);
}

/**
 * @brief Traverse city_tree and solve Q6.
 * 
 * @param key City name.
 * @param value List of businesses id's that belong to the city.
 * @param data table/matrix, table_idx, top, business hash table.
 * @return gboolean FALSE -> read the entire tree.
 */
gboolean cityTreeTraverseQ6(gpointer key, gpointer value, gpointer data){
    /* Get data. */
    DATA_Q6 *data_q6 = data;
   
    /* Get variables from data. */
    GList *new_list_of_cities = data_q6->list_of_cities;
    GList *new_list_of_ids = data_q6->list_of_ids;
    GList *new_list_of_names = data_q6->list_of_names;
    GList *new_list_of_ratings = data_q6->list_of_ratings;
    int top = data_q6->top;
    int table_size = data_q6->table_size;
    GHashTable *businesses = data_q6->business;

    /* Parse attributes. */
    char *city_name = key;

    /* Get last - sorted in ascending order of ratings. */
    GList *original_list_of_ids = g_list_last((GList*) value);

    /* Get top id-name-#* */
    int i;
    for(i = 0; (original_list_of_ids != NULL) && (i < top); ++i, table_size++){
        // Get id of business
        char *id = original_list_of_ids->data;

        // Get business information (name, city, state, stars...)
        BUS_HT_VALUE *bus_info = g_hash_table_lookup(businesses, id);

        // FLOAT to CHAR*
        char *stars = malloc(sizeof(char) * FLOAT_SIZE);
        snprintf(stars, sizeof(char) * FLOAT_SIZE, "%.3f", get_stars_business(bus_info));

        // Save all data into the lists
        new_list_of_cities  = g_list_append(new_list_of_cities, g_strdup(city_name));
        new_list_of_ids     = g_list_append(new_list_of_ids, g_strdup(id));
        new_list_of_names   = g_list_append(new_list_of_names, g_strdup(get_name_business(bus_info)));
        new_list_of_ratings = g_list_append(new_list_of_ratings, stars);
    
        // Update list pointer
        original_list_of_ids = g_list_previous(original_list_of_ids);
    }

    // Update data_struct
    data_q6->table_size = table_size;
    data_q6->list_of_cities  = new_list_of_cities;
    data_q6->list_of_ids     = new_list_of_ids;
    data_q6->list_of_names   = new_list_of_names;
    data_q6->list_of_ratings = new_list_of_ratings;

    return FALSE;
}

TABLE top_businesses_by_city(SGR sgr, int top){
    /* Get cities tree. */
    GTree *cities_tree = getCityTree(sgr);

    // Init data_struct
    DATA_Q6 *data_struct = init_dataq6(sgr, top);
    
    // For each node, get top business and place their info into table_struct.
    g_tree_foreach(cities_tree, cityTreeTraverseQ6, data_struct);

    /* Prepend indexes/titles of each list. */
    data_struct->list_of_cities  = g_list_prepend(data_struct->list_of_cities, g_strdup("city"));
    data_struct->list_of_ids     = g_list_prepend(data_struct->list_of_ids, g_strdup("business_id"));
    data_struct->list_of_names   = g_list_prepend(data_struct->list_of_names, g_strdup("name"));
    data_struct->list_of_ratings = g_list_prepend(data_struct->list_of_ratings, g_strdup("stars"));
    
    /* Create array of lists. */
    GList **lists = malloc(sizeof(GList*) * 4);
    lists[0] = data_struct->list_of_cities;
    lists[1] = data_struct->list_of_ids;
    lists[2] = data_struct->list_of_names;
    lists[3] = data_struct->list_of_ratings;

    /* Get table structure. */
    TABLE table_struct = listsToTable(lists, 4, data_struct->table_size);

    /* Free allocated space. */
    free_dataq6(data_struct);
    free(lists);

    return table_struct;
}

/* Given an list of reviewd businesses checks if hey are from different states  */
int is_international_user(GList* buss_list , GHashTable* businessTable) {
    if (buss_list) {
        BUS_HT_VALUE* temp = g_hash_table_lookup(businessTable, buss_list->data);
        char* previous_state = get_state_business(temp), * novo;

        for (; buss_list; buss_list = buss_list->next) {
            temp = g_hash_table_lookup(businessTable, buss_list->data);
            novo = get_state_business(temp);
            if (strcmp(previous_state, novo) != 0) return 1;
            previous_state = novo;
        }
        return -1;
    }
    return -1;
}

TABLE international_users(SGR sgr) {
    
    if(!sgr) return NULL;
    
    GHashTable* usersTable = get_users_hash_table(sgr);
    GList* international_user = NULL;
    int col = 1;

    GHashTableIter iter;
    gpointer key, value;

    // Generates a list of users that are international.
    g_hash_table_iter_init(&iter, usersTable);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        if (is_international_user(get_list_users(value), get_businesses_hash_table(sgr)) == 1) {
            international_user = g_list_prepend(international_user, key); ++col;
        }
    }
    
    /* Index/Title of list. */
    international_user = g_list_prepend(international_user, "user_id");

    /* Array of only one list. */
    GList **lists = malloc(sizeof(GList*)); lists[0] = international_user;

    /* Get table structure. */
    TABLE table_struct = listsToTable(lists, 1, col);

    /* Free allocated space */
    free(lists);
    g_list_free(international_user);

    return table_struct;
}

TABLE top_businesses_with_category(SGR sgr, int top, char *key){
    /* Get business hash table. */
    GHashTable *businesses = get_businesses_hash_table(sgr);

    /* Get category tree. */
    GTree *category_tree = getCategoryTree(sgr);
    
    /* Category name is not case sensitive. */
    char *category = g_strdup(key);
    category[0] = g_ascii_toupper(category[0]);
    
    /* Get list_of_ids of business from given category. */
    GList *list_of_ids_old = g_tree_lookup(category_tree, category);
    free(category);

    /* Sort list_of_ids by rating. */
    list_of_ids_old = g_list_sort_with_data(list_of_ids_old, compareBusinessRating, businesses);

    /* List is sorted in ascending order. "Goto" the end of the list to traverse in descending order. */
    list_of_ids_old = g_list_last(list_of_ids_old);

    /* Create lists... */
    GList *list_of_ids = NULL, *list_of_names = NULL, *list_of_ratings = NULL;
    
    int i = 1;
    for(; (list_of_ids_old != NULL) && (i <= top); ++i, list_of_ids_old = g_list_previous(list_of_ids_old)){
        /* Get business info. */
        BUS_HT_VALUE *bus_info = g_hash_table_lookup(businesses, list_of_ids_old->data);

        /* Convert float to char*. */
        char *stars = malloc(sizeof(char) * FLOAT_SIZE);
        snprintf(stars, sizeof(char) * FLOAT_SIZE, "%.3f", get_stars_business(bus_info));

        /* Insert id, name and stars into respective lists. */
        list_of_ids     = g_list_append(list_of_ids, g_strdup(list_of_ids_old->data));
        list_of_names   = g_list_append(list_of_names, g_strdup(get_name_business(bus_info)));
        list_of_ratings = g_list_append(list_of_ratings, stars);
    } 

    /* Prepend indexes/titles of each list. */    
    list_of_ids     = g_list_prepend(list_of_ids, g_strdup("business_id"));
    list_of_names   = g_list_prepend(list_of_names, g_strdup("name"));
    list_of_ratings = g_list_prepend(list_of_ratings, g_strdup("stars"));

    /* Get id's, names and rating of businesses and put into a Glist*. */
    GList **lists = malloc(sizeof(GList*) * 3);
    lists[0] = list_of_ids; lists[1] = list_of_names; lists[2] = list_of_ratings;

    /* Get table structure. */
    TABLE table_struct = listsToTable(lists, 3, i);

    /* Free allocated space. */
    g_list_free_full(list_of_ids, g_free);
    g_list_free_full(list_of_names, g_free);
    g_list_free_full(list_of_ratings, g_free); // free list of ratings
    free(lists);
    
    return table_struct;
}

TABLE reviews_with_word(SGR sgr, char *key){
    /* If dictionary is not loaded into memory. */
    if(!isDictionaryLoadedMem(sgr))
        load_dictionary(sgr);
    
    /* Get dictionary. */
    GHashTable *dictionary = getDictionary(sgr);

    /* Not case sensitive -> set all letters to lowercase */
    char *word = g_strdup(key);
    int i;
    for(i = 0; word[i] != '\0'; ++i) word[i] = g_ascii_tolower(word[i]);

    /* Search for word -> get list of review ids. */
    GList *list_of_ids = g_hash_table_lookup(dictionary, GINT_TO_POINTER(g_str_hash(word)));
    free(word);

    /* Deep copy list_of_ids */
    GList *list_of_ids_copy = g_list_copy_deep(list_of_ids, copy_list, NULL);

    /* Prepend title. */
    list_of_ids_copy = g_list_prepend(list_of_ids_copy, g_strdup("review_id"));

    /* Allocate memory to array of lists. */
    GList **lists = malloc(sizeof(GList*));
    lists[0] = list_of_ids_copy;

    /* Get TABLE. */
    TABLE table = listsToTable(lists, 1, g_list_length(list_of_ids_copy));

    /* Free allocated memory. */
    g_list_free_full(list_of_ids_copy, g_free);
    free(lists);

    return table;    
}
