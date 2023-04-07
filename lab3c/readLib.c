#include "readLib.h"
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <unistd.h>
#include "TableHASH.h"

int importFile(TableHash *table, TableHash *search) {
    int check = 1;
    FILE *file = NULL;
    file = getFile("\nВведите название файла\n", "r", &check);
    char *key = NULL;
    char *info = NULL;
    int i = 0;
    
    if(file) {
        printf("(OK) Файл найден. Начат импорт\n");
        while (check) {
            key = readString(file);
            if(key == NULL) {
                break;
            }
            i++;
            info = readString(file);
            if(info == NULL) {
                break;
            }
            i++;
            addInfoTH(table, key, info);
        }
        if(i%2 == 1) {
            free(key);
        }
    } else {
        printf("(X) Файл не найден\n");
    }
    if(check == -1) {
        return 0;
    }
    
    return 1;
}

char *readString(FILE *file) {
    char *ptr = (char *)malloc(1);
    char buf[81];
    int n, len = 0;
    *ptr = '\0';
    do {
        n = fscanf(file, "%80[^\n]", buf);
        if(n < 0) {
            free(ptr);
            ptr = NULL;
            continue;
        }
        if(n == 0) {
            fscanf(file, "%*c", NULL);
        }
        
        else {
            len += strlen(buf);
            ptr = (char *) realloc(ptr, len + 1);
            strcat(ptr, buf);
        }
    } while(n > 0);
        
    return ptr;
}


    FILE* getFile(const char* invite, char choose[2], int *check) {
        FILE *file = NULL;
        char *dir = NULL;
        dir = getcwd(dir, 1000);
        dir = strtok(dir, "\t");
        char *s = NULL;
        int lendir = (int) strlen(dir);
        s = myreadline(invite);
        if(s) {
            dir = realloc(dir, lendir*sizeof(char)+strlen(s)*sizeof(char) + 2);
            dir[lendir] = '/';
            dir[lendir+1] = '\0';
            dir = strcat(dir, s);
            file = fopen(dir, choose);
            free(s);
        } else {
            *check = -1;
        }
        printf("%s\n", dir);
        free(dir);
        return file;
    }

int getIntUnsignt(char *number) {
    char chars[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    
    if(number != NULL){
        int lenght = (int) strlen(number);
        
        if(number[0] == ' ') {
            return -1;
        }
        
        if(number[0] == '0' && lenght == 1) {
            return 0; 
        }
        
        if(number[0] == '0'){
            return -1;
        }
        
        if(lenght > 9) {
            return -1;
        }
        
        if(lenght == 0) {
            return -1;
        }
        
        int flag = 0;
        
        for(int i = 0; i < lenght; i++) {
            for(int b = 0; b < 10; b++) {
                if(chars[b] == number[i]) {
                    flag++;
                    break;
                }
            }
        }
        
        if(flag == lenght) {
            return atoi(number);
        }
    }
        
    return -1;
}

char *myreadline(const char* invite) {
    {
    char *ptr = (char *)malloc(1);
    char buf[81];
    int n, len = 0;
    *ptr = '\0';
    printf("%s", invite);
    do{
        n = scanf("%80[^\n]", buf);
        if(n < 0){
            free(ptr);
            ptr = NULL;
            continue;
        }
        if(n == 0) {
            scanf("%*c");
        }
        
        else {
            len += strlen(buf);
            ptr = (char *) realloc(ptr, len + 1);
            strcat(ptr, buf);
        }
    } while(n > 0);
    return ptr;
        
    }
}
