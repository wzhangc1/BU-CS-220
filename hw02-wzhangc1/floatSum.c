#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "floatInt.h"
#define max(a, b) ((a > b) ? a : b)

float floatSum(float a,float b);
void floatPrint(char * pref,float a);

int main(int argc,char **argv) {
	if (argc!=3) {
		printf("Please invoke as %s <a> <b>\n",argv[0]);
		printf("   where <a> and <b> are floating point numbers.\n");
		return 1;
	}
	float a=atof(argv[1]);
	floatPrint("a             :",a);
	float b=atof(argv[2]);
	floatPrint("b             :",b);
	floatPrint("floatSum(a,b) :",floatSum(a,b));
	floatPrint("a + b         :",a+b);
	return 0;
}

float floatSum(float a,float b) {
	int exp;
	int frac;
	int sign;
	
	//Special cases-----------------------------------------------------
	if (isnanf(a) || isnanf(b))
	{
		return NAN;
	}
	
	if (isinff(a) || isinff(b))
	{
		if (isinff(a) && isinff(b) && float2Sign(a) != float2Sign(b))
		{
			return 0;
		}
		
		if (isinff(a))
		{
			if (float2Sign(a))
			{
				return -INFINITY;
			}
			else
			{
				return INFINITY;
			}
		}
		else
		{
			if (float2Sign(b))
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
	exp = max(float2Exp(a), float2Exp(b));
	
	frac = ((int)(pow(-1, float2Sign(a)) * float2Frac(a)) >> (exp - 
		float2Exp(a))) + ((int)(pow(-1, float2Sign(b)) * float2Frac(b)) 
		>> (exp - float2Exp(b)));
	
	if (frac < 0)
	{
		sign = 1;
		frac = -frac;
	}
	else
	{
		sign = 0;
	}
	
	if (frac >= (1<<24))
	{
		frac = (frac >> 1);
		exp++;
	}
	else if (frac < (1<<23))
	{
		frac = (frac << 1);
		exp--;
	}
	
	if (exp < -126)
	{
		frac = (frac >> 1);
		exp++;
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
