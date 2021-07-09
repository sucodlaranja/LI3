#include <stdio.h>
#include "../includes/Interpretador.h"

int main()
{
    
    printf("Mensagem inicial.\n");
    SGR sgr = load_sgr("input_files/users_full.csv", "input_files/business_full.csv", "input_files/reviews_1M.csv");
    if(!sgr) sgr = Sgr_tester(NULL,sgr);
    interpretador(sgr);
    free_sgr(sgr);
    printf("Mensagem final.\n");
    return 0;
    
   
} 
