#include "../includes/Table.h"

#define PAGE_SIZE 10

/*struct*/
struct table_struct{
    char *var_name;
    int lines;
    int columns;
    char* **tbl;
};

/*Getters*/
int get_Lines(TABLE table) {return table->lines;}
int get_Columns(TABLE table) {return table->columns;}
char * getValue(TABLE table,int linha,int coluna) {return table->tbl[linha][coluna];}
char* getVar_name(TABLE table) {return table->var_name;}
char*** get_tbl(TABLE table) { return table->tbl; } 
void setVar_name(TABLE table,char* name) {
    table->var_name = g_strdup(name);
}

//Function that finds variable var_name in Table
TABLE look_Table(GList *tabela, char* var_name) {
    GList *temp = tabela;
    TABLE table;
    for(; temp; temp = temp->next) {
        table = temp->data;
        if(strcmp(table->var_name,var_name) == 0) return table;
    }
    return NULL;
}

TABLE initTable(int L, int C) {
    TABLE t = malloc(sizeof(struct table_struct));
    t->lines = L;
    t->columns = C;
    t->var_name = NULL;

    t->tbl = malloc(sizeof(char*) * L);

    int i;
    for (i = 0; i < L; ++i)
        t->tbl[i] = malloc(sizeof(char*) * C);
    
    return t;
}

void freeTable(TABLE t) {
    if(t != NULL){
        int i, j;
        for (i = 0; i < t->lines; ++i) {
            for (j = 0; j < t->columns; ++j)
                free(t->tbl[i][j]);
            free(t->tbl[i]);
        }

        free(t->tbl);
        if(t->var_name != NULL) free(t->var_name);
        free(t);
    }
}


// Prints one one line with the respective spaces and right format (one unusual case to fix (El Señor Taco))
void makeLine(TABLE table,int coluna) {
    int linha = table->lines;
    
    delim_Maker(linha);
    
    int size,space;
    for(int i=0;i < linha;i++) {
        printf("|");
        if(table->tbl[i][coluna] != NULL) {
        size = strlen(table->tbl[i][coluna]);
        space = (34 - size) / 2;
        
        
        if(size%2 == 0 && i%2 == 0) repeat_char(space-1,' '); 
        else if ((size%2 == 0 && i%2 != 0) || (size%2 !=0 && i%2 == 0) ) repeat_char(space,' ');
        else repeat_char(space+1,' ');
        printf("%s",table->tbl[i][coluna]);
        repeat_char(space,' ');
        }
        else repeat_char(34, ' ');
        
        
    }
    printf("|\n");
       
}

//prints the table 5 by 5 (can change) elements
void maketable(TABLE table,int inicio,int fim) {
    makeLine(table,0);                                  //prints the index first
    for(int i = inicio; i < fim; i++) {
        makeLine(table,i);  
    }
    delim_Maker(table->lines);
}


void show(TABLE table){
    
    if(!table) return; // imprimir erro ou algo do género

    int colunas = table->columns;
    int pages = (colunas)/PAGE_SIZE,inicio = 1,fim,page = 1;
    if(colunas < PAGE_SIZE) pages++;
    if(colunas <= PAGE_SIZE) fim = colunas; else fim = PAGE_SIZE+1;
    
    char comando[10];
    
    maketable(table,inicio,fim);
    printf("Page %d of %d\n\n",page,pages);                                                //prints the page and pages numbers 
    printf("Show mode controls (write): next/previous, leave or clear\n");                                                 
    while(1) {
        
    
        char * empty = fgets(comando,10,stdin);                                            //Receives the command of page mode
        if(empty) {
            if(strcmp(comando,"next\n") == 0) {                                                   
                if(page == pages) printf("This is the last page\n");
                else {
                    inicio = fim;
                    if(colunas > 2*fim) fim +=PAGE_SIZE;
                    else fim = colunas;
                    page++;
                    maketable(table,inicio,fim);
                    printf("Page %d of %d\n\n",page,pages);
                    printf("Show mode controls (write): next/previous, leave or clear\n");
                }

            }

            else if(strcmp(comando,"previous\n") == 0)  {
                if(page == 1) printf("this is the first page\n");
                else {
                    fim = inicio;
                    inicio-=PAGE_SIZE;
                    page--;
                    maketable(table,inicio,fim);
                    printf("Page %d of %d\n\n",page,pages);
                    printf("Show mode controls (write): next/previous, leave or clear\n");
                }
            }
            else if(strcmp(comando,"clear\n") == 0) {
                printf("\e[1;1H\e[2J");
                printf("Page %d of %d\n\n",page,pages);
                printf("Show mode controls (write): next/previous, leave or clear\n");
            }

            else if(strcmp(comando,"leave\n") == 0) {
                printf("Exited Show Mode\n");
                break;
            }
        }
    }
    
    
    
}

//filter function
 TABLE filter(TABLE table,char * column_name, char* value, OPERATOR oper) { 
     
    TABLE new = NULL;      
    int coluna,linhaN,linha;
    for(linha = 0; linha < table->lines; linha++) if(strcmp(column_name,table->tbl[linha][0]) == 0) break;    //Locates the column first
    
    if(linha == table->lines) printf("Column does not exist\n");                                                 //verifies if the column really exists
    
    else {
        new = malloc(sizeof(struct table_struct));
        new->tbl = malloc(sizeof(char*));          
        new->tbl[0] = malloc(sizeof(char*)*2); 
        int dinamicsize = 2; 
        
        
                                                                                           //so that we can reallocate space
        new->tbl[0][0] = g_strdup(table->tbl[linha][0]);                                                 //transfers the indice                                                                           
        if(oper == EQ) {    
            for(coluna = 1,linhaN = 1; coluna < table->columns; coluna++) {
                if(strcmp(value,table->tbl[linha][coluna]) == 0) {
                    if(linhaN >= dinamicsize) {
                        dinamicsize*=2; 
                        new->tbl[0] = realloc(new->tbl[0],dinamicsize*sizeof(char*));
                    }
                    new->tbl[0][linhaN] = g_strdup(table->tbl[linha][coluna]);
                    linhaN++;
                }
            }
        }
        else if(oper == GT) {
            for(coluna = 1,linhaN = 1; coluna < table->columns; coluna++) {
                if(strcmp(value,table->tbl[linha][coluna]) > 0) {
                    if(linhaN >= dinamicsize) {
                        dinamicsize*=2; 
                        new->tbl[0] = realloc(new->tbl[0],dinamicsize*sizeof(char*));
                    }
                    new->tbl[0][linhaN] = g_strdup(table->tbl[linha][coluna]);
                    linhaN++;
                }
            }
        }
        else {
            for(coluna = 1,linhaN = 1; coluna < table->columns; coluna++) {
                if(strcmp(value,table->tbl[linha][coluna]) < 0) {
                    if(linhaN >= dinamicsize) {
                        dinamicsize*=2; 
                        new->tbl[0] = realloc(new->tbl[0],dinamicsize*sizeof(char*));
                    }
                    new->tbl[0][linhaN] = g_strdup(table->tbl[linha][coluna]);
                    linhaN++;
                }
            }
        }
        new->lines = 1;
        new->columns = linhaN;
        //new->sizes = table->sizes; tirei isto só para compilar, dps vê se faz falta xD
    }
        
    return new;
}

/*Finds out index number of given column name returns -1 if column does not exist in this table*/
int compareColumn(TABLE table, char* column) {
    
    for(int i = 0; i<table->lines; i++) {
        if(strcmp(table->tbl[i][0],column) == 0) return i;
    }
    return -1;
}

/*Transfers all content in origin from determined column to new table */
int projTable(TABLE origin,TABLE new,char* column,int linha) {
    int pos = compareColumn(origin,column);
    
    if(pos == -1) {
        printf("Column %s does not exist in table %s\n",column, origin->var_name); 
        return -1;
    }
    else {
        for(int i = 0; i < origin->columns;i++) {
            new->tbl[linha][i] = g_strdup(origin->tbl[pos][i]);
        }
        return 1;
    }
}

/* Given an line, divides each string and stores them on a list, preppendind the list on a matriz.*/
GList* processline(char* line, GList* mat, int firstlineBlock,char sep) {
    int maxsize, count = 0;
    if (firstlineBlock) line[-1] = '\n';
    GList* new = NULL;

    for (int i = 0; line[i- firstlineBlock] ; count++) {
        if (i == 0) firstlineBlock = 1;
        maxsize = 32;
        char* string = malloc(maxsize);
        int j = 0;

        //Reads a each string.
        for (; line[j + i] != sep && line[j + i]; j++) {
            string[j] = line[j + i];
            if (j == maxsize -1) {
                maxsize *= 2;
                string = realloc(string,maxsize);
            }
        }
        string[j++] = '\0'; // Safe guard.
        i += j;
        new = g_list_prepend(new, string);
        
    }

    return g_list_prepend(mat, new);
}

/* Given a matriz transforms it into a table */
TABLE matToTable(GList* mat) {
    if (mat) {

        // Mallocs all of the table whit the right structures
        TABLE table = initTable(g_list_length(mat->data), g_list_length(mat));

        //Inserts all of the strings onto the table
        mat = g_list_last(mat);
        for (int i = 0; mat; mat = mat->prev, i++) {
            GList* line = g_list_last(mat->data);
            for (int j = 0; line && j < table->lines; line = line->prev, j++) {
                table->tbl[j][i] = line->data;
            }
        }
        return table;
    }
    else return NULL;
}

void freeGListMat(GList* mat){ for(;mat;mat = mat->next) g_list_free(mat->data); }

// Given an file transform it into a Table
TABLE fromFile(FILE* filepointer,char sep) {

    /* Check for invalid file pointer. */
    if (!filepointer) {
        perror("bus_file_error: ");
        return NULL;
    }

    //Initializes the glib hash table, the buffer to read the file and other variables.
    char* buffer = malloc(sizeof(char) * BLOCK_SIZE_TABLE),* aux;
    size_t bytes_read;
    GList* mat = NULL;
    
    while ((bytes_read = fread(buffer, sizeof(char), BLOCK_SIZE_TABLE, filepointer)) > 0) {
        // Ignore the last (incomplete) line, if needed; file ends with \n
        int i;
        for (i = (bytes_read - 1) / sizeof(char); buffer[i] != '\n'; --i);
        buffer[i] = '\0';
        // rearrange the file pointer to read this last line in the next block
        fseek(filepointer, -(bytes_read - i * sizeof(char)) + 1, SEEK_CUR);

        //First line of ther block
        if ((aux = strtok(buffer, "\n")) != NULL) mat = processline(aux, mat, 0,sep);
        else break;
        while ((aux = strtok(NULL,"\n")) != NULL) mat = processline(aux, mat, 1,sep);
    }
    free(buffer);
    fclose(filepointer);
    TABLE table = matToTable(mat);
    freeGListMat(mat);
    g_list_free(mat);
    return table;
}

int toFile(FILE* filepointer, TABLE table, char sep, char newline) {

    /* Check for invalid file pointer. */
    if (!filepointer) {
        perror("bus_file_error: ");
        return -1;
    }

    //Initializes the glib hash table, the buffer to read the file and other variables.
    char* buffer = malloc(sizeof(char) * BLOCK_SIZE_TABLE);
    size_t size_buffer = 0, size;

    for (int i = 0; i < table->columns; i++) {
        for (int j = 0; j < table->lines; j++) {
            size = strlen(table->tbl[j][i]) + 1;

            // If there is no space in the buffer writes the info on the file and resets the counter 
            if (size_buffer + size >= BLOCK_SIZE_TABLE) {
                fwrite(buffer, size_buffer, 1, filepointer);
                size_buffer = 0;
            }
            
            //Writes the info on the buffer
            if (j == table->lines -1) sprintf(buffer + size_buffer, "%s%c", table->tbl[j][i], newline);
            else sprintf(buffer + size_buffer, "%s%c", table->tbl[j][i], sep);
            size_buffer += size;
        }
    }
    // writes the last block on the file
    
    fwrite(buffer, size_buffer, 1, filepointer);
    free(buffer);
    fclose(filepointer);
    return 0;
}

void freeGList(GList* Tabela) {
    TABLE temp = NULL;
    for(;Tabela;Tabela = Tabela->next) {
        temp = Tabela->data;
        freeTable(temp);
    }
}