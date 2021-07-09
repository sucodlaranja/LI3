#ifndef TABLE_H
#define TABLE_H
#include "glibWarningAvoid.h"
#include <stdio.h>

typedef enum {LT,EQ,GT} OPERATOR;

#define BLOCK_SIZE_TABLE 1048576 * 0.5 // 512 KBytes

typedef struct table_struct *TABLE;

int get_Columns(TABLE table);
int get_Lines(TABLE table);
char * getValue(TABLE table,int linha,int coluna);
char* getVar_name(TABLE table);
char*** get_tbl(TABLE table);
void setVar_name(TABLE table,char* name);


void show(TABLE table);
TABLE filter(TABLE table,char * column_name, char* value, OPERATOR oper);
TABLE look_Table(GList *tabela, char* var_name);
int projTable(TABLE origin,TABLE new,char* column,int linha);

int compareColumn(TABLE table, char* column);
void makeLine(TABLE table,int linha);
void maketable(TABLE table,int inicio,int fim);
//...

TABLE initTable(int L, int C);
void freeTable(TABLE t);
void freeGList(GList* Tabela);


//Table_aux
void repeat_char(int n, char x);
void delim_Maker(int columns);
void show_one(char * value);

// Given an file transform it into a Table
TABLE fromFile(FILE* filepointer, char sep);

// Writes a table back to a file
int toFile(FILE* filepointer, TABLE table, char sep, char newline);

#endif