#include <stdio.h>
#include <string.h>



// Repeats char "x" n times
void repeat_char(int n, char x) {
    for(int i = 0; i < n; i++) {
        printf("%c",x);
    }
}

// Prints the delimiter "-" based in the number of columns in the query
void delim_Maker(int columns) {
    if(columns == 2) repeat_char(70,'-');
        else if(columns == 1) repeat_char(35,'-');
        else if(columns == 3) repeat_char(104, '-');
        else if(columns == 4) repeat_char(139,'-');
        else if(columns == 5) repeat_char(173, '-');
        printf("\n");
}





//For the filter and the "x[i][j] ones"
void show_one(char * value) {
    
    delim_Maker(1);
    int size = strlen(value);
    int space = (34 - size) / 2;
    printf("|");
    if(size%2 == 0) repeat_char(space-1,' ');
    else repeat_char(space,' ');
    printf("%s", value);
    repeat_char(space,' ');
    printf("|\n");

}


