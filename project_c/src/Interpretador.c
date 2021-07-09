#include "../includes/Interpretador.h"




void interpretador(SGR sgr) {
   
    char comando[200];         //Principal buffer that will receibe all the commands for comparation
    char variable_name[20];    //Variable name 
    char delim[10];               //to fix
    char function_name[100];   //string that will contain the function that the user asked
    char filepath[20];         //the filepath
    int posX;
    int posY;
    GList* tabela = NULL;

    while (1)
    {
        printf("Escolha um comando:");
        
        char* empty = fgets(comando,200,stdin);
        
        
        if(empty != NULL) {
            
            if(sscanf(comando,"show(%[^)])",variable_name) == 1) {
                TABLE temp = look_Table(tabela,variable_name);
                if(temp != NULL) show(temp);
                else printf("Variable not in table\n");
            }

            
            else if(sscanf(comando,"%s = %[^)]",variable_name,function_name) == 2) {
               
                
                TABLE new = recognize_function(function_name,tabela,sgr);
                if(new != NULL) {
                    setVar_name(new,variable_name);
                    TABLE temp = look_Table(tabela,variable_name);                                                      //analizes if the variable already exists so we can overwrite it
                    if(temp != NULL) {
                        tabela = g_list_remove(tabela,temp);
                        freeTable(temp);
                    }
                    tabela = g_list_prepend(tabela,new);
                    
                    printf("\nVariable added with sucess\n");
                }
                else printf("\nVariable was not added\n");
                
            }


            
            else if(sscanf(comando,"toCSV(%[^,],%[^,],%[^)])",variable_name,delim,filepath) == 3) {
                if(strlen(delim) > 1) printf("Delim is not a char\n");                                                 //in case someone uses a string instead
                else {
                    TABLE temp = look_Table(tabela,variable_name);
                    if(!temp) printf("TABLE %s does not exist in this pogram\n",variable_name);
                    else {
                        toFile(fopen(filepath,"w"),temp,delim[0],'\n');
                        printf("File %s successfully created\n",filepath);
                    }
                }
                
            }  

            //Loads sgr with new files
            else if(sscanf(comando,"load_sgr(%[^)]",function_name) == 1) {
                sgr = Sgr_tester(function_name,sgr);
            }

            
            else if(sscanf(comando,"%[^[][%d][%d]",variable_name,&posX,&posY) == 3) {
                printf("variable name: %s\nX Position : %d\nY Position : %d\n",variable_name,posX,posY);
                TABLE temp = look_Table(tabela,variable_name);                                                         //Finds the table
                if(temp != NULL && posX < get_Lines(temp) && posY+1 < get_Columns(temp)-1) {
                    show_one(getValue(temp,posX,0));    
                    show_one(getValue(temp,posX,posY+1));                                                              //+1 because we put the index in posy = 0
                    delim_Maker(1);
                }
                else printf("Variable does not exist\n");
            }

            
        

            else if(strcmp(comando,"quit\n") == 0) {
                printf("Program ended\n");
                freeGList(tabela);
                g_list_free(tabela);
                break;
            }

            

            //Displays all commands and possible querries
            else if(strcmp(comando,"help\n") == 0) help(tabela);

            else if(strcmp(comando,"clear\n") == 0) printf("\e[1;1H\e[2J");
            
            else {
                
                printf("Command does not exist\n");
        
            }
        
        }
    }
    
}
    
