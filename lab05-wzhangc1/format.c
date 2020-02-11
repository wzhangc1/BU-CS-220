#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "format.h"

/*-------------------------------------------------------------------------
   Prototypes for local functions
---------------------------------------------------------------------------*/
fmtDesc formatString(char * string);
fmtDesc formatPointer(void * data);
fmtDesc pointsTo(void *ptr, fmtDesc lines);
fmtDesc downFmt(fmtDesc lines);
fmtDesc boxFmt(fmtDesc lines);
fmtDesc formatFmtVec(void * data,int len,enum fmtType type);
fmtDesc catFmt(fmtDesc lfirst, fmtDesc lsecond);
fmtDesc stackFmt(fmtDesc lfirst, fmtDesc lsecond);
int fmtSize(enum fmtType type);
int countFmt(fmtDesc lines);
void freeFmt(fmtDesc lines);

/*-------------------------------------------------------------------------
   Definitions of Public Functions
---------------------------------------------------------------------------*/
fmtDesc  formatData(void *data,enum fmtType type) {
	char val[256];
	switch(type) {
		case fmtFloat: sprintf(val,"%8f",*((float*)data)); break;
		case fmtPointer: sprintf(val,"%016lx",(unsigned long)data); break;
		case fmtString : strcpy(val,(char *)data); break;
		case fmtInt : sprintf(val,"%6d",*((int*)data)); break;
		default : 
			assert(type==fmtFloat); // should never get here
	}
	return formatString(val);
}

fmtDesc  formatPtrScalar(void *data,enum fmtType type) {
	return pointsTo((void *)data,downFmt(formatData(data,type)));
}

fmtDesc formatPtrVector(void * data,int len, enum fmtType type) {
	return pointsTo(data,downFmt(formatFmtVec(data,len,type)));
}

fmtDesc  formatPtrMatrix(void * data,int rows,int cols,enum fmtType type) {
	int r;
	fmtDesc lines=formatFmtVec(data,cols,type); // Handle row 0
	for(r=1;r<rows;r++) {
		lines=stackFmt(lines,formatFmtVec(data+(cols*r*fmtSize(type)),cols,type));
	}
	return pointsTo(data,downFmt(lines));
} 

fmtDesc  formatPtrPlistScalar(void **data,int nptrs,enum fmtType type) {
	// data points to a list of pointers to floats
	fmtDesc lines=formatPtrScalar(*data,type);
	for(int i=1;i<nptrs;i++) {
		fmtDesc slines=formatPtrScalar(*(data+i),type);
		lines=stackFmt(lines,slines);
	}
	return pointsTo(data,downFmt(lines));
}

fmtDesc  formatPtrPlistVector(void **data, int nptrs, int len, enum fmtType type) {
	// data points to a list of pointers to vectors of floats
	fmtDesc lines;
	int i;
	lines=formatPtrVector(*data,len,type);
	for(i=1;i<nptrs;i++) {
		fmtDesc slines=formatPtrVector(*(data+i),len,type);
		lines=stackFmt(lines,slines);
	}
	return pointsTo(data,downFmt(lines));
}

fmtDesc  formatPtrPlistPlistScalar(void ***data,int nptr1,int nptr2,enum fmtType type) {
	// data points to a list of pointers to pointers to floats
	fmtDesc lines=formatPtrPlistScalar(*data,nptr2,type);
	int i;
	for(i=1;i<nptr1;i++) {
		fmtDesc slines=formatPtrPlistScalar(*(data+i),nptr2,type);
		lines=stackFmt(lines,slines);
	}
	return pointsTo(data,lines);
}

void printFormat(char *descr, fmtDesc lines) {
	printf("%s\n",descr);
	int i;
	for(i=0; lines[i][0]!=0x00; i++) {
		printf("   %s\n",lines[i]);
	}
	freeFmt(lines);
}

/*-------------------------------------------------------------------------
   Definitions of Local Functions
---------------------------------------------------------------------------*/

fmtDesc  formatString(char * string) {
	fmtDesc lines=malloc(2*PTRSIZE);
	lines[0]=strdup(string);
	lines[1]=strdup("");
	return lines;
}

fmtDesc  formatPointer(void *data) {
	return boxFmt(formatData(data,fmtPointer));
}

fmtDesc  pointsTo(void *ptr, fmtDesc lines) {
	fmtDesc ptrLines=formatPointer(ptr);
	// Make the pointer itself
	fmtDesc arrowLines=malloc(3*PTRSIZE);
	arrowLines[0]=strdup("  ");
	arrowLines[1]=strdup("->");
	arrowLines[2]=strdup("");
	ptrLines=catFmt(ptrLines,arrowLines);
	ptrLines=catFmt(ptrLines,lines);
	return ptrLines;
}

fmtDesc  downFmt(fmtDesc lines) {
	int nl=countFmt(lines)+1;
	fmtDesc nLines=malloc((nl+1)*PTRSIZE);
	nLines[0]=strdup(" ");
	for(int i=0;i<nl;i++) {
		nLines[i+1]=lines[i];
	}
	free(lines); // Don't do freeFmt because we are still using the strings
	return nLines;
}

fmtDesc  boxFmt(fmtDesc lines) {
	int nl=countFmt(lines)+2;
	fmtDesc nLines=malloc((nl+1)*PTRSIZE);
	int w=0;
	for(int i=0;lines[i][0]!=0x00;i++) {
		if (w<strlen(lines[i])) w=strlen(lines[i]);
	}
	char * sep=(char *)malloc(w+3);
	sep[0]='+';
	for(int i=0;i<w;i++) sep[1+i]='-';
	sep[w+1]='+';
	sep[w+2]=0x00;
	nLines[0]=sep;
	for(int i=0;lines[i][0]!=0x00;i++) {
		char * newLine=(char *)malloc(w+3);
		newLine[0]='|';
		for (int j=0;j<w;j++) {
			newLine[j+1]=(j<strlen(lines[i]))?lines[i][j] : ' ';
		}
		newLine[w+1]='|';
		newLine[w+2]=0x00;
		nLines[i+1]=newLine;
	}
	nLines[nl-1]=strdup(sep);
	nLines[nl]=strdup("");
	freeFmt(lines);
	return nLines;
}

fmtDesc  formatFmtVec(void * data,int len,enum fmtType type) {
	fmtDesc lines=formatData(data,type); // Handle first element
	for(int i=1;i<len;i++) {
		lines=catFmt(lines,formatData(", ",fmtString));
		lines=catFmt(lines,formatData(data+i*fmtSize(type),type));
	}
	return lines;
}

fmtDesc  catFmt(fmtDesc lfirst, fmtDesc lsecond) {
	int nlf,fw=0;
	for(nlf=0;lfirst[nlf][0]!=0x00;nlf++) {
		if (fw<strlen(lfirst[nlf])) fw=strlen(lfirst[nlf]);
	}
	int nls=countFmt(lsecond);
	int nl=(nlf>nls) ? nlf:nls;
	fmtDesc lines=malloc((nl+1)*PTRSIZE);
	for(int i=0;i<nl;i++) {
		char * newLine;
		if (i<nls) newLine=(char *)malloc(fw+strlen(lsecond[i])+1);
		else newLine=(char *)malloc(fw+1);
		if (i<nlf) strcpy(newLine,lfirst[i]);
		else newLine[0]=0x00;
		// Pad newLine with blanks up to fw width
		for(int lw=strlen(newLine);lw<fw;lw++) newLine[lw]=' ';
		newLine[fw]=0x00;
		if (i<nls) strcat(newLine,lsecond[i]);
		lines[i]=newLine;
	}
	lines[nl]=strdup(""); // Guard lines with an empty line
	freeFmt(lfirst);
	freeFmt(lsecond);
	return lines;
}

fmtDesc  stackFmt(fmtDesc lfirst, fmtDesc lsecond) {
	int nlf=countFmt(lfirst);
	int nls=countFmt(lsecond);
	fmtDesc lines=malloc(PTRSIZE*(nlf+nls+1));
	int i;
	for(i=0;i<nlf;i++) {
		lines[i]=lfirst[i];
	}
	free(lfirst[i]);
	free(lfirst);
	for(i=0;i<nls;i++) {
		lines[nlf+i]=lsecond[i];
	}
	free(lsecond[i]);
	free(lsecond);
	lines[nlf+nls]=strdup("");
	return lines;
}

int fmtSize(enum fmtType type) {
	switch(type) {
		case fmtFloat: return sizeof(float);
		case fmtPointer : return sizeof(void *);
		case fmtString: return sizeof(char);
		case fmtInt: return sizeof(int);
	}
	return 0;
}

int countFmt(fmtDesc lines) {
	int i;
	for(i=0; lines[i][0]!=0x00; i++) {};
	return i;
}

void freeFmt(fmtDesc lines) {
	int i;
	for(i=0; lines[i][0]!=0x00; i++) free(lines[i]);
	free(lines[i]);
	free(lines);
}
