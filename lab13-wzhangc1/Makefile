testF : cache x86Math_trace.txt
	./cache F 8 6 <x86Math_trace.txt
	
testD : cache x86Math_trace.txt
	./cache D 8 6 <x86Math_trace.txt
	
testS : cache x86Math_trace.txt
	./cache S 8 6 4 <x86Math_trace.txt	
	
xmprF : cache xmpr_trace.txt
	./cache F 8 6 <xmpr_trace.txt
	
xmprD : cache xmpr_trace.txt
	./cache D 8 6 <xmpr_trace.txt
	
xmprS : cache xmpr_trace.txt
	./cache S 8 6 4 <xmpr_trace.txt	
	
xmpcF : cache xmpc_trace.txt
	./cache F 8 6 <xmpc_trace.txt
	
xmpcD : cache xmpc_trace.txt
	./cache D 8 6 <xmpc_trace.txt
	
xmpcS : cache xmpc_trace.txt
	./cache S 8 6 4 <xmpc_trace.txt	
		
cache : cache.c cam.c cam.h ways.h ways.c
	gcc -g -Wall -o cache cache.c ways.c cam.c

x86Math_trace.txt : x86Math
	gdb -x gdb.cmds --args x86Math 4
	
xmpr_trace.txt : xmpr
	gdb -x gdb.cmds --args xmpr
	
xmpc_trace.txt : xmpc
	gdb -x gdb.cmds --args xmpc		
	
x86Math : x86Math.c Makefile
	gcc -O0 -g -Wall -o x86Math  x86Math.c
	
xmpr: xmpr.c array.c array.h
	gcc -g -I. -o xmpr xmpr.c array.c
	
xmpc:xmpc.c array.c array.h
	gcc -g -I. -o xmpc xmpc.c array.c	
	
clean:
	-rm  x86Math x86Math_trace.txt xmpr xmpr_trace.txt xmpc xmpc_trace.txt cache
