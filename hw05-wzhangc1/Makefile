test : fib
	./fib 6 | tee stack.txt
	
gdb : fib
	gdb -x gdb_cmds.txt --args ./fib 6	
	
fib.s : fib
	objdump -dSl fib >fib.s	

fib : fib.c
	gcc -g -Wall -o fib fib.c
	
clean : 
	-rm fib fib.s
