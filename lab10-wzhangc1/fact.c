#include <stdio.h>
#include "stack.h"

int factorial(int n);

int main(int argc, char *argv[]) {
   printf("Factorial 4=%d\n",factorial(4));
	return 0;
}

int factorial(int n) {
	int answer=0xbadddadd;
	if (n<2) answer=n;
	else answer=(factorial(n-1) * n);
	printf("\n================= Stack when n==%d\n",n);
	printStackInfo();
	return answer;
}
