leftBit.s : leftBit
	objdump -dSl leftBit >leftBit.s

gdb : leftBit
	gdb -ex "set disassemble-next-line on" -ex "break main" -ex "run" --args ./leftBit 14

test : leftBit
	./leftBit 14
	
leftBit : leftBit.c
	gcc -g -Wall -o leftBit leftBit.c
	
clean :
	-rm leftBit leftBit.s
