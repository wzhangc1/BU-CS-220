test : encode decode
	echo "This is a test" | ./encode 3
	echo "This is a test" | ./encode 3 | ./decode 3
	echo "What about a 5?" | ./encode 5 | ./decode 5

	
encode : encode.c
	gcc -g -Wall -o encode encode.c
	
decode : decode.c
	gcc -g -Wall -o decode decode.c
	
cmdParms : cmdParms.c
	gcc -g -Wall -o cmdParms cmdParms.c
	
clean :
	-rm encode decode cmdParms
