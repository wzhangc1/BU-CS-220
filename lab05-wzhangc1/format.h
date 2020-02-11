#define PTRSIZE 8 // Assuming 8 byte pointers on this machine

enum fmtType {
	fmtFloat,
	fmtPointer,
	fmtString,
	fmtInt
};

typedef char ** fmtDesc;

fmtDesc formatData(void *data,enum fmtType type);
fmtDesc formatPtrScalar(void *data,enum fmtType type);
fmtDesc formatPtrVector(void *data,int len, enum fmtType type);
fmtDesc formatPtrMatrix(void * data,int rows,int cols,enum fmtType type);
fmtDesc formatPtrPlistScalar(void **data,int nptrs,enum fmtType type);
fmtDesc formatPtrPlistVector(void **data, int nptrs, int len, enum fmtType type);
fmtDesc formatPtrPlistPlistScalar(void ***data,int nptr1,int nptr2,enum fmtType type);
void printFormat(char *descr,fmtDesc lines);
