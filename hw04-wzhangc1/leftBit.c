#include <stdio.h>
#include <stdlib.h>

int leftBit(int x);
void printBin(int n);

int main(int argc, char **argv) {
	if (argc<2) {
		printf("Invoke as %s <n> \n\twhere <n> is an integer\n",argv[0]);
		return 1;
	}
	int x=atoi(argv[1]);
	printf("X= "); 
	printBin(x); 
	printf("\n");
	printf("The leftmost bit of %d is at position %d\n",x,leftBit(x));
	return 0;
}

int leftBit(int x) {
	if (x==0) return -1;
	int w=32; // Number of bits that might contain leftmost 1
	int n=0; // Rightmost bit that might contain leftmost 1
	while(w>1) { //Narrow down to a single bit
		int hw=w/2; // Look at half the range of bits
		int mask=((1<<hw)-1)<<(n+hw); // mask is nw ones in the left half of the range
		printf("M= "); printBin(mask); printf(" hw=%d n=%d\n",hw,n);
		if (x&mask) n=n+hw; // If the left half of the range has a one bit, start at the left half
		w=hw; // You have ruled out either the left half of the range or the right half of the range
	}
	return n;
}

void printBin(int n) {
	int i;
	for(i=31;i>=0;i--) {
		printf("%c",(n&1<<i)?'1':'0');
		if (0==i%4) printf(" ");
	}
}
