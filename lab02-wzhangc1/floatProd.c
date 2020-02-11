#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "floatInt.h"

float floatProd(float a,float b);
void floatPrint(char * pref,float a);

int main(int argc,char **argv) {
	if (argc!=3) {
		printf("Please invoke as %s <a> <b>\n",argv[0]);
		return 1;
	}
	float a=atof(argv[1]);
	floatPrint("a             :",a);
	float b=atof(argv[2]);
	floatPrint("b             :",b);
	floatPrint("floatProd(a,b):",floatProd(a,b));
	floatPrint("a x b         :",a*b);
	return 0;
}

float floatProd(float a,float b) {
	/* Your code to compute the product of a and b without doing a*b goes here */
	
	int sign;
	int exp;
	int frac;
	long tempFrac;
	
	//Special cases-----------------------------------------------------
	if (isnanf(a) || isnanf(b))
	{
		return NAN;
	}
	
	if (isinff(a) || isinff(b))
	{
		if (a == 0 || b == 0)
		{
			return NAN;
		}
		else
		{
			if (float2Sign(a) != float2Sign(b))
			{
				return -INFINITY;
			}
			else
			{
				return INFINITY;
			}
		}
	}
	
	//------------------------------------------------------------------
	if (float2Sign(a) != float2Sign(b))
	{
		sign = 1;
	}
	else
	{
		sign = 0;
	}
	
	exp = float2Exp(a) + float2Exp(b);
	
	tempFrac = (long)float2Frac(a) * float2Frac(b);
	
	tempFrac = (tempFrac >> 23);
	
	if (tempFrac > (1<<24))
	{
		tempFrac = (tempFrac >> 1);
		
		exp++;
	}
	
	frac = (int)tempFrac;
	//printf("----------------%d\n", frac);
	if (exp > 126)
	{
		exp = 127;
		frac = 0;
	}
	else if (exp < -126)
	{
		while (frac != 0 && exp < -126)
		{
			frac = (frac >> 1);
			exp++;
		}
	}
	return makeFloat(sign, exp, frac);
}

void floatPrint(char * pref,float a) {
	int norm=isNormal(a);
	int frac=float2Frac(a);
	int exp=float2Exp(a);
	if (isNormal(a)) {
		frac &= ~(1<<23); // Turn of 1. for print
	} else exp=-126;
	printf("%s %g = -1^%d x %d.%06x x 2^%d\n",pref,a,
		float2Sign(a),norm,frac,exp);
}
