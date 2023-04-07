#include <stdio.h>
#include "TableHASH.h"
#include <readline/readline.h>
#include <string.h>

int main(void) {
    
    TableHash *table = createTH(10);
    TableHash *search = NULL;
    
    clearTableHash(search);
    clearTableHash(table);
    
    return 0;
}
