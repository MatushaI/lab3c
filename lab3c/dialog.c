#include "dialog.h"
#include "readLib.h"
#include <stdlib.h>
#include <stdio.h>
#include "TableHASH.h"
#include <time.h>
#include <unistd.h>

int dialog(const char *msgs[], int N, int *ifPagePrev) {
    
    char *errorMessage = "";
    int n = 0;
    char *string = NULL;
    
    do {
        printf("%s\n", errorMessage);
        
        for (int i = 0; i < N; i++) {
            printf("%s\n", msgs[i]);
        }
        
        string = myreadline("> ");
        
        if(string == NULL) {
            if(ifPagePrev != NULL) {
                *ifPagePrev = -1;
            }
            return 0;
        }
        
        n = getIntUnsignt(string);
        errorMessage = "\n(!) Пункт меню не найден. Попробуйте ещё раз\n";
        free(string);
        
    } while (n < 0 || n >= N);
    
    return n;
}

int addInfo_In(TableHash *table, TableHash *searchTable) {
    const char *msgs[] = {"0. В основное меню", "1. Вставить элемент"};
    char *errorMessage = "";
    int checkZero = 0;
    char *key = NULL;
    char *info = NULL;
    int n = dialog(msgs, sizeof(msgs) / sizeof(msgs[0]), &checkZero);
    
    if(checkZero == -1) {
        return 0;
    }
    
    if(n == 0) {
        return 1;
    }
    
    printf("%s\n", errorMessage);
    key = myreadline("Введите ключ вставляемого элемента\n> ");
    if(!key) {
        return 0;
    }
    
    info = myreadline("Введите информацию\n> ");
    if(info == NULL) {
        free(key);
        return 0;
    }
    
    if(!addInfoTH(table, key, info)) {
        printf("\n(OK) Успешно\n");
    } else {
        printf("\n(X) Ошибка добавления элемента\n");
    }
    
    return 1;
}

int deleteKey_In(TableHash *table) {
    
    char *key = NULL;
    
    key = myreadline("Введите ключ удаляемого элемента\n> ");
    if(key == NULL) {
        return 0;
    }
    
    if(deleteKeyTH(table, key)){
        printf("\n(X) Ключ не найден\n");
    } else {
        printf("\n(OK) Успешно\n");
    }
    
    return 1;
}

int deleteOldVersions_In(TableHash *table) {
    char *key = NULL;
    
    key = myreadline("Введите ключ удаляемого элемента\n> ");
    if(key == NULL) {
        return 0;
    }
    
    if(deleteOldVersionsTH(table, key)){
        printf("\n(X) Ключ не найден\n");
    } else {
        printf("\n(OK) Успешно\n");
    }
    
    return 1;
}

int delete_In(TableHash *table, TableHash *search) {
    int (*func[])(TableHash *) = {NULL, deleteKey_In, deleteOldVersions_In};
    const char *msgs[] = {"0. В основное меню", "1. Удалить по значению ключа", "2. Удалить последние версии"};
    int checkZero = 0;
    int n = dialog(msgs, sizeof(msgs) / sizeof(msgs[0]), &checkZero);
    
    if(checkZero == -1) {
        return 0;
    }
    
    if(n == 0) {
        return 1;
    }
    
    func[n](table);
    
    return 1;
    
}

int printTable_In(TableHash *table, TableHash *search) {
    printTH(table);
    return 1;
}

TableHash* printTableSearch_In(TableHash *table, TableHash *search) {
    if(search != NULL) {
        printTH(search);
    } else {
        printf("\n(X) Результат поиска пуст\n");
    }
    
    return NULL;
}

TableHash* searchKey_In(TableHash *table, TableHash *search) {
    char *key = NULL;
        
    key = myreadline("Введите ключ искомого элемента\n> ");
    if(key == NULL) {
        return NULL;
    }
        
    if(!searchKeyTH(table, search, key)) {
        printf("\n(X) Ключ не найден\n");
    } else {
        printf("Результат поиска:\n");
        printTH(search);
    }
    
    return search;
}

TableHash *searchKeyVersion_In(TableHash *table, TableHash *search) {
    char *errorMessage = "";
    char *string = NULL;
    char *key = NULL;
    int version = 0;
    
    key = myreadline("Введите ключ искомого элемента\n> ");
    if(key == NULL) {
        return 0;
    }
    
    do {
        printf("%s\n", errorMessage);
        string = myreadline("Введите версию искомого элемента\n> ");
        if(string == NULL) {
            free(key);
            return NULL;
        }
        version = getIntUnsignt(string);
        errorMessage = "\n(!) Неверный формат версии. Попробуйте ещё раз\n";
        free(string);
    } while(version == -1);
    
    
    if(!searchKeyVersionTH(table, search, key, version)){
        printf("\n(X) Ключ или версия не найдены\n");
    } else {
        printf("Результат поиска:\n\n");
        printTH(search);
    }
    
    return search;
}

int search_In(TableHash *table, TableHash *search) {
    TableHash* (*func[])(TableHash *, TableHash *) = {NULL, searchKey_In, searchKeyVersion_In, printTableSearch_In};
    const char *msgs[] = {"0. В основное меню", "1. Поиск по значению ключа", "2. Поиск по значению ключа и версии", "3. Результат поиска"};
    int checkZero = 0;
    int n = dialog(msgs, sizeof(msgs) / sizeof(msgs[0]), &checkZero);
    
    if(checkZero == -1) {
        return 0;
    }
    
    if(n == 0) {
        return 1;
    }
    
    search = func[n](table, search);
    
    return 1;
}

void printCat(void) {
    
    const char *cat[] = {"  ╭━━━╮  ╱╲   ╱╲", "  ┃╭━━╯  ▏▔▔▔▔▔▕ ", "  ┃╰━━━━━▏╭▆┊╭▆▕ ", "  ╰┫╯╯╯╯╯▏╰╯▼╰╯▕ ", "   ┃╯╯╯╯╯▏╰━┻━╯▕", "   ╰┓┏┳━┓┏┳┳━━━╯","    ┃┃┃ ┃┃┃┃","    ┗┻┛ ┗┛┗┛\n\n"};
    
    for (int i = 0; i < 45; i++) {
        system("clear");
        for (int b = 0; b < 8; b++) {
            for (int c = 0; c < i; c++) {
                printf(" ");
            }
            printf("%s\n", cat[b]);
            
        }
        printf("  Бегу сдавать лабу после ночи с valgrind\n");
        usleep(50000);
    }
    system("clear");
}
