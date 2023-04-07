#ifndef TableHASH_h
#define TableHASH_h

#include <stdio.h>

typedef struct Item {
    char *info;
    unsigned int version;
    struct Item *next;
} Item;

typedef struct KeySpaceHash {
    char* key;
    Item *item;
    struct KeySpaceHash *next;
} KeyspaceHash;

typedef struct TitleHash {
    KeyspaceHash *ks;
} TitleHash;

typedef struct TableHash {
    unsigned int maxSize;
    TitleHash *th;
} TableHash;

int getIndex(char *string, int size);
TableHash *createTH(int size);
int addInfoTH(TableHash *table, char *key, char *info);
int printTH(TableHash *table);
int deleteKeyTH(TableHash *table, char *key);
int deleteOldVersionsTH(TableHash *table, char *key);
TableHash *searchKeyTH(TableHash *table, TableHash *search, char *key);
TableHash *searchKeyVersionTH(TableHash *table, TableHash *search, char *key, int version);
void clearTableHash(TableHash *table);

#endif 
