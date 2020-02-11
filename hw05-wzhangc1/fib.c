#include <stdio.h>
#include <stdlib.h>

int fib(int n);

int main(int argc, char **argv) {

	if (argc<2) {
		printf("Invoke as %s <n>\n",argv[0]);
		return 1;
	}
	
	int n=atoi(argv[1]);
	int result=fib(n);
	printf("fib(%d)=%d\n",n,result);
	return 0;
}

int fib(int n) {
	int answer=0;
	if (n==0) {
		answer=0;
	} else if (n==1) {
		answer=1;
	} else {
		int n2=fib(n-2);
		int n1=fib(n-1);
		answer=n1+n2;
	}
	return answer;
}	
