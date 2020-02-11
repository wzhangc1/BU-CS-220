test : warehouse order1.txt order2.txt order3.txt
	./warehouse order1.txt
	./warehouse order2.txt
	./warehouse order3.txt
	
warehouse : warehouse.c bench.h bench.c order.c order.h benchOrder.h
	gcc -Wall -g -o warehouse warehouse.c order.c bench.c 2>&1 | tee -a comp.txt
	
clean :
	-rm warehouse 
