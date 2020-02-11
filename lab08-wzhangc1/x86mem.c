#include <stdio.h>

int gmat[2][3]={{0,-1,-2},{-10,-11,-12}};

int main(int argc, char **argv) {

	int r,c;

	int matrix[3][5]={{0,1,2,3,4},{10,11,12,13,14},{20,21,22,23,24}};
	int rowVal=0;
	
	for(r=0;r<3;r++) {
		if (r>0) {
			rowVal=matrix[r-1][0]+10;
		}
		for(c=0;c<5;c++) {
			matrix[r][c]=100 + rowVal + c;
			gmat[r][c]=-100 - rowVal - c;
		}
	}
	
	return 0;
}
