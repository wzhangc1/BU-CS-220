#include "packInt.h"
#include <assert.h>

void putInt(int value,long store[])
{
	store[0]++;
	int count = 1+(store[0] & ((1 << 22)-1));
	int memUsed = ((count)*11)/64;
	int shiftBeg = (11*(count))%64;
	int shiftEnd = (11*(count+1))%64;
	
	long temp = value;
	
	long mask = 1;
	mask = (((mask << 11)-1) << shiftBeg);
	
	store[memUsed] = (store[memUsed] | ((temp << shiftBeg) & mask));
	
	if (shiftEnd < 11)
	{
		long mask2 = 1;
		mask2 = (((mask2 << 11)-1) >> (11-shiftEnd));
		
		store[memUsed+1] = (store[memUsed+1]) | ((temp >> (11-shiftEnd)) & mask2);
	}
}

int storeUsed(long store[]) {
	int count = (store[0] & ((1 << 22)-1));
	return 1+((count+2)*11-1)/64;
}

int getInt(long store[]) {
	static int i = 2;
	
	int memUsed = (i*11)/64;
	int shiftBeg = (11*(i))%64;
	int shiftEnd = (11*(i+1))%64;
	
	long temp;
	
	if (shiftEnd < 11 && shiftEnd != 0)
	{
		temp = 0;
		
		temp = ((store[memUsed+1] & ((1 << shiftEnd)-1)) << (11-shiftEnd));
		long mask = 1;
		mask = (((mask << 11)-1) >> shiftEnd);
		temp = temp | ((store[memUsed] >> shiftBeg)& mask);
		
		if (((store[memUsed+1] >> (shiftEnd-1)) & 1) == 1)
		{
			temp = -(((~temp)+1) & ((1<<11)-1));
		}
	}
	else
	{
		if (((store[memUsed] >> (shiftBeg+10)) & 1) == 1)
		{
			temp = -((~(store[memUsed] >> shiftBeg)+1) & ((1 << 11)-1));
		}
		else
		{
			temp = ((store[memUsed] >> shiftBeg) & ((1 << 11)-1));
		}
	}
	
	i++;
	
	return temp;
}

