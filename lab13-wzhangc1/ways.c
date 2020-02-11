#include "ways.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct way_struct {
		bool valid;
		bool dirty;
		int lastUsed;
};

static int time=1;

struct ways_data {
	int n;
	struct way_struct *ways;
	int misses;
	int hits;
	int writes;
};

waysptr ways_create(int n) {
	waysptr ways=(waysptr) malloc(sizeof(struct ways_data));
	assert(ways);
	ways->n=n;
	ways->ways=(struct way_struct *)malloc(n*sizeof(struct way_struct));
	ways->misses=0;
	ways->hits=0;
	ways->writes=0;
	int i;
	for(i=0;i<n;i++) {
		ways->ways[i].valid=false;
		ways->ways[i].dirty=false;
		ways->ways[i].lastUsed=0;
	}
	return ways;
}

void way_setValid(waysptr ways,int way) {
	assert(ways);
	assert(way<ways->n);
	ways->misses++;
	if (ways->ways[way].valid && ways->ways[way].dirty) ways->writes++;
	ways->ways[way].valid=true;
	ways->ways[way].dirty=false;
}

bool way_isValid(waysptr ways,int way) {
	assert(ways);
	assert(way<ways->n);
	return ways->ways[way].valid;
}

void way_setUsed(waysptr ways,int way) {
	assert(ways);
	assert(way<ways->n);
	assert(ways->ways[way].valid);
	ways->hits++;
	ways->ways[way].lastUsed=time;
	time++;
}

int way_firstUsed(waysptr ways,int start, int end) {
	assert(ways);
	assert(start>=0);
	assert(end>start);
	assert(end<=ways->n);
	int i; int first=start; int oldest;
	oldest=time+1;
	for(i=start;i<end;i++) {
		if (!ways->ways[i].valid) return i;
		if (oldest>ways->ways[i].lastUsed) {
			first=i;
			oldest=ways->ways[i].lastUsed;
		}
	}
	return first;
}

void way_setDirty(waysptr ways,int way) {
	assert(ways);
	assert(way<ways->n);
	ways->ways[way].dirty=true;
}

bool way_isDirty(waysptr ways,int way) {
	assert(ways);
	assert(way<ways->n);
	return ways->ways[way].dirty;
}

void report_waystats(waysptr ways) {
	assert(ways);
	printf("There were %d cache misses (%d writebacks) and %d hits\n",
		ways->misses,ways->writes,ways->hits);
	printf("For a cache hit rate of %6.2f%%\n",(float)(ways->hits-ways->misses)*100.0 / ways->hits);
}

void ways_free(waysptr ways) {
	assert(ways);
	free(ways->ways);
	free(ways);
}
