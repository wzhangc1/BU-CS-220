stack.txt: fact
	./fact | tee stack.txt 

fact: fact.c stack.c stack.h
	gcc -g -Wall -rdynamic -o fact fact.c stack.c
	
fact.s : fact
	objdump -d fact -S -l > fact.s
	
clean:
	-rm fact fact.s stack.txt
