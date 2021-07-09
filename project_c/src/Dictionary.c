#include "../includes/sgr.h"

/* For each word - insert in dictionary + prepend id into list_of_ids. */
int update_dictionary(GHashTable *dictionary, char *text, char *id){
    /* Copy text - so that we do not mess up the original buffer */
    char *review = g_strdup(text);
        
    // Read word by word
    char *word = calloc(WORD_SIZE+1, sizeof(char));
    int i, j;
    for(i = 0, j = 0; review[i] != '\0'; ++i){
        if(g_ascii_isalnum(review[i])){
            if(j < WORD_SIZE) // maximum lenght of word
                word[j++] = g_ascii_tolower(review[i]);  // copy word content to another variable
        }
        else{
            word[j] = '\0'; // close string - might have "garbage" in k>j indexes.
            /* Words that matter. */
            if((j < WORD_SIZE) && (j > 0)){
                /* Get hash from word. */
                guint64 key = g_str_hash(word);                

                /* Get list of review id's from dictionary. */
                GList *list_of_ids = g_hash_table_lookup(dictionary, GINT_TO_POINTER(key));

                /* If this review already has the word, skip to next word. */
                gboolean repeated = FALSE; // NOT repeated by default
                if(list_of_ids != NULL)
                    repeated = g_strcmp0(id, (char*) list_of_ids->data) == 0? TRUE : FALSE;
                
                // If NOT repeated -> associate review id with the word.
                if(!repeated){
                    /* Prepend review id to the list. */
                    list_of_ids = g_list_prepend(list_of_ids, g_strdup(id));

                    /* Update dictionary. */
                    g_hash_table_replace(dictionary, GINT_TO_POINTER(key), list_of_ids);
                }
            }
            j = 0;
        }
    }

    /* Free allocated memory. */
    free(review);
    free(word);

    return 0;
}

/* Checks if business and user id are valid. Returns TRUE if they are. */
gboolean check_business_and_user(SGR sgr, char *bus_id, char *usr_id){
    if(!sgr) return FALSE; // invalid sgr

    /* Get business & users hash table. */
    GHashTable *bus   = get_businesses_hash_table(sgr);
    GHashTable *users = get_users_hash_table(sgr);

    return g_hash_table_contains(bus, bus_id) && g_hash_table_contains(users, usr_id);
}

/* Skip first part of the review (until text) & gets review id. */
char *ignoreReviewsLine(char *line, char *rev_id, char *usr_id, char *bus_id){
    // rev_id;usr_id;bus_id;stars;useful;funny;cool;date;text\n 
    // Get & close review id
    int i;
    for(i = 0; line[i] != ';'; ++i)
        rev_id[i] = line[i];
    rev_id[i] = '\0';

    int idx;
    for(idx = 0, i++; line[i] != ';'; ++i, idx++)
        usr_id[idx] = line[i];
    usr_id[idx] = '\0';

    for(idx = 0, i++; line[i] != ';'; ++i, idx++)
        bus_id[idx] = line[i];
    bus_id[idx] = '\0';

    // Skip 6* -> ';'
    int c;
    for(c = 0; line[i] != '\0' &&  c < 6; ++i)
        if(line[i] == ';') ++c;
    
    return line + i;
}

int load_dictionary(SGR sgr){
    /* Open reviews file. */
    FILE *rev_file = fopen(getRevFilepath(sgr), "r");

    /* Check for invalid file pointer. */
    if (!rev_file) {
        perror("bus_file_error: ");
        return -1;
    }

    /* Initiate glib structure. */
    GHashTable *dictionary = initDictionary(sgr);

    // Allocate memory to buffer -> block-by-block processing
    char *buffer = calloc(BLOCK_SIZE, sizeof(char));
    size_t bytes_read = 0; // stores number of bytes read with fread().

    // Allocate memory for review id.
    char *rev_id = calloc(ID_SIZE, sizeof(char));
    char *bus_id = calloc(ID_SIZE, sizeof(char));
    char *usr_id = calloc(ID_SIZE, sizeof(char));

    /* Ignores first line. */
    if (fscanf(rev_file, "%*[^\n]\n") == EOF) return -1;

    /* Reading chunks of BLOCK_SIZE size into buffer using fread(). */
    while((bytes_read = fread(buffer, sizeof(char), BLOCK_SIZE, rev_file)) > 0){
		
        // Ignore the last (incomplete) line, if needed.   	
        int i = 0;
        for(i = bytes_read / sizeof(char); buffer[i-1] != '\n'; --i);
        buffer[i-1] = '\0';
    	
        // Rearrange the file pointer to read this last line in the next block.
        fseek(rev_file, - (bytes_read - i * sizeof(char)), SEEK_CUR);
    
        // Process each line
        char *line;
        if((line = strtok(buffer, "\n")) != NULL){ 
            do{
                /* Ignore first part of the line (id's & stars) */
                char *text = ignoreReviewsLine(line, rev_id, usr_id, bus_id);

                /* Check if business & user are valid: then, update */
                if(check_business_and_user(sgr, bus_id, usr_id))
                    update_dictionary(dictionary, text, rev_id);

            }while((line = strtok(NULL, "\n")) != NULL);
        }
    }


    /* Close reviews file. */
    fclose(rev_file);

    /* Free allocated memory. */
    free(buffer);
    free(rev_id);
    free(bus_id);
    free(usr_id);

    /* Dictionary has to be in memory. */
    setDictionaryLoadedMem(sgr, TRUE);
    
    return 0;
}
