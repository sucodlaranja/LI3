
/* Print business file! */
void print_bus(char *id, char *name, char *city, char *state, char **category, int nCat)
{
    printf("%s;%s;%s;%s;", id, name, city, state);

    int k;
    for(k = 0; k < nCat; ++k){
        printf("%s", category[k]);
        if(k+1 < nCat) printf(",");
    }

    printf("\n");   
}

/* Print string from gpointer (from list). */
void print_string(gpointer p1, gpointer p2){
    printf("%s\n", (char*)p1);
}

/* Print binary tree that has value = GList */
gboolean printListBusNames(gpointer p1, gpointer p2, gpointer p3){
    // key = letter, value = list of business names (strings)
    int letter = GPOINTER_TO_INT(p1);
    GList *ll = p2;

    if(letter == 'U'){
        g_list_foreach(ll, print_string, NULL);
        printf("'%c' => %d businesse(s)\n", letter, g_list_length(ll));
    }

    return FALSE;
}

/* Print list from GTree. */
gboolean printListString(gpointer p1, gpointer p2, gpointer p3){
    // key = city name (strings), value = list of VALUE_ID_NAME_RATING
    char *city_name = p1;
    GList *ll = p2;
    //printf("%d -> %s\n", g_list_length(ll), city_name);
    if(strcmp(city_name, "Atlanta") == 0) g_list_foreach(ll, print_string, NULL);
    return FALSE;
}

