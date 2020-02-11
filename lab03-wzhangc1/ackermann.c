#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

long ack(long m,long n);

int main(int argc, char **argv) {
	if (argc<3) {
		printf("Invoke as %s <m> <n> - where <m> and <n> are positive integers.\n",
			argv[0]);
		return 1;
	}

	long m=atol(argv[1]);
	long n=atol(argv[2]);

	printf("ackermann(%ld,%ld)=%ld\n",m,n,ack(m,n));
	return 0;
}

long ack(long m,long n) {
	assert(m>=0);
	assert(n>=0);
	if (m==0) return n+1;
	if (n==0) return ack(m-1,1);
	return ack(m-1,ack(m,n-1));
}