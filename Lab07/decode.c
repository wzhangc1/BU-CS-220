#include <stdio.h>
#include <stdlib.h>

char decode(char in, char *perm);

int main(int argc,char **argv)
{
	char c;
	int cn=0;
	int n=atoi(argv[1]);
	
	while (EOF != (c = getchar()))
	{
		for(int i = 0; i < cn; i++)
		{
			getchar();
		}
		
		putchar(c);
		
		cn++;
		
		if (cn == n)
		{
			cn = 0;
		}
	}
	
	return 0 ;
}
