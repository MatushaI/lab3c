#include <stdio.h>
#include <stdlib.h>
#include "TableHASH.h"
#include "readLib.h"
#include <string.h>
#include "dialog.h"

int main(void) {
    
    const char *msgs[] = {"0. Выход", "1. Добавить элемент", "2. Поиск", "3. Удаление", "4. Печать таблицы", "5. Импорт"};
    const int Nmsgs = sizeof(msgs) / sizeof(msgs[0]);
    
    char *errorMessage = "";
    int n = 0;
    char *string = NULL;
    
    do {
        printf("%s\n", errorMessage);
        string = myreadline("Введите количество элементов таблицы\n> ");
        
        if(string == NULL) {
            return 0;
        }
        
        n = getIntUnsignt(string);
        errorMessage = "\n(!) Ошибка ввода. Попробуйте ещё раз\n";
        free(string);
        
    } while (n <= 0);
    
    int (*func[])(TableHash *, TableHash *) = {NULL, addInfo_In, search_In, delete_In, printTable_In, importFile};
    
    TableHash *table = createTH(n);
    TableHash *search = createTH(1);
    
    int rc;
    while((rc = dialog(msgs, Nmsgs, NULL))){
        if(!func[rc](table, search)) 
            break;
    }
    
    clearTableHash(search);
    clearTableHash(table);
    
    printf("\n*** Программа завершена ***\n");
    return 0;
}
