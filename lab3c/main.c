#include <stdio.h>
#include "TableHASH.h"
#include <readline/readline.h>

int main(void) {
    TableHash *table = createTH(10);
    char *key = readline("Ключ\n > ");
    char *info = readline("Инфо\n > ");
    //char *key1 = readline("Ключ\n > ");
    char *info1 = readline("Инфо\n > ");
    char *info3 = readline("Инфо\n > ");
    //printf("i: %d, %d \n", getIndex(key, 10), getIndex(key1, 10));
    TableHash *search = NULL;
    
    addInfoTH(table, key, info);
    addInfoTH(table, key, info1);
    addInfoTH(table, key, info3);
    //addInfoTH(table, key1, info1);
    printTH(table);
    printf("========\n");
    
    //addInfoTH(table, key, info);
    //addInfoTH(table, key1, info1);
    
    //deleteOldVersionsTH(table, key);
    //deleteKey(table, key3);
    //deleteKey(table, key3);
    printf("========\n");
    printTH(table);
    char *key4 = readline("Поиск\n > ");
    search = searchKeyVersionTH(table, search, key4, 2);
    printTH(search);
    //search = searchKeyTH(table, search, key4);
    printTH(search);
    
    clearTableHash(search);
    clearTableHash(table);
    
    return 0;
}
