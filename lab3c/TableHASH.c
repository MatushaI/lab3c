#include "TableHASH.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int getIndex(char *string, int maxSize) {
    if(string == NULL) {
        return -1;
    }
    long hash = 0;
    int index;
    double intpart = 1;
    for (int i = 0; i < strlen(string); i++) {
        hash = (hash<<5) - hash + string[i];
    }
    double a = 0.61803398;
    index = round((maxSize - 1)*modf(hash*a, &intpart));
    return index;
}

TableHash *createTH(int size) {
    if(size <= 0) {
        return NULL;
    }
    TableHash *table = calloc(1, sizeof(TableHash));
    table->maxSize = size;
    table->th = calloc(size, sizeof(TableHash));
    for (int i = 0; i < size; i++) {
        table->th[i].ks = NULL;
    }
    return table;
}

int addInfoTH(TableHash *table, char *key, char *info) {
    int index = getIndex(key, table->maxSize);
    
    if(!info || !key ) {
        if(key) {
            free(info);
        } else if(info) {
            free(key);
        }
        return 1;
    }
    
    KeyspaceHash *ks = table->th[index].ks;
    while (ks) {
        if(!strcmp(key, ks->key)) {
            break;
        }
        ks = ks->next;
    }
    
    Item *item = calloc(1, sizeof(Item));
    if(!item) {
        free(ks);
        return 1;
    }
    
    if(!ks) {
        
        ks = calloc(1, sizeof(KeyspaceHash));
        if(!ks) {
            return 1;
        }
        ks->item = NULL;
        ks->key = key;
        ks->next = NULL;
        ks->next = table->th[index].ks;
        table->th[index].ks = ks;
    }
    
    item->info = info;
    
    if(ks->item == NULL) {
        item->version = 1;
        item->next = NULL;
        ks->item = item;
    } else {
        item->version = ks->item->version + 1;
        item->next = ks->item;
        ks->item = item;
    }
    
    if(table->th[index].ks == NULL) {
        printf("NULL\n");
    }
    
    return 0;
}

int printTH(TableHash *table) {
    if(!table) {
        return 1;
    }
    KeyspaceHash *ks = NULL;
    Item *item = NULL;
    for (int i = 0; i < table->maxSize; i++) {
        ks = table->th[i].ks;
        while (ks) {
            printf("Ключ: %s\n", ks->key);
            item = ks->item;
            while (item) {
                printf("Версия: %d, информация: %s\n", item->version, item->info);
                item = item->next;
            }
            ks = ks->next;
        }
    }
    return 0;
}

int deleteKeyTH(TableHash *table, char *key) {
    int index = getIndex(key, table->maxSize);
    Item *item = NULL;
    Item *del = NULL;
    KeyspaceHash* prev = NULL;
    KeyspaceHash* ks = table->th[index].ks;
    
    while (ks) {
        
        if(!strcmp(key, ks->key)) {
            break;
        }
        prev = ks;
        ks = ks->next;
    }
    
    if(!ks) {
        free(key);
        return 1;
    }
    
    if(!prev) {
        table->th[index].ks = ks->next;
    } else {
        prev->next = ks->next;
    }
    
    item = ks->item;
    
    while (item) {
        del = item;
        free(item->info);
        item = item->next;
        free(del);
    }
    free(key);
    free(ks);
    return 0;
}

int deleteOldVersionsTH(TableHash *table, char *key) {
    int index = getIndex(key, table->maxSize);
    Item *item = NULL;
    Item *del = NULL;
    KeyspaceHash* ks = table->th[index].ks;
    
    while (ks) {
        if(!strcmp(ks->key, key)) {
            break;
        }
        ks = ks->next;
    }
    
    if(!ks) {
        free(key);
        return 1;
    }
    
    item = ks->item;
    if(!item->next) {
        return 1;
    } else {
        item = item->next;
        ks->item->next = NULL;
        while (item) {
            del = item;
            free(item->info);
            item = item->next;
            free(del);
        }
    }
    free(key);
    return 0;
}

TableHash *searchKeyTH(TableHash *table, TableHash *search, char *key) {
    
    if(search) {
        clearTableHash(search);
    }
    
    int index = getIndex(key, table->maxSize);
    KeyspaceHash *ks = table->th[index].ks;
    Item *item = NULL;
    Item *newItem = NULL;
    while (ks) {
        if(!strcmp(key, ks->key)) {
            break;
        }
        ks = ks->next;
    }
    
    if(!ks) {
        free(key);
        search = NULL;
        return NULL;
    }
    search = createTH(1);
    KeyspaceHash *ksNew = calloc(1, sizeof(KeyspaceHash));
    ksNew->item = NULL;
    ksNew->key = key;
    ksNew->next = NULL;
    search->th[0].ks = ksNew;
    item = ks->item;
    
    while (item) {
        newItem = calloc(1, sizeof(Item));
        newItem->version = item->version;
        newItem->info = strdup(item->info);
        newItem->next = NULL;
        if(!ksNew->item) {
            ksNew->item = newItem;
        } else {
            newItem->next = ksNew->item;
            ksNew->item = newItem;
        }
        item = item->next;
    }
    
    if(!item) {
        free(newItem);
        free(ksNew);
    }
    
    return search;
}

TableHash *searchKeyVersionTH(TableHash *table, TableHash *search, char *key, int version) {
    
    if(search) {
        clearTableHash(search);
    }
    
    int index = getIndex(key, table->maxSize);
    KeyspaceHash *ks = table->th[index].ks;
    Item *item = NULL;
    Item *newItem = NULL;
    while (ks) {
        if(!strcmp(key, ks->key)) {
            break;
        }
        ks = ks->next;
    }
    
    if(!ks) {
        free(key);
        search = NULL;
        return NULL;
    }
    
    search = createTH(1);
    KeyspaceHash *ksNew = calloc(1, sizeof(KeyspaceHash));
    ksNew->item = NULL;
    ksNew->key = key;
    ksNew->next = NULL;
    search->th[0].ks = ksNew;
    item = ks->item;
    
    while (item) {
        if(item->version == version) {
            newItem = calloc(1, sizeof(Item));
            newItem->version = item->version;
            newItem->info = strdup(item->info);
            newItem->next = NULL;
            ksNew->item = newItem;
            break;
        }
        item = item->next;
    }
    
    if(!item) {
        free(newItem);
        free(ksNew);
    }
    
    return search;
}

void clearTableHash(TableHash *table) {
    KeyspaceHash *ks = NULL;
    KeyspaceHash *prev = NULL;
    Item *item = NULL;
    Item *del = NULL;
    
    if(table) {
        for (int i = 0; i < table->maxSize; i++) {
            ks = table->th[i].ks;
            while (ks) {
                prev = ks;
                item = ks->item;
                while (item) {
                    del = item;
                    free(item->info);
                    item = item->next;
                    free(del);
                }
                free(ks->key);
                ks = ks->next;
                free(ks);
            }
        }
        free(table->th);
        free(table);
    }
    
}
