#include "btree.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void stringLower(char * word);


int main(int argc, char **argv) {

	node root=NULL;
	for(int i=1;i<argc;i++) {
		FILE *f;
		f=fopen(argv[i],"r");
		char buffer[4096];
		char delims[]=" ,.()\n;?\"";

		while(NULL!=fgets(buffer,sizeof(buffer),f)) {
			char * word = strtok(buffer,delims);
			while(word!=NULL) {
				stringLower(word);
				if (root==NULL) root=newNode(word);
				else addTree(root,word);
				word=strtok(NULL,delims);
			}
		}
		fclose(f);
	}
	printTree(root);
	freeTree(root);
	return 0;
}

void stringLower(char *word) {
	for ( ; *word; ++word) *word = tolower(*word);
}