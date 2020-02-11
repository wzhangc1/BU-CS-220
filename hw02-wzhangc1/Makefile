test : floatSum
	./floatSum 1.0 -2.75
	./floatSum 3.2 -1.3
	./floatSum 6.22e23 0.005
	
testSpecial : floatSum
	./floatSum nan 0.0
	./floatSum inf 3.754
	./floatSum 7.35 0.0
	./floatSum -inf 3.34
	
testSubNorm : floatSum
	./floatSum 1.3e-42 1.0
	./floatSum 1.0e-20 3.2e-10
	./floatSum 7.993e-20 8.32e-20
	./floatSum 7.993e-24 8.32e-23
	
floatSum : floatSum.c floatInt.h floatInt.c
	gcc -Wall -g -o floatSum floatSum.c floatInt.c -lm
	
clean : 
	-rm floatSum
