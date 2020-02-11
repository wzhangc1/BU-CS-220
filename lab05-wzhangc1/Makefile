test : showArray
	./showArray
	
valgrind : showArray
	valgrind --leak-check=full ./showArray	
	
gdb : showArray
	gdb --args ./showArray	
	
showArray : showArray.c format.c format.h
	gcc -g -Wall -o showArray showArray.c format.c 
	
clean :
	-rm showArray
