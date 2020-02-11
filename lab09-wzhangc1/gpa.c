#include <stdio.h>

int grade2gp(char *grade);
enum lgrades { A, Am, Bp, B, Bm, Cp, C, Cm, Dp, D, Dm, F};

int main() {

	char grade[10];
	int crhrs;
	int sumGP=0; // Note... we represent grade points using tenths of a grade point as a unit to avoid float
	int totHrs=0;
	
	while(2==scanf("%d %s ",&crhrs,grade)) {
		sumGP+=crhrs*grade2gp(grade);
		totHrs+=crhrs;
	}
	printf("GPA=%4.2f\n",(float)sumGP/(10.0*totHrs));
	return 0;
}

int grade2gp(char *grade) {
	enum lgrades gr;
	switch(grade[0]) {
		case 'A' : 
			if (grade[1]==0x00) gr=A;
			else if (grade[1]=='-') gr=Am;
			else gr=F; // invalid grade
			break;
		case 'B' :
			if (grade[1]==0x00) gr=B;
			else if (grade[1]=='+') gr=Bp;
			else if (grade[1]=='-') gr=Bm;
			else gr=F;
			break;
		case 'C' :
			if (grade[1]==0x00) gr=C;
			else if (grade[1]=='+') gr=Cp;
			else if (grade[1]=='-') gr=Cm;
			else gr=F;
			break;
		case 'D':
			if (grade[1]==0x00) gr=D;
			else if (grade[1]=='+') gr=Dp;
			else if (grade[1]=='-') gr=Dm;
			else gr=F;
			break;
		default: 
			gr=F;
	}
	
	switch(gr) {
		case A : return 40;
		case Am: return 37;
		case Bp: return 33;
		case B : return 30;
		case Bm: return 27;
		case Cp: return 23;
		case C : return 20;
		case Cm: return 17;
		case Dp: return 13;
		case D : return 10;
		case Dm: return 7;
		case F : return 0;
	}
	return 0;
}
