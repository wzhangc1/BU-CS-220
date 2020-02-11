test : gpa
	./gpa <transcript.txt
	
gpa.s : gpa
	objdump -Sl gpa >gpa.s
	
rodata : gpa
	objdump -sj.rodata gpa	
	
gdb : gpa
	gdb -x gdb_cmds.txt gpa
	
gpa : gpa.c
	gcc -g -Wall -o gpa gpa.c
	
clean : 
	-rm gpa gpa.s	
