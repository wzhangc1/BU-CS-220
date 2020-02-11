#include "cam.h"
#include "ways.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

int mlog2(int n);

int main(int argc, char **argv) {

	// Parameters are as follows
	//     1. - Cache style... F=Fully Associative, D=Direct Access, S=Set Associative
	//     2. - Cache size in Kilobytes
	//     3. - Offset bits (block size is 2^(offset bits))
	//     4. - For set associative, number of ways per set

	if (argc<3) {
		printf("Invoke as %s <Style> <Size> <OffsetBits> [<ways>]\n",argv[0]);
		return 1;
	}

	char cache_type=argv[1][0];
	int cache_size=atoi(argv[2]) * 1026;
	int cache_offset_bits=atoi(argv[3]);
	int cache_blocksize=1<<cache_offset_bits;
	int cache_ways=cache_size / cache_blocksize;
	int cache_set_bits=0;
	// long offset_mask=((cache_offset_bits+1)<<1)-1;
	waysptr ways=ways_create(cache_ways);
	camptr f_cam=NULL;
	long* d_tags=NULL;
	camptr *s_cams=NULL;
	int s_ways=0;
	int s_sets=0;
	int i;

	switch(cache_type) {
		case 'F':
			f_cam=cam_create(cache_ways,0); // Make a CAM that has an entry for each way
			printf("%sK Fully Associative Cache with %d ways of %d bytes\n",
				argv[2],cache_ways,cache_blocksize);
			printf("   Needs a single 8x%d CAM\n",cache_ways);
			break;
		case 'D':
			d_tags=(long*)malloc(cache_ways * sizeof(long)); // Make an array of tags - one for each set
			for(i=0;i<cache_ways;i++) d_tags[i]=0; // Initialize the array to tags of zero (unset)
			printf("%sK Direct Access Cache with %d sets of %d bytes\n",
				argv[2],cache_ways,cache_blocksize);
			cache_set_bits=mlog2(cache_ways); // The number of bits in the set depends on how many ways we have
			break;
		case 'S':
			if (argc < 4) {
				printf("Ways parameter (4th parameter) required for set associated caches\n");
				return 1;
			}
			s_ways=atoi(argv[4]);
			assert(0==cache_ways%s_ways); // There must be an even number of ways in each set
			s_sets = cache_ways / s_ways; // Determine how many sets
			s_cams=(camptr *)malloc(s_sets * sizeof(camptr)); // Make an array of CAMS - one for each set
			for(i=0;i<s_sets;i++) s_cams[i]=cam_create(s_ways,0); // Create a cam for each set that has s_ways entries
			cache_set_bits=mlog2(s_sets);
			printf("%sK Set Associative Cache with %d %d-way sets of %d bytes\n",
				argv[2],s_sets,s_ways,cache_blocksize);
			printf("   Needs  %d 8x%d CAM\n",s_sets,s_ways);
			break;
		default:
			printf("Cache type %c not yet supported\n",cache_type);
			return 1;
	}

	char trace[80];
	while(fgets(trace,sizeof(trace),stdin)) {
		char tt;
		void * taddr;
		//int offset;
		unsigned long tag;
		unsigned long set;
		int way;
		if (2==sscanf(trace,"%c %p ",&tt,&taddr)) {
			if (tt!='I' && tt!='R' && tt!='W') continue;
			//offset=taddr & offset_mask;
			int nontagbits=cache_offset_bits + cache_set_bits;
			tag=(unsigned long)taddr>>nontagbits;
			tag=tag<<(nontagbits%4); // Express in multiples of 4
			set = (unsigned long)taddr>>cache_offset_bits;
			set &= (1<<cache_set_bits)-1; // turn off tag bits
			switch(cache_type) {
				case 'F':
					way=cam_read(f_cam,tag);
					
					if (way==-1 || !way_isValid(ways,way)) 
					{
						way = way_firstUsed(ways, 0, cache_ways);
						cam_write(f_cam, way, tag);
						way_setValid(ways, way);
					}
					
					break;
				case 'D':
					way=set;
					if (!way_isValid(ways,way) || d_tags[way]!=tag) { // Replace this set
						d_tags[way]=tag;
						way_setValid(ways,way);
					}
					break;
				case 'S':
					way=cam_read(s_cams[set],tag);
					if (way==-1 || !way_isValid(ways,way+set*s_ways)) { // Replace this way
						way=way_firstUsed(ways,set*s_ways,(set+1)*s_ways); // find victim
						cam_write(s_cams[set],way%s_ways,tag);
						way_setValid(ways,way);
					} else {
						way+=set*s_ways;
					}
					break;
				default:
					printf("Cache type %c not yet supported\n",cache_type);
			}
			way_setUsed(ways,way);
			if (tt=='W') way_setDirty(ways,way);
		}
	}
	report_waystats(ways);

	// Dump cache contents
	int way;
	int set;
	switch(cache_type) {
		case 'F':
			printf("        Tag       | Lin | V | D | Data\n");
			printf("------------------+-----+---+---+-------\n");
			for(way=0;way<cache_ways; way++) {
				if (!way_isValid(ways,way)) continue;
				printf(" %016lx | %3d | %d | %d | ...\n",
					cam_value(f_cam,way),way,1,way_isDirty(ways,way));
			}
			cam_free(f_cam);
			break;
		case 'D':
			printf("Set |        Tag       | V | D | Data\n");
			printf("----+------------------+---+---+-------\n");
			for(way=0;way<cache_ways; way++) {
				if (!way_isValid(ways,way)) continue;
				printf(" %2x | %016lx | %d | %d | ...\n",
					way,d_tags[way],1,way_isDirty(ways,way));
			}
			free(d_tags);
			break;
		case 'S':
			printf("Set | Way|       Tag        | V | D | Data\n");
			printf("----+----+------------------+---+---+-------\n");
			for(set=0;set<s_sets; set++) {
				int way;
				for(way=0;way<s_ways;way++) {
					int fway=way+set*s_ways;
					if (!way_isValid(ways,fway)) continue;
					printf(" %2x | %2d | %016lx | %d | %d | ...\n",
						set,way,cam_value(s_cams[set],way),1,way_isDirty(ways,fway));
				}
			}
			for(i=0;i<s_sets;i++) cam_free(s_cams[i]);
			free(s_cams);
			break;
		default:
			printf("Cache type %c not yet supported\n",cache_type);
			return 1;
	}
	ways_free(ways);
	return 0;
}

int mlog2(int n) {
	int i;
	for(i=0;i<64;i++) {
		if (n==(1<<i)) return i;
		assert(n>(1<<i));
	}
	return 64;
}
