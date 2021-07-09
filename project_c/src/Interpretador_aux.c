#include "../includes/Interpretador.h"

#define MaxInter 30
#define MAXFILEPATH 100


void printsList(GList *tabela) {
    
    TABLE temp = NULL;
    GList *tab = tabela;
    for(;tab->next;tab = tab->next) {
        temp = tab->data;
        printf("\"%s\",",getVar_name(temp));
        }
    temp = tab->data;
    printf("\"%s\".\n",getVar_name(temp));
    
}

void help(GList *tabela) {
    printf("Possible Commands:\n"
                    " -variable = Function name: saves a function in a variable\n\n"
                    " -show(variable): Prints the content of determined variable\n\n"
                    " -toCSV(variable,char delim,FILE* filepath): Puts content a determined TABLE into a .csv file\n\n"
                    " -load_sgr(char *users, char *businesses, char *reviews): loads all the files to the strucs.\n\n"
                    " -clear : clear screen.\n\n"
                    " -quit : exits program.\n\n"
                        "Possible Functions:\n "
                        " -businesses_started_by_letter(SGR sgr, char letter): prints all the bussinesses that start with \"letter\"\n\n"
                        " -business_info(SGR sgr, char *business_id): Prints all info of a business with the given id\n\n"
                        " -businesses_reviewed(SGR sgr, char *user_id)): Prints all the businesses that user_id reviewed\n\n"
                        " -businesses_with_stars_and_city(SGR sgr, float stars, char *city): Prints all the businesses with determined number of stars in determined city\n\n"
                        " -top_businesses_by_city(SGR sgr, int top): Prints all top businesses\n\n"
                        " -international_users(SGR sgr): Prints all international users\n\n"
                        " -top_businesses_with_category(SGR sgr, int top, char *category): Prints all top businesses in determined category\n\n"
                        " -reviews_with_word(SGR sgr,char *word): Prints all top reviews with determied word\n\n"
                        " -proj(x, cols): Makes a new table with a subset columns from x TABLE\n\n"
                        " -fromCSV(filepath, delim)"
                        " -filter(x, column_name, value, oper): Filter a column from TABLE x based on variable value with oper options(GT,EQ,LT)\n\n");
                       if(tabela) {
                           printf("\nTables in Program:");
                        printsList(tabela);
                       }
}

/*Test variables format for load_sgr */
int parse_variablesSGR(char* variables,char* first, char* second, char* third) {
    int i,size = strlen(variables),indice;
    for(indice = i = 0; variables[i] != ',' && variables[i] != ')' && i<size; i++,indice++) {
        first[indice] = variables[i];
    }
    
    if(strcmp(first,"") == 0 || variables[i] == ')' || i>=size) return -1;
    i++;

    for(indice = 0;variables[i] != ',' && variables[i] != ')' && i<size; i++, indice++) {
        second[indice] = variables[i];
    }
    if(strcmp(second,"") == 0 || variables[i] == ')' || i>=size) return -1;

    i++;
    for(indice = 0;variables[i] != ')' && variables[i] != ',' && i<size; i++, indice++) {
        third[indice] = variables[i]; 
    }

    if(strcmp(third,"") == 0 || variables[i] == ',') return -1;
    
    return 0;
}


/* Test variables format for "Filter" */
int parse_variablesFilter(char* variables,char* first, char* second, char* third,char* fourth) {
    int i,size = strlen(variables),indice;

    for(indice = i = 0; variables[i] != ',' && variables[i] != ')' && i<size; i++,indice++) {
        first[indice] = variables[i];
    }
    
    if(strcmp(first,"") == 0 || variables[i] == ')' || i>=size) return -1;
    i++;
    first[indice] = '\0';
    for(indice = 0;variables[i] != ',' && variables[i] != ')' && i<size; i++, indice++) {
        second[indice] = variables[i];
    }
    if(strcmp(second,"") == 0 || variables[i] == ')' || i>=size) return -1;
    second[indice] = '\0';
    i++;
    for(indice = 0;variables[i] != ',' && variables[i] != ')' && i<size; i++, indice++) {
        third[indice] = variables[i];
    }
    if(strcmp(third,"") == 0 || variables[i] == ')' || i>=size) return -1;

    third[indice] = '\0';
    i++;
    for(indice = 0;variables[i] != ')' && variables[i] != ',' && i<size; i++, indice++) {
        fourth[indice] = variables[i]; 
    }

    if(strcmp(fourth,"") == 0 || variables[i] == ',') return -1;
    fourth[indice] = '\0';
    return 0;
}

/* Test variables format for query with (sgr,string) arguments */
int parse_variablesString(char* variables,char* string) {
    int i = 0,indice, size = strlen(variables);

    for(; variables[i] != ',' && i < size; i++);                     //ignores SGR name
    
    if(  i >= size) return -1;

    i++;                                                                                   
    for(indice = 0; variables[i] != ',' && variables[i] != ')' && i<size; i++,indice++) {
           string[indice] = variables[i];
    }
    string[indice] = '\0';

    if(strcmp(string,"") == 0 || variables[i] == ',') return -1;

    return 0;          
}


/*tests and parses variables for queries with (int/float,char*) formats */
int parse_variablesCharNumber(char * variables, char * first, char* number) {

    int i = 0,indice, size = strlen(variables);

    for(; variables[i] != ',' && i < size; i++);                                            //ignores SGR name
    
    if(i >= size) return -1;

    i++;                                                                                    //So that ignores the ','

    //Puts the number in a string 
    for(indice = 0; variables[i] != ','  && i<size; i++,indice++) {
           number[indice] = variables[i];
    }
    number[indice] = '\0';


    if(strcmp(number,"") == 0 || i >= size) return -1;

    i++;                                                                                    //So that ignores the ','
    //for the String argument
    for(indice = 0; i<size && variables[i] != ',' ; i++,indice++) {
        first[indice] = variables[i];
    }
    first[indice] = '\0';

    if(strcmp(first,"") == 0 || variables[i] == ',') return -1;
    
    return 0;
 
}

/*for  businesses_started_by_letter, tests if the variables format are correct */
char parse_variablesChar(char* variables) {
    int i = 0;
    for(;variables[i] != ',';i++);
    
    char letter = variables[++i];
    
    if(variables[i+1] != '\0') return '\0';
    return letter;

}

/*tests if variables on fromcsv are correct */
char parse_variablesCSV(char*variables,char* filepath) {
    int i = 0,indice, size = strlen(variables);
    char delim;

    for(indice = 0; variables[i] != ','  && i<size; i++,indice++) {
           filepath[indice] = variables[i];
    }
    filepath[indice] = '\0';

    if(strcmp(filepath,"") == 0  ||  i >= size) return '\0';

    i++;
    
    delim=variables[i];
    if(i+1 != size) {
        printf("delim is not a char\n");
        return '\0';
    }
    return delim;
    
}

//For function proj, puts on "headers" variables the TABLE name and all headers, returns -1 if something goes wrong 
int GetHeaders(char* variables,char **headers) {
    int size = 0;
    int i,j;
    for(i = 0; size<6 && i<strlen(variables); i++,size++) {
        //printf("%s\n",variables);
        headers[size] = malloc(ID_SIZE+1);
        
        for(j = 0;variables[i] != ',' && i < strlen(variables);i++,j++) {
            headers[size][j] = variables[i];
            
        }
        headers[size][j] = '\0';
        
    }
    return size;
}

/*Recognizes the OPERATOR for the function filter*/
OPERATOR find_operator(char *operator) {
    if(strcmp(operator,"LT") == 0) {
        return LT;
    }
    else if(strcmp(operator,"GT") == 0) {
        return GT;
    }
    else return EQ;
}

/*Function that recognize the query that the user asked, tests the format and if its ok, makes the TABLE otherwise returns NULL and prints why*/
TABLE recognize_function(char* function,GList *tabela, SGR sgr) {

    char *function_name = strtok(function,"(");
    char *variables = strtok(NULL, "\0");
    TABLE new = NULL;
    
    if(variables != NULL) {
        
        /* Query 2*/
        if(strcmp(function_name,"businesses_started_by_letter") == 0) {
        
            char letter = parse_variablesChar(variables);
            
            if(letter != '0') new = businesses_started_by_letter(sgr,letter);

            else printf("\nBad format of function\nPlease Try again -- (businesses_started_by_letter(SGR sgr, char letter))\n\n");
        }

        /* Query 3*/
        else if(strcmp(function_name,"business_info") == 0) {
            char * business_id = malloc(ID_SIZE);
            int bool = parse_variablesString(variables,business_id);

            if(bool != -1) {
                new = businesses_info(sgr,business_id);
            }

            else printf("\nBad format of function\nPlease Try again -- (business_info(SGR sgr,char *business_id))\n\n");

            free(business_id);
            

        }

        /* Query 4*/
        else if(strcmp(function_name,"businesses_reviewed") == 0) {
            char * user_id = malloc(ID_SIZE);
            int bool = parse_variablesString(variables,user_id);
            
            
            if(bool != -1) {
               new = businesses_reviewed(sgr,user_id);
            }

            else printf("\nBad format of function\nPlease Try again -- (businesses_reviewed(SGR sgr,char *user_id))\n\n");

            free(user_id);
            

        }

        /* Query 5*/
        else if(strcmp(function_name,"businesses_with_stars_and_city") == 0) {
            
            char* city = malloc(CITY_SIZE);
            char* number = malloc(MaxInter);
            int bool = parse_variablesCharNumber(variables,city,number);
            if(bool != -1) {
                float stars = atof(number);
                
                new = businesses_with_stars_and_city(sgr,stars,city);
                
                
            }

            else printf("\nBad format of function\nPlease Try again -- (businesses_with_stars_and_city(SGR sgr,float stars,char *city))\n\n");
        
            free(city);
            free(number);
            
        }

        /* Query 6*/
        else if(strcmp(function_name,"top_businesses_by_city") == 0) {
            char* number = malloc(MaxInter);
            int bool = parse_variablesString(variables,number);

            if(bool != -1) {
                int top = atoi(number);
                if(top == 0) printf("Argument is not a number\n Please Try again -- (top_businesses_by_city(SGR sgr, int top))");
                else new = top_businesses_by_city(sgr,top);
                
            }
            free(number);
        }

        /* Query 7*/
        else if(strcmp(function_name,"international_users") == 0) {
            new = international_users(sgr);
            printf("Number of international users: %d",get_Columns(new));
        }


        /* Query 8*/
        else if(strcmp(function_name,"top_businesses_with_category") == 0) {
            char* category = malloc(MAX_CATEGORIES);
            char* number = malloc(MaxInter);
            int bool = parse_variablesCharNumber(variables,category,number);

            if(bool != -1) {
                int top = atoi(number);
                
                new = top_businesses_with_category(sgr,top,category);
            } 

            else printf("\nBad format of function\nPlease Try again -- (top_businesses_with_category(SGR sgr,int top,char *category))\n\n");
            free(number);
            free(category);
        }

        /* Query 9*/
        else if(strcmp(function_name,"reviews_with_word") == 0) {
            char* word = malloc(WORD_SIZE);
            

            int bool = parse_variablesString(variables,word);

            if(bool != -1) {
                new = reviews_with_word(sgr,word);
            } 


            else printf("\nBad format of function\nPlease Try again -- (reviews_with_word(SGR sgr,char *word))\n\n");
        
            free(word);
            
        }

        /*Proj function*/
        else if(strcmp(function_name, "proj") == 0) {
                char **headers;
                headers = malloc(6 * ID_SIZE);
                int size = GetHeaders(variables,headers);
                
                   
               TABLE temp = look_Table(tabela,headers[0]);
               if(temp == NULL) printf("Table does not exist.\n");

                else {

                  int any = -1;                                                                 //to Find out if at least one column exists
                  new = initTable(size-1,get_Columns(temp));

                  for(int i = 1; i<size;i++) {
                      any = projTable(temp,new,headers[i],i-1);                                //Makes column by column
                   }
                   if(any == -1) {                                                             //
                       freeTable(new);
                       new = NULL;
                       printf("No Columns with given headers\n");
                   }
                }
                for(int i = 0; i<size; i++) free(headers[i]);
                free(headers);
            
            
        }

        /*filter function*/
        else if(strcmp(function_name,"filter") == 0) {
            char * table_name = malloc(MaxInter);
            char * column = malloc(MaxInter);
            char * value = malloc(MaxInter);
            char * operator = malloc(MaxInter);
            
            int bool = parse_variablesFilter(variables,table_name,column,value,operator);
            
            
            if(bool != -1) {
                TABLE temp = look_Table(tabela,table_name);
                if(!temp) printf("table %s does not exist in this pogram\n",table_name);
                else { 
                        OPERATOR oper = find_operator(operator);
                        new = filter(temp,column,value,oper);
                }
            }
            else printf("\nBad format of function\nPlease Try again -- filter(x, column_name, value, oper)\n\n");
            free(table_name);
            free(column);
            free(value);
            free(operator);
        }

        /*fromCSV function */
        else if(strcmp("fromCSV",function_name) == 0) {
            char* filepath = malloc(MAXFILEPATH);
            char delim = parse_variablesCSV(variables,filepath);
            
            if(delim != '\0') { 
                new = fromFile(fopen(filepath,"r"),delim); 
            }
            else printf("\nBad format of function\nPlease Try again --  fromCSV(filepath, delim)\n\n");
            free(filepath);
        }

        
        else printf("Function does not exist\n");
            
        
    }
    
    else printf("Missing arguments\n");
    
    return new;
}

/*Tests the format of load_sgr function and if the filepaths really exists */
SGR Sgr_tester(char * variables,SGR sgr) {
    char * users = malloc(MAXFILEPATH);
    char * businesses = malloc(MAXFILEPATH);
    char * reviews = malloc(MAXFILEPATH);
    int bool = 1;
    if(variables) bool = parse_variablesSGR(variables, users,businesses,reviews);

    if(bool == -1) {
        printf("Format not right: load_sgr(char *users, char *businesses, char *reviews)\n");
        
    }
    else {
        if(variables) {
        free_sgr(sgr);
        sgr = load_sgr(users,businesses,reviews);
        }

        
        //If something fails in load_sgr function we ask new filepaths until its done correctly
        while(!sgr) {                                       
            printf("Error sgr not Loaded\n\n");

            printf("Please Write users path:");
            if(scanf("%s",users));
            

            printf("Please Write businesses path:");
            if(scanf("%s",businesses));
            
            printf("Please Write reviews path:");
            if(scanf("%s",reviews));
            
            sgr = load_sgr(users,businesses,reviews);
            
        }
        
        if(fgets(users,100,stdin));
        
        printf("SGR sucessufly loaded\n");
    }
    
    

    free(businesses);
    free(users);
    free(reviews);

    return sgr;
}
