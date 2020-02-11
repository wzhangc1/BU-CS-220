x86mem.s : x86mem
	objdump -d -Sl x86mem > x86mem.s
	
gdb : x86mem
	gdb -x gdb_cmds.txt --args ./x86mem	


x86mem : x86mem.c
	gcc -g -Wall -o x86mem x86mem.c
	
clean :
	-rm x86mem x86mem.s	
