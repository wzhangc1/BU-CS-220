#include <stdlib.h>
#include <stdio.h>
#include "llnode.h"

int main(int argc,char **argv) {

	int i;
	node listHead=NULL;

	for(i=1;i<argc;i++) {
		listHead=addToHead(listHead,atoi(argv[i]));
	}

	printf("Args -> head: count=%d ",listSize(listHead));
	printList(listHead);
	freeList(listHead);
	listHead=NULL;

	for(i=1;i<argc;i++) {
		listHead=addToTail(listHead,atoi(argv[i]));
	}

	printf("Args -> tail: count=%d ",listSize(listHead));
	printList(listHead);
	freeList(listHead);
	listHead=NULL;

	for(i=1;i<argc;i++) {
		listHead=addIncreasing(listHead,atoi(argv[i]));
	}

	printf("Args inorder: count=%d ",listSize(listHead));
	printList(listHead);
	freeList(listHead);
	listHead=NULL;

	return 0;
}
