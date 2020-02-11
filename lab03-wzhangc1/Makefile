test : ackermann
	./ackermann 1 1
	./ackermann 1 2
	./ackermann 2 1
	./ackermann 3 5
	
gdb : ackermann
	gdb -x gdb_cmds.txt --args ackermann 4 1 
	
ackermann : ackermann.c
	gcc -g -Wall -o ackermann ackermann.c
	
clean : 
	-rm ackermann