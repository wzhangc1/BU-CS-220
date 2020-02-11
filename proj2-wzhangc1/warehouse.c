#include "order.h"
#include "bench.h"
#include <stdio.h>
#include <stdbool.h> 
int main(int argc, char **argv) {
	if (argc<2) {
		printf("Please invoke as %s <order_file>\n",argv[0]);
		return 1;
	}
	if (!openOrder(argv[1])) {
		printf("Unable to open order file %s\n",argv[1]);
		return 1;
	}
	
	int recentBin[NUMSLOTS];
	
	for (int i = 0; i < NUMSLOTS; i++)
	{
		recentBin[i] = -1;
	}
		
	do {
		int pn=nextPartNumber();
		int bin=pn/10;
		
		bool binInBench = false;
		
		for (int i = 0; i < NUMSLOTS && !binInBench; i++)
		{
			if (binInSlot(i) == bin)
			{
				bool match = false;
				int k;
				
				for (int j = 0; j < NUMSLOTS && !match; j++)
				{
					if (recentBin[j] == i)
					{
						k = j;
						
						match = true;
					}
				}
				
				for (int j = k; j > 0; j--)
				{
					recentBin[j] = recentBin[j-1];
				}
				
				recentBin[0] = i;
				
				binInBench = true;
			}
		}
		
		for (int i = 0; i < NUMSLOTS && !binInBench; i++)
		{
			if (binInSlot(i) == -1)
			{
				fetchBin(bin,i);
				
				for (int j = NUMSLOTS-1; j >= 0; j--)
				{
					recentBin[j] = recentBin[j-1];
				}
				
				recentBin[0] = i;
				
				binInBench = true;
			}
		}
		
		if (!binInBench)
		{
			int i = recentBin[NUMSLOTS-1];
			
			fetchBin(bin, i);
			
			for (int j = NUMSLOTS-1; j >= 0; j--)
			{
				recentBin[j] = recentBin[j-1];
			}
			
			recentBin[0] = i;
		}
	} while(fetchNextPart());
	closeOrder();
	return 0;
}
