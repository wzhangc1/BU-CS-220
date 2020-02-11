#ifndef WAYS_H
#define WAYS_H
#include <stdbool.h>

typedef struct ways_data *waysptr;

waysptr ways_create(int n);
void way_setValid(waysptr ways,int way);
bool way_isValid(waysptr ways,int way);
void way_setUsed(waysptr ways,int way);
int way_firstUsed(waysptr ways,int start, int end);
void way_setDirty(waysptr ways,int way);
bool way_isDirty(waysptr ways,int way);
void report_waystats(waysptr ways);
void ways_free(waysptr ways);
#endif
