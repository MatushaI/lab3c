#ifndef readLib_h
#define readLib_h

#include <stdio.h>
#include "TableHASH.h"

FILE* getFile(const char* invite, char choose[2], int *check);
int importFile(TableHash *table, TableHash *search);
char *readString(FILE *file);
int getIntUnsignt(char *number);
char *myreadline(const char* invite);

#endif 
