#pragma warning(disable : 4996)
#pragma warning(disable : 4146)

#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <string.h>

#define BLOCK_SIZE 1048576 // 1MBytes

/*
    para fzr benchmark (corre o programa 100 vezes) usar comando abaixo
         perf stat -r 100 -d ./a.out

    para comparar processamento com strtok ou char by char
*/

typedef struct value_users {
    char name[250];
    GList* list_reviews;
}Vusers;

typedef struct value_reviews {
    char review_id[22];
    char business_id[22];
}Vreviews;

void debug_print_hTprint(GHashTable* hT) {
    GHashTableIter iter;
    char* key; Vusers* value; int cc = 0;

    g_hash_table_iter_init(&iter, hT);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        printf("%d-> user_id:%s name:%s ", cc++, key, value->name);
        GList* new = value->list_reviews;
        for (; new != NULL; new = new->next) {
            Vreviews* val = new->data;
            printf("review_id:%s ", val->review_id);
        }
        printf("\n");
    }
}

GHashTable* load_users(char* filepath) {

    int cc = 0; // Remove PLS 

    // Open the file and check if it is valid.
    FILE* f = fopen(filepath, "r");
    if (!f) {
        perror("bus_file_error: ");
        return;
    }

    //Initializes the glib hash table, the buffer to read the file and other variables.
    GHashTable* hTable = g_hash_table_new(g_str_hash, g_str_equal);
    char* buffer = malloc(sizeof(char) * BLOCK_SIZE);
    size_t bytes_read;

    while ((bytes_read = fread(buffer, sizeof(char), BLOCK_SIZE, f)) > 0) {
        // Ignore the last (incomplete) line, if needed; file ends with \n
        int i;
        for (i = bytes_read / sizeof(char); buffer[i] != '\n'; --i);
        buffer[i] = '\0';
        // rearrange the file pointer to read this last line in the next block
        fseek(f, -(bytes_read - i * sizeof(char)) + 1, SEEK_CUR);

        char* rest = buffer, *line;
        strtok_s(buffer, "\n", &rest); // Ingnore the first line of the file, "user_id;name;friends".  
        
        // Read line by line and store on the hash Table
        while ((line = strtok_s(NULL, "\n", &rest)) != NULL) {
            char* user_id = malloc(22);
            char* restL = line;
            Vusers* value = malloc(sizeof(Vusers));
            value->list_reviews = NULL;

            strcpy(user_id, strtok_s(line, ";", &restL));
            strcpy(value->name, strtok_s(NULL, ";", &restL));
            strtok_s(NULL, ";", &restL); // Ignore list of friends

            g_hash_table_insert(hTable, user_id, value); // Insert on the hash Table 
            //printf("%d-> user_id:%s name:%s\n", cc++, user_id, value->name);
        }
    }

    free(buffer);
    fclose(f);
    return hTable;
}


void load_reviews(char* filepath, GHashTable* htUsers) {
    // Open the file and check if it is valid.
    FILE* f = fopen(filepath, "r");
    if (!f) {
        perror("bus_file_error: ");
        return;
    }

    //Initializes the glib hash table, the buffer to read the file and other variables.
    char* buffer = malloc(sizeof(char) * BLOCK_SIZE);
    size_t bytes_read;

    while ((bytes_read = fread(buffer, sizeof(char), BLOCK_SIZE, f)) > 0) {
        // Ignore the last (incomplete) line, if needed; file ends with \n
        int i;
        for (i = bytes_read / sizeof(char); buffer[i] != '\n'; --i);
        buffer[i] = '\0';
        // rearrange the file pointer to read this last line in the next block
        fseek(f, -(bytes_read - i * sizeof(char)) + 1, SEEK_CUR);

        char* rest = buffer, *line;
        strtok_s(buffer, "\n", &rest); // Ingnore the first line of the file, "user_id;name;friends".  

        int cc = 0; // Remove PLS 
        
        // Read line by line and store on the hash Table
        while ((line = strtok_s(NULL, "\n", &rest)) != NULL) {
            char* user_id = malloc(22);
            char* restL = line;
            Vreviews* value = malloc(sizeof(Vreviews));

            strcpy(value->review_id, strtok_s(line, ";", &restL));
            strcpy(user_id, strtok_s(NULL, ";", &restL));
            strcpy(value->business_id, strtok_s(NULL, ";", &restL));

            strtok_s(NULL, ";", &restL); // Ignore list of friends
            strtok_s(NULL, ";", &restL);
            strtok_s(NULL, ";", &restL);

            strtok_s(NULL, ";", &restL);
            strtok_s(NULL, ";", &restL);

            //printf("review_id:%s business_id:%s\n", value->review_id, value->business_id);

            if (g_hash_table_contains(htUsers, user_id)) {
                Vusers* val = g_hash_table_lookup(htUsers, user_id);
                val->list_reviews = g_list_append(val->list_reviews, value);
                g_hash_table_replace(htUsers, user_id, val);
            }
        }
    }

    free(buffer);
    fclose(f);
}


void copyPaste(char* filepath, char* filepathnew, int bytes) {
    char* buffer = malloc(BLOCK_SIZE);

    FILE* f = fopen(filepath, "r");
    FILE* new = fopen(filepathnew, "w");

    for (int i = 0; i < bytes; i++) {
        fread(buffer, BLOCK_SIZE, 1, f);
        fwrite(buffer, BLOCK_SIZE, 1, new);
    }

    fwrite("\n", 1, 1, new);

    fclose(f);
    fclose(new);

}

int main()
{
    //copyPaste("D:/LI3FILES/users_full.csv", "D:/LI3FILES/testfile.csv",3);
    GHashTable* htUsers = load_users("D:/LI3FILES/users_full.csv");
    load_reviews("D:/LI3FILES/reviews_1M.csv", htUsers);
    debug_print_hTprint(htUsers);

    return 0;
}