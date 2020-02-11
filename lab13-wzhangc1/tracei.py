def tracei() :
	# print("Running python tracei.py");
	import re
	gdb.execute("set logging overwrite on")
	# print("I %s" % gdb.parse_and_eval("$rip"));
	fqmodule=gdb.current_progspace().filename;
	no=re.search(".*/(.*)",fqmodule);
	if (no is None) : 
		print("Unable to find module name in : %s " % fqmodule)
		return;
	module=no.group(1)
	print("Working on module %s" % module)
	gdb.execute("set logging file " + module + "_trace.txt");
	gdb.execute("set logging on");
	gdb.execute("set height unlimited");
	while (1) :
		x=gdb.execute("x /i $rip",to_string=True);
		# print("x is %s" % x)
		loc=re.search("=> (0x[0-9a-f]+) <[^>]*>:\s+(\w+)\s*(.*)",x);
		if loc is None:
			loc=re.search("=> (0x[0-9a-f]+):\s+(\w+)\s*(.*)",x);
			if (loc is None):
				print("Unable to parse instruction at rip: %s" % x)
				return
		# print("loc is %s" % loc)
		print("I %s" % loc.group(1))
		iaddr = loc.group(1);
		op = loc.group(2);
		operands=loc.group(3).strip();
		if (op == "push" or op == "pushq") :
			print("W %s" % gdb.parse_and_eval("$rsp + 8"))
			gdb.execute("stepi");
			continue
		if (op == "callq") :
			print("W %s" % gdb.parse_and_eval("$rsp + 8"))
			# print("   callq operands; %s" % operands);
			if (operands.find('__run_exit_handlers')!=-1) :
				print(" =====> Execution complete.");
				gdb.execute("kill");
				gdb.execute("quit");
				return
			if (operands.find('@')==-1) :
				gdb.execute("stepi");
				continue;
			print("Not auto-stepping into %s" % operands);
			gdb.execute("nexti");
			continue		
		if (op == "pop" or op == "leaveq") :
			print("R %s" % gdb.parse_and_eval("$rsp"))
			gdb.execute("stepi");
			continue;
		if (op.startswith('j')) : 
			gdb.execute("stepi");
			continue # Jump instrunctions dont update memory
		if (op == "lea" or op=="nop") :
			gdb.execute("stepi");
			continue; # lea instrunctions never update memory
		#print("opcode is %s" % op)		
		#print("operands are %s " % operands)
		# Ignore operands that are registers
		opi=1
		while(operands) :
			no=re.match("%xmm\d(.*)",operands);
			if (no != None):
				operands=no.group(1)
				continue		
			no=re.match("%\w\w\w(.*)",operands);
			if (no != None):
				operands=no.group(1)
				continue
			no=re.match("%r\d+(.*)",operands);
			if (no != None):
				operands=no.group(1)
				continue
			no=re.match("\$0x[0-9a-f]+(.*)",operands);
			if (no != None): 
				operands=no.group(1)
				continue
			no=re.match(",(.*)",operands);
			if (no != None):
				operands=no.group(1)
				opi=opi+1
				continue;
			no=re.match("(-0x[0-9a-f]+)\(%(\w\w\w)\)(.*)",operands)
			if (no != None):
				addr=gdb.parse_and_eval("$" + no.group(2) + no.group(1));
				if (opi<2 or op== "cmpl" ) : print("R %s" % addr)
				else : print("W %s" % addr)
				operands=no.group(3)
				opi=opi+1
				continue
			no=re.match("(0x[0-9a-f]+)\(%(\w\w\w)\)(.*)",operands)
			if (no != None):
				addr=gdb.parse_and_eval("$" + no.group(2) + "+" + no.group(1));
				if (opi<2 or op== "cmpl" ) : print("R %s" % addr)
				else : print("W %s" % addr)
				operands=no.group(3)
				opi=opi+1
				continue			
			no=re.match("\(%(\w\w\w)\)(.*)",operands)
			if (no != None):
				addr=gdb.parse_and_eval("$" + no.group(1));
				if (opi<2 or op== "cmpl" ) : print("R 0x%x" % addr)
				else : print("W %s" % addr)
				operands=no.group(2)
				opi=opi+1
				continue
			no=re.match("(.*)\((.*),(.*),(.*)\)(.*)",operands);
			if (no != None) :
				offset=no.group(1);
				base=no.group(2);
				if (base=="") : base="0"
				row=no.group(3);
				if (row.startswith("%")) :
					row='$'+row.lstrip('%')
				rowlen=no.group(4);
				addr=gdb.parse_and_eval(offset + "+" + base + "+" + row + "*" + rowlen);
				print("Got offset %s base %s row %s rowlen %s = addr %x" % (offset,base,row,rowlen,addr));
				operands=no.group(5)
				return
			no=re.match("0x[0-9a-f]+ <.*>",operands) 
			if (no != None) : break
			no=re.match("\s*\#.*",operands)
			if (no != None) : break
			print("Unrecognized operand: %s" % operands)
			print("  In instruction: %s " % x)
			return
		gdb.execute("stepi");
	
