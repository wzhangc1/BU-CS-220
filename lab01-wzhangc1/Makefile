test : leftBit
	./leftBit 1
	./leftBit 14
	./leftBit -32045
	
leftBit : leftBit.c
	gcc -g -Wall -o leftBit leftBit.c
	
clean :
	-rm leftBit