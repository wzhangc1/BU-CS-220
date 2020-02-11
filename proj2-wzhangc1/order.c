#include "order.h"
#include "benchOrder.h"
#include <stdio.h>

FILE * order = NULL;
int nextPart=-1;
int orderOpen=0;

int openOrder(char * orderFile) {
	clearBench();
	order=fopen(orderFile,"r");
	if (order==NULL) {
		perror("Unable to open order file");
		return 0;
	}
	if (1!=fscanf(order,"%d",&nextPart)) {
		printf("Unable to find first part number in order\n");
		return 0;
	}
	orderOpen=1;
	return 1;
}

void closeOrder() {
	fclose(order);
	printEarnings();
	orderOpen=0;
}

int nextPartNumber() { return nextPart; }

int fetchNextPart() {
	getWidget(nextPart);
	if (1!=fscanf(order,"%d",&nextPart)) return 0; // No more parts available
	return 1;
}

int isOrderOpen() {
	return orderOpen;
}

#ifdef BESTPACK
int fnp() {
	if (1!=fscanf(order,"%d",&nextPart)) return 0;
	return 1;
}
#endif