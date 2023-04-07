#ifndef dialog_h
#define dialog_h

#include <stdio.h>
#include "TableHASH.h"

int dialog(const char *msgs[], int N, int *ifPagePrev);
int deleteKey_In(TableHash *table);
int deleteOldVersions_In(TableHash *table);

int addInfo_In(TableHash *table, TableHash *searchTable);
int delete_In(TableHash *table, TableHash *search);
int search_In(TableHash *table, TableHash *search);
int printTable_In(TableHash *table, TableHash *search);

void printCat(void);

#endif 
