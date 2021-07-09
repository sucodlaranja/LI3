#include "../includes/LoadFiles.h"

/******************************************************************************
 *                              BUSINESS.CSV
 *              id;name;city;state;category1,category2,,,\n
 * 
 * Hash Table: key = id, value = BUS_HT_VALUE = (name, city, state)
 * 
 * BalancedBT: node_key = A-Z, node_value = list of business names
 * 
 * BalancedBT: node_key = city names, node_value = list of business id's
 * 
 * BalancedBT: node_key = categories, node_value = list of business id's
 * 
 * 
 * testing -> perf stat -r 100 -d ./sgr
 * 
 * 
 * 
 ****************************************************************************** 
*/


/** 
 * Structures related with the business database. 
*/
/* Value of Business Hash Table. */
typedef struct bus_ht_value{
    char *name;
    char *city;
    char *state;
    float stars;
    int n_reviews;
}BUS_HT_VALUE;

/* Allocate space to BUS_HT_VALUE. */
BUS_HT_VALUE *init_bus_ht_value(char *name, char *city, char *state){
    BUS_HT_VALUE *value = malloc(sizeof(struct bus_ht_value));
    value->name = g_strdup(name);
    value->city = g_strdup(city);
    value->state = g_strdup(state);
    value->stars = 0.0;
    value->n_reviews = 0;
    return value;
}

/* Free  BUS_HT_VALUE.*/
void free_bus_ht_value(gpointer p){
  BUS_HT_VALUE *value = p;
  free(value->name);
  free(value->city);
  free(value->state);
  free(value);
}

/* Associates s business with N categories. Not yet sorted by stars! */
int update_category_tree(GTree *categories_tree, char **category, int numCategories, char *id){
    int i;
    for(i = 0; i < numCategories; ++i){

        // Already allocated list of business
        GList *list_of_ids = g_tree_lookup(categories_tree, category[i]);
        
        // Preprend the new value.
        list_of_ids = g_list_prepend(list_of_ids, g_strdup(id));        

        // Replace/Add the category to the tree.
        g_tree_replace(categories_tree, g_strdup(category[i]), list_of_ids);
    }

    return 0;
}

/* Process string with format: "id;name;city;state;" */
int processLine(char *line, char *id, char *name, char *city, char *state){
    int i;
    for(i = 0; line[i] != ';' && line[i] != '\0'; ++i) id[i] = line[i];
    id[i] = '\0';
    if(line[i] != '\0') i++; // skip ';'
    
    int j;
    for(j = 0; line[i] != ';' && line[i] != '\0'; ++i, j++) name[j] = line[i];
    name[j] = '\0';
    if(line[i] != '\0') i++; // skip ';'

    for(j = 0; line[i] != ';' && line[i] != '\0'; ++i, j++) city[j] = line[i];
    city[j] = '\0';
    if(line[i] != '\0') i++; // skip ';'

    for(j = 0; line[i] != ';' && line[i] != '\0'; ++i, j++) state[j] = line[i];
    state[j] = '\0';


   return line[i] != '\0'? i + 1 : -1;
}

/* Process string: "c1, c2,...,cn\n" and returns number of categories.*/
int processCategories(char *categories, char **save_categories){
    int numCategories = 0;
    // Check if business has no categories
    if(categories[0] != '\0'){
        int i = 0;
        do{
            int j;
            char category[CATEGORY_SIZE];
            for(j = 0; categories[i] != ',' && categories[i] != '\0'; i++, j++){
                if(categories[i] == ';') return -1;
                category[j] = categories[i];
            }
            category[j] = '\0';
            category[0] = g_ascii_toupper(category[0]); // not case sensitive
            strcpy(save_categories[numCategories], category);
            numCategories++;
        }while(categories[i++] != '\0');
    }
    
    return numCategories;
}

/* Load business file. */
int load_business_file ( FILE *bus_file,
                         GHashTable *business_table,
                         GTree *business_names_tree,
                         GTree *cities_tree,
                         GTree *categories_tree)
{
    /* Check for invalid file pointer. */
    if(!bus_file){
        perror("bus_file_error: ");
        return 1;
    }

    // Allocate memory to buffer -> block-by-block processing
    char *buffer = calloc(BLOCK_SIZE, sizeof(char));
    size_t bytes_read = 0; // stores number of bytes read with fread().

    // Variables
    char *id = calloc(ID_SIZE, sizeof(char));
    char *name = calloc(NAME_SIZE, sizeof(char));
    char *city = calloc(CITY_SIZE, sizeof(char));
    char *state = calloc(STATE_SIZE, sizeof(char));
    char **category_table = calloc(MAX_CATEGORIES, sizeof(char*));
    int k;
    for(k = 0; k < MAX_CATEGORIES; ++k) category_table[k] = calloc(CATEGORY_SIZE, sizeof(char));


    // Ignore first line of file ??? option 1 vs option 2 vs nothing
    if (fscanf(bus_file, "%*[^\n]\n") == EOF) return -1; // Ignores first line

    /* Reading chunks of BLOCK_SIZE size into buffer using fread(). */
    while((bytes_read = fread(buffer, sizeof(char), BLOCK_SIZE, bus_file)) > 0){
		
        // Ignore the last (incomplete) line, if needed.   	
        int i = 0;
        for(i = bytes_read / sizeof(char); buffer[i-1] != '\n'; --i);
        buffer[i-1] = '\0';
    	
        // Rearrange the file pointer to read this last line in the next block.
        fseek(bus_file, - (bytes_read - i * sizeof(char)), SEEK_CUR);

        // Process each line
        char *line;
        // Start strtok.
        if((line = strtok(buffer, "\n")) != NULL){ 
            do{
                // Lists
                GList *prevL = NULL;
                GList *newL = NULL;

                // Get id, name, city, state and return number of char's read.
                int nCharRead = processLine(line, id, name, city, state);

                // Categories are beyond the last ';'
                char *categories = line + nCharRead;
                
                // Get all the categories that this business is associated with.
                int numCategories = processCategories(categories, category_table);

                // If line is valid, keep going -> otherwise, abort line
                if(nCharRead != -1 && numCategories != -1){

                    // Insert business in database (HashTable).
                    BUS_HT_VALUE *ht_value = init_bus_ht_value(name, city, state);
                    g_hash_table_insert(business_table, g_strdup(id), ht_value);

                    // Add business name to the balanced btree (organized by 1st letter)
                    prevL = g_tree_lookup(business_names_tree, GINT_TO_POINTER(g_ascii_toupper(name[0])));
                    newL = g_list_prepend(prevL, g_strdup(id));
                    g_tree_replace(business_names_tree, GINT_TO_POINTER(g_ascii_toupper(name[0])), newL);

                    // Add city to tree or update list of businesses in a city.
                    city[0] = g_ascii_toupper(city[0]); // portland == Portland ?
                    prevL = g_tree_lookup(cities_tree, city);
                    newL = g_list_prepend(prevL, g_strdup(id));
                    g_tree_replace(cities_tree, g_strdup(city), newL);
                
                    // Add category to tree or update list of businesses in certain category. 
                    update_category_tree(categories_tree, category_table, numCategories, id);
                }

            }while((line = strtok(NULL, "\n")) != NULL);  
        }
    }

    free(buffer); free(id); free(name); free(city); free(state);
    for(k = 0; k < MAX_CATEGORIES; ++k) free(category_table[k]);
    free(category_table);

    fclose(bus_file);
    return 0;
}

/***************************************************************
 *                         USERS.CSV
 * Load users.csv.
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 *
 * 
 * 
 * 
 ****************************************************************
 */

/**
 * Structures related with the users database.
*/
/* Value of Users Hash Table. */
struct users_ht_value {
    char* name;
    GList* list_reviews;
};

/* Frees the memory of the respective struct.*/
void free_users_ht_value(gpointer p) {
    USERS_HT_VALUE* value = p;
    free(value->name);
    g_list_free_full(value->list_reviews, free);
    g_free(p);
}

/* Stores the elements on the hash table.*/
void processLineUsers(GHashTable* users_table, char* u_id, char* u_name) {
    USERS_HT_VALUE* value = malloc(sizeof(USERS_HT_VALUE));
    value->name = g_strdup(u_name);
    value->list_reviews = NULL;

    g_hash_table_insert(users_table, g_strdup(u_id), value);
}

/* Reads the users file and stores on the hash table.*/
int load_users_file(FILE* filepointer, GHashTable* users_table) {

    /* Check for invalid file pointer. */
    if (!filepointer) {
        perror("bus_file_error: ");
        return -1;
    }

    if (fscanf(filepointer, "%*[^\n]\n") == EOF) return -1; // Ignores first line

    //Initializes the buffer to read the file and other variables.
    char* buffer = malloc(sizeof(char) * BLOCK_SIZE);
    size_t bytes_read;
    char* u_id,* u_name;

    while ((bytes_read = fread(buffer, sizeof(char), BLOCK_SIZE, filepointer)) > 0) {
        // Ignore the last (incomplete) line, if needed; file ends with \n
        int i;
        for (i = (bytes_read - 1) / sizeof(char); buffer[i] != '\n'; --i);
        buffer[i] = '\0';
        // rearrange the file pointer to read this last line in the next block
        fseek(filepointer, -(bytes_read - i * sizeof(char)) + 1, SEEK_CUR);
        
        // Reads the first line in the block
        u_id = strtok(buffer, ";");
        u_name = strtok(NULL, ";");
        processLineUsers(users_table, u_id, u_name);
        strtok(NULL, "\n"); // Ignores the rest of the line.

        // Read line by line and store on the hash Table
        while ((u_id = strtok(NULL, ";")) != NULL ) {
            u_name = strtok(NULL, ";");
            processLineUsers(users_table, u_id, u_name); // Insert on the hash Table
            strtok(NULL, "\n"); // Ignores the rest of the line.
        }
    }

    free(buffer);
    fclose(filepointer);
    return 0;
}

/***************************************************************
 *                         REVIEWS.CSV
 * Load reviews.csv.
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 *
 * 
 * 
 * 
 ****************************************************************
 */

/* Updates the rating of a business.*/
void updateRating(BUS_HT_VALUE *value, float rev) {

    int n_reviews = value->n_reviews;
    float stars = value->stars;

    float temp = n_reviews * stars + rev;
    float newRating = temp / (++n_reviews);

    value->n_reviews = n_reviews;
    value->stars = newRating;
}

/* Stores the information on the respectives hash tables.*/
void processLineReviews(GHashTable* users_table, char* user_id, char* buss_id, char* stars, GHashTable* business_table) {
    char* business_id = g_strdup(buss_id);
    USERS_HT_VALUE* value;

    value = g_hash_table_lookup(users_table, user_id);
    value->list_reviews = g_list_prepend(value->list_reviews, business_id);

    /* Update business rating. !Valgrind error here!*/
    updateRating(g_hash_table_lookup(business_table, buss_id), atof(stars));
}

/* Reads the reviews file and stores on the hash table.*/
int load_reviews_file(FILE* filepointer, GHashTable* users_table, GHashTable* business_table) {

    /* Check for invalid file pointer. */
    if (!filepointer) {
        perror("bus_file_error: ");
        return -1;
    }

    if (fscanf(filepointer, "%*[^\n]\n") == EOF) return -1; // Ignores first line.

    //Initializes the glib hash table, the buffer to read the file and other variables.
    char* buffer = malloc(sizeof(char) * BLOCK_SIZE);
    size_t bytes_read;
    char* user_id, * buss_id, * stars;

    while ((bytes_read = fread(buffer, sizeof(char), BLOCK_SIZE, filepointer)) > 0) {
        // Ignore the last (incomplete) line, if needed; file ends with \n
        int i;
        for (i = (bytes_read - 1) / sizeof(char); buffer[i] != '\n'; --i);
        buffer[i] = '\0';
        // rearrange the file pointer to read this last line in the next block
        fseek(filepointer, -(bytes_read - i * sizeof(char)) + 1, SEEK_CUR);

        strtok(buffer, ";");
        user_id = strtok(NULL, ";");
        buss_id = strtok(NULL, ";");
        stars   = strtok(NULL, ";");
        processLineReviews(users_table, user_id, buss_id, stars, business_table);
        
        strtok(NULL, "\n"); // ignore rest of the line
        
        // Read line by line and store on the hash Table
        while (strtok(NULL, ";") != NULL) {
            user_id = strtok(NULL, ";");
            buss_id = strtok(NULL, ";");
            stars = strtok(NULL, ";");
            processLineReviews(users_table, user_id, buss_id, stars, business_table);
            
            strtok(NULL, "\n"); // ignore rest of the line
        }
    }

    free(buffer);
    fclose(filepointer);
    return 0;
}

/*getters*/
GList* get_list_users(USERS_HT_VALUE* users_value) {return users_value->list_reviews;}

char* get_state_business(BUS_HT_VALUE* value) {return value->state;}

char* get_name_business(BUS_HT_VALUE* business_table) {return business_table->name;}

char *get_city_business(BUS_HT_VALUE* business_table){return business_table->city;}

float get_stars_business(BUS_HT_VALUE* business_table){return business_table->stars;}

int get_number_reviews_business(BUS_HT_VALUE* business_table){return business_table->n_reviews;}