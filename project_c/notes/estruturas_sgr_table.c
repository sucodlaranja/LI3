//#include <glib.h>
//#include <gmodule.h>


// ver se ghashtable & gtree devolvem pointers

typedef struct{

    /* Hash Table where key = bus_id & value = */
    GHashTable businesses;
    
    /*  */
    GTree bus_names;

    /*  */
    GTree cities;

    /*  */
    GTree categories;
    
}BUS_DATABASE;


typedef struct{

    /*  */
    GHashTable users; // value? gpointer...lista de reviews do user


}USR_DATABASE;


typedef struct{

    // ???
    
}REV_DATABASE;


typedef struct{
    /* bus_database */
    short bus_loaded;
    BUS_DATABASE *bus_database;

    /**/
    short usr_loaded;
    USR_DATABASE *usr_database;

    /**/
    short rev_loaded;
    REV_DATABASE *rev_database;

}*SGR;


