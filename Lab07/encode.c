#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char randChar();

int main(int argc, char **argv) {
	time_t t; srand((unsigned) time(&t));
	int n=atoi(argv[1]);
	int cn=1; char c;
	while(EOF !=(c=getchar())) {
		int i;
		for(i=0;i<cn;i++) putchar(i==0?c:randChar());
		cn++;
		if (cn>n) cn=1;
	}
	return 0;
}

char randChar() {
	char ltrs[]="abcdefghijklmnopqrstuvwxyz";
	return ltrs[rand()%strlen(ltrs)];
}