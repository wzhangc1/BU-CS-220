test : usell
	./usell 56 42 29 51 29 42 3
	
usell : usell.c llnode.c llnode.h
	gcc -g -Wall -o usell usell.c llnode.c
	
clean :
	-rm usell