#ifndef INTERPRETADOR_H
#define INTERPRETADOR_H

#include "Table.h"
#include "sgr.h"
#include "Queries.h"
#include "glibWarningAvoid.h"

/**
 * @brief 
 * 
 * @param sgr Initial SGR structure (empty by default). 
 * @return 0 if OK
 */
void interpretador(SGR sgr);


//aux
void help(GList *tabela);
TABLE recognize_function(char* function,GList* tabela, SGR sgr);
SGR Sgr_tester(char * variables,SGR sgr);

#endif