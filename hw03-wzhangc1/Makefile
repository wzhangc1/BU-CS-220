test : countWords
	./countWords franz_kafka.txt
	
countWords : countWords.c btree.c btree.h
	gcc -g -Wall -o countWords countWords.c btree.c
	
clean :
	-rm countWords
