#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void swap_simple(int *aptr,int *bptr);
void swap_xorbit(int *aptr,int *bptr);
int getBit(int *aptr,int i);
void flipBit(int *aptr,int i);
void swap_xorword(int *aptr,int *bptr);

int main(int argc, char **argv) {
	if (argc<3) {
		printf("Invoke as %s <a> <b>\n",argv[0]);
		return 1;
	}
	int a=atoi(argv[1]);
	int b=atoi(argv[2]);
	printf("A=%d B=%d\n",a,b);
	swap_simple(&a,&b);
	printf("After simple swap, A=%d B=%d\n",a,b);
	swap_xorbit(&a,&b);
	printf("After xor bit swap, A=%d B=%d\n",a,b);
	swap_xorword(&a,&b);
	printf("After xor word swap, A=%d B=%d\n",a,b);
	return 0;
}

void swap_simple(int *aptr,int *bptr) {
	int temp = *aptr;
	*aptr = *bptr;
	*bptr = temp;
}

void swap_xorbit(int *aptr,int *bptr) {
	for (int i = 0; i < sizeof(int) * CHAR_BIT; i++)
	{
		if (getBit(aptr,i) != getBit(bptr,i))
		{
			flipBit(aptr,i);
			flipBit(bptr,i);
		}
	}
}

int getBit(int *aptr,int i)
{
	if ((*aptr & 1 << i) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void flipBit(int *aptr,int i)
{
	(*aptr) ^= 1<<i;
}

void swap_xorword(int *aptr,int *bptr)
{
	*aptr = *aptr ^ *bptr;
	*bptr = *aptr ^ *bptr;
	*aptr = *aptr ^ *bptr;
}
