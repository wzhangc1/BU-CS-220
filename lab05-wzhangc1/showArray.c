#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "format.h"

int main(int argc, char **argv) {

	// Example scalars - jumbled so addresses wont be sequential
	float f0=0.0;	float f4=4.0; 		float f1=1.0;		 	float f7=7.0;
	float f2=2.0;	float f10=10.0;	float f3=3.0;			float f5=5.0;
	float f9=9.0;	float f6=6.0;		float f11=11.0;		float f8=8.0;

	// Example vectors - See code below for initializations
	float vec1[4];
	float vec2[4];
	float vec3[4];
	float * plist1[4];
	float * plist2[4];
	float * plist3[4];

	// Example arrays - three different flavors!
	float matrix[3][4];
	float *plistVector[3];
	float **plistPlist[3];

	// Initialize vectors
	vec1[0]=f0; vec1[1]=f1; vec1[2]=f2; vec1[3]=f3;
	vec2[0]=f4; vec2[1]=f5; vec2[2]=f6; vec2[3]=f7;
	vec3[0]=f8; vec3[1]=f9; vec3[2]=f10; vec3[3]=f11;

	plist1[0]=&f0; plist1[1]=&f1; plist1[2]=&f2; plist1[3]=&f3;
	plist2[0]=&f4; plist2[1]=&f5; plist2[2]=&f6; plist2[3]=&f7;
	plist3[0]=&f8; plist3[1]=&f9; plist3[2]=&f10; plist3[3]=&f11;

	// Initialize arrays
	matrix[0][0]=f0; matrix[0][1]=f1; matrix[0][2]=f2; matrix[0][3]=f3;
	matrix[1][0]=f4; matrix[1][1]=f5; matrix[1][2]=f6; matrix[1][3]=f7;
	matrix[2][0]=f8; matrix[2][1]=f9; matrix[2][2]=f10; matrix[2][3]=f11;

	plistVector[0]=(float *)&vec1;
	plistVector[1]=(float *)&vec2;
	plistVector[2]=(float *)&vec3;

	plistPlist[0]=(float **)&plist1;
	plistPlist[1]=(float **)&plist2;
	plistPlist[2]=(float **)&plist3;

	/*------------------------------------------------------------------------
		Insert code to print the structure of some of the
		above variables...
	--------------------------------------------------------------------------*/
	printFormat("f3:",formatData((void *)&f3,fmtFloat));
	
	printFormat("&f0:",formatPtrScalar(&f0,fmtFloat));
	
	printFormat("&vec1:",formatPtrVector(&vec1, 4, fmtFloat));
	
	printFormat("&matrix:",formatPtrMatrix(&matrix, 3, 4, fmtFloat));
	
	printFormat("&plist1",formatPtrPlistScalar((void*)&plist1, 4, fmtFloat));
	
	printFormat("&plistVector",formatPtrPlistVector((void**)&plistVector, 3, 4, fmtFloat));
	
	printFormat("&plistPlist",formatPtrPlistPlistScalar((void***)&plistPlist, 3, 4, fmtFloat));
	
	/*------------------------------------------------------------------------
		Insert analysis code here....
	--------------------------------------------------------------------------*/	

	/*------------------------------------------------------------------------
		1. Can matrix and plistVector both be accessed using row and column 
			array notation?
	--------------------------------------------------------------------------*/	

	int row; int col; int compare=1; // Assume comparison is true until proven otherwise
	for(row=0;row<3 && compare;row++) {
		for(col=0;col<4 && compare ;col++) {
			if (matrix[row][col] != plistVector[row][col]) compare=0;
		}
	}
	printf("1. matrix and plistVector %s both be accessed using row/col array notation.\n",compare?"can":"cannot");
	
	/*------------------------------------------------------------------------
		2. Are the values in matrix in row major order?
	--------------------------------------------------------------------------*/	
	
	compare=1; float * prev=NULL;
	for(row=0;row<3 && compare;row++) {
		for(col=0;col<4 && compare ;col++) {
			if (prev!=NULL) if ((prev+1) != &(matrix[row][col])) compare=0;
			prev=&(matrix[row][col]);			
		}
	}
	printf("2. matrix %s in row major order.\n",compare?"is":"is not");
	
	/*------------------------------------------------------------------------
		3. Are the values in plistVector in row major order?
	--------------------------------------------------------------------------*/
	/* Insert your code here ... */
	compare=1; prev=NULL;
	for(row=0;row<3 && compare;row++) {
		for(col=0;col<4 && compare ;col++) {
			if (prev!=NULL)	if (*prev+1 != plistVector[row][col]) compare=0;
			prev=&(plistVector[row][col]);		
		}
	}
	printf("3. plistVector %s in row major order.\n",compare?"is":"is not");

	/*------------------------------------------------------------------------
		4. Can plistPlist be accessed using row and column array notation?
	--------------------------------------------------------------------------*/			
	compare=1;
	/* insert your code here ... */
	
	for(row=0;row<3 && compare;row++) {
		for(col=0;col<4 && compare ;col++) {
			if (matrix[row][col] != *plistPlist[row][col]) compare=0;
		}
	}
	
	printf("4. matrix and plistPlist %s both be accessed using row/col array notation.\n",compare?"can":"cannot");
	
	
	/*------------------------------------------------------------------------
		5. Are the values in plistPlist in row major order?
	--------------------------------------------------------------------------*/		
	compare=1; float *prevPtr=NULL;	
	/* Insert your code here .... */
	
	for(row=0;row<3 && compare;row++) {
		for(col=0;col<4 && compare ;col++) {
			if (prevPtr!=NULL)	if (*prevPtr+1 != *plistPlist[row][col]) compare=0;
			prevPtr=plistPlist[row][col];
		}
	}
	
	printf("5. plistPlist %s in row major order.\n",compare?"is":"is not");
	
	return 0;
}
