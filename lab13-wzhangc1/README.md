# Lab13 - Simulating Cache

## Background

We've started to talk about the use of Cache to speed up memory accesss without significantly increasing the cost of hardware. In [lecture 18](http://www.cs.binghamton.edu/~tbartens/CS220_Spring_2019/lectures/L18_Cache.pdf) we described three different Cache configurations, Fully Associative Cache, Direct Map Cache. and Set Associative Cache. Today's lab consists of a software model of different cache configurations in order to get a better understanding of how Cache works.

## Memory Traces

Our cache simulation will use a memory trace as input data, where the memory trace is very similar to the one described in Lecture 18. The memory trace consists of one line for each memory access performed by a program. Each line starts with a single letter, "I" for read instruction, "R" for read data, and "W" for write data, followed by the hexadecimal 64 bit address of that data.

I have put together a methodology for running a program under gdb with a python script to create a memory trace. This methodology uses a set of gdb start-up commands in a file to bootstrap the process by breaking at the first instruction in the main function, and when that breakpoint is hit, starts up a python script called **tracei.py**. The python script does several things, including:

  - Writing results to a file called **\<pgm\>_trace.txt** where \<pgm\> is the name of the binary file being executed.
  - For each instruction:
  
    - Printing a "I" line to the trace with the current value of %rip
    - Then looking at the X86 instruction, if the instruction is a call, or push, create a "W" for the current value of %rsp+8.
    - If the instruction is a call to a library function, invoke "nexti" so we don't trace library functions
    - If the instruction is a call to `__run_exit_handlers`, kill the current program and quit. We are done.
    - If the instruction is "lea", ignore it... we never read or write memory in an "lea" instruction
    - If the instruction is a pop or leave instruction, create a "R" for the current value of %rsp
    - For all other instructions, if the operand is a memory reference and it is not the last operand, generate a "R" for the operand value. If the last operand is a memory reference, generate a "W" for the operand value.   
     - Then, do a "stepi" command to step into the next instruction.
     
The result of the script is to write a memory trace to a file.  Note that this process may take significantly longer than running the program on its own, but it's worth the wait.

## Programs to Model

I have supplied three different C programs to use for simulating our Cache model.  The first program is a very simple program designed to evaluate a polynomial. I've used this program to demonstrate X86 arithmetic in previous lectures, so it is called "x86Math". It takes a single command line argument, an value of "x", and calculates the result of x^2 + 4x + 4, and then divides that result by x=2.

The second program called "xmpr" allocates an array of integers with 200 rows and 100 columns, and fills all 2000 elements with the number "3". It does this using row major order, filling in all columns of the first row, then all columns of the second row, and so on.

The third program called "xmpc" does the exact same thing as xmpr, except it does it in column major order - filling in all rows for the first column first, then all rows for the second column, and so on.

Generating memory traces for xmpr and xmpc takes about a full minute, so be patient when generating memory traces (each of these memory traces is about 7M big). After the memory trace is saved in a file, things run faster.

## Simulating Content Addressable Memory (CAM)

I have provided C software to simulate a Content Addressable Memory in files **cam.c** and **cam.h**. Remeber from Lecture 18 that CAM's are special memories that allow you to specify a data value and find out what row in the CAM that data is stored. The data associated with a CAM is stored in a structure, and a pointer to that structure is defined as data type `camptr`. 

The functions provided to deal with CAM are as follows:

  - `camptr cam_create(int rows,long initval)` : the `cam_create` function creates a camptr structure of a cam that contains `rows` rows, where each row has a 64 bit value. (You can think of each bit as a column of the RAM, so this creates a `rows`x64 CAM.) The second parameter, `initval`, contains an initial value that is written to each of the rows of the CAM. The function returns a pointer to the resulting data structure to be used in subsequent references to this CAM.
  
  - `void cam_write(camptr cam,int address,unsigned long value)` : This function writes the `value` parameter to the `address` row of the CAM specified by `cam`.
  
  - `int cam_read(camptr cam,unsigned long value)` : This function determines the first row (address) the value speciifed in the `value` parameter is stored in. If the value is not found in the CAM, a `-1` value is returned.
  
  - `unsigned long cam_value(camptr cam,in address)` : This function returns the value stored in the CAM at the address in `address`. This is used to write debug information about the cam.
  
  - `void cam_free(camptr cam)` : This function frees the data associated with the CAM.
  
We use these functions to model the CAM's used in both Fully Associative Caches and Set Associative Caches.

## Modeling Cache Ways

All three cache configurations use a cache "way" to model a single block of memory. I have provided the software to model a set of ways in files **ways.c** and **ways.h**. Again, a special data type is defined calles `waysptr` which contains a pointer to the structure used to keep track of a set of ways. The following functions are provided to deal with ways:

  - `waysptr ways_create(int n)` : A function to allocate space for `n` ways, and initialize those ways.  The entire set of ways contains statistics, including `hits` - the number of times memory was requested, and is contained in the ways, `misses` the number of times memory was requested, and is NOT contained in the ways, and `writes` - the number of times data from the cache was written back to main memory. Each way would normally have 64 bytes of data, but for the purposes of this simulation, we don't actually need to keep track of the values associated with the memory... just the address of the start of that memory.  The address is kept in a combination of the way's tag, the way's set, and the offset into the way. The tags and sets are maintained differently, depending on the cache configuration. For the purposes of this simulation, we will also ignore the offset into the way. However, we do keep track of three extra fields for each way - a flag to keep track of whether the way is valid (contains valid memory) or not, a flag to keep track of whether this way has been updated since it was read from memory, the "dirty" flag, and the last time this way was used. The ways_create function sets the valid and dirty flags to false, and lastUsed to zero.
  
  - `void way_setValid(waysptr ways, int way)` - Sets the valid flag to `true` for the way specified in the arguments. This function should *only* be called when resetting the data in a specific way. If you are doing this, you must need this block in the cache (and it wasn't already there), so this function increments the `misses` statistic. The function will check to see if the way is currently valid and dirty. If so, it increments the `writes` statistic, assuming you need to write the current way back to real memory. Since new data is written into this way, this function sets the `valid` flag to true, and the `dirty` flag to false.
  
  - `bool way_isValid(waysptr ways, int way)` - Returns the valid flag for the way specified in the arguments.
  
  - `void way_setDirty(waysptr ways, int way)` - Sets the dirty flag to `true` for the way specified in the arguments. This should be called every time you write data to this way.
  
  - `bool way_isDirty(waysptr ways, int way)` - Returns the dirty flag forhte way specified in the arguments.
  
  - `void way_setUsed(waysptr ways, int way)` - This function assumes that the way specified in the arguments is valid, and is being referenced. This increments the `hits` statistic, sets the lastUsed field to the current time, and increments the global time variable.
  
  - `int way_firstUsed(waysptr ways,int start, int end)` - This function finds the way in the set of ways between `start` and `end` which was used the longest time ago (the "least recently used" way). You can use this function to determine the best choice of a way to use for a new block of memory. This function will return any way that is not already marked as `valid`. If all ways are valid, this function will return the least recently used way.  (The `start` and `end` parameters are required for Set Associative Cache configurations where you want to choose the best victim within the current set of ways. For Fully Associative Cache configurations, use 0 for the start, and the total number of ways for the end parameter.)
  
  
  - `void report_waystats(waysptr ways)` - This function writes statistics about this set of ways to standard ouput.
  - `void ways_free(waysptr ways)` - Frees the memory associated with the specified ways.
  
## Files Provided

The lab13 repository comes with many files, as follows:

  - **x86Math.c** - Simple C program to trace.
  
  - **xmpr.c** and **xmpc.c** - Array initialization programs in row-major order and column-major order respectively.
    
  - **array.h** and **array.c** - Array manipulation library code used in array xmpr and xmpc programs.
  
  - **gdb.cmds** and **tracei.py** - GDB commands and python script used to generate memory traces.
  
  - **cam.h** and **cam.c** - Library code to simulate Content Addressable Memory (CAM)
  
  - **ways.h** and **ways.c** - Library code to simulate sets of cache ways.
  
  - **cache.c** - Main program to simulate cache.  See description below.
  
  - **Makefile** - A make command Makefile that supports the following targets:
  
    - `x86Math` - Build the x86Math executable (evaluate quadratic)
    - `xmpr` - Build the xmpr executable (initialize arrays in row major order).
    - `xmpc` - Build the xmpc executable (initialize arrays in column major order).
    - `x86Math_trace.txt` - Run GDB on x86Math to produce a memory trace file.
    - `xmpr_trace.txt` - Run GDB on xmpr to produce a memory trace file.
    - `xmpc_Trace.txt` - Run GDB on xmpc to produce a memory trace file.
    - `cache` - Build the cache simulation executable called `cache`
    - `testF` - Run cache simulation with a Fully Associative Cache on the memory trace from x86Math
    - `testD` - Run cache simulation with a Direct Map Cache on the memory trace from x86Math
    - `testS` - Run cache simulation with a Set Associative Cache on the memory trace from x86Math
    - `xmprF` - Run cache simulation with a Fully Associative Cache on the memory trace from xmpr
    - `xmprD` - Run cache simulation with a Direct Map Cache on the memory trace from xmpr
    - `xmprS` - Run cache simulation with a Set Associative Cache on the memory trace from xmpr
    - `xmpcF` - Run cache simulation with a Fully Associative Cache on the memory trace from xmpc
    - `xmpcD` - Run cache simulation with a Direct Map Cache on the memory trace from xmpc
    - `xmpcS` - Run cache simulation with a Set Associative Cache on the memory trace from xmpc
    - `clean` - Removes all the files created by this Makefile

## Simulating Cache

I have also provided the basic infrastructure for our cache modeling in a program called **cache.c**. This C program takes three or four command line parameters that define how the simulated cache should be configured.  These command line parameters are defined as follows:

  - Cache Configuration - a single letter, "F" for Fully Associated Cache, "D" for Direct Map Cache, and "S" for Set Associative Cache.
  
  - Cache Size - The total size of the cache, in Kilobytes (1026 bytes). I've been using the value of 8 to represent an 8K Cache for most of my testing - small but surprisingly useful.
  
  - OffsetBits - The number of bits used to determine the offset within a single way. The size of a way is 2^{OffsetBits}. Almost all modern Caches use 6 bits for the offset, which results in way sizes of 64 bytes. Note that the total number of ways for this  cache will be (Cache Size / 2^{OffsetBits})
  
  - WaysPerSet - The number of ways in each set of a Set Associative Cache (only needed if the Cache Configuration is "S".) This must divide evenly into the total number of sets (total cache size / 2^{OffsetBits})
  
The main function in **cache.c** reads these parameters, and prints messages about the selected cache configuration. The function also invokes `ways_create` to allocate and initialize the ways, and, depending on the cache configuration, either creates a single CAM using `cam_create` for a Fully Associative cache; an array of tags (one for each set) for a Direct Map Cache; or an array of tags *and* a CAM for each set for a Set Associative Cache.

Next, the main function in **cache.c** reads each line of a memory trace from standard input. (Note that lines that don't start with a "I", "W", or "R" are considered to be comments and ignored.) The address is divided up into sub-fields, `tag` and `set`. Note that there is a special feature of this program that pads the `tag` on the right with zeroes so that the hexadecimal digits of the tag match the hexadecimal digits of the original address. This is done so the result is easier to debug.  Note also that there is no concept of `set` for Fully Associative caches. In this case, `set` remains zero.

Once the `tag` and `set` sub-fields are extracted from the address, there is code to check to see if the required memory is already in the cache. Clearly how this is done depends on the cache configuration. If the way is not present - if this is a cache miss - then this code must choose a victim way (and the best way to do this is to use the `way_firstUsed` function to get the least recently used way), and invoke the `way_setValid` function on that way to update the statistics for that way (basically simulate returning the way to real memory if it is dirty, and copying new real memory to that way so that it becomes valid.)

In both cases where a cache miss occured and in cases where a cache-hit occured, if this is a write memory transation, the `way_setDirty` function needs to be invoked, and in all cases, the `way_setUsed` function needs to be invoked.

After looping through the entire memory trace, the `report_waystats` function is invoked, and depending on the cache configuration, the final contents of the tags, sets, Valid, and Dirty flags are printed.

## Working the Lab

You will need to update the **cache.c** file. I left out the code that determines how to simulate a Fully Associative cache.  Read through the file, find the comments that indicate where you need to provide code and what that code should do.  Then run the cache simulation on the x86Math memory trace. It turns out that the x86Math program is very short and doesn't do a good job of distinguishing between different cache configurations, so the statistics that result from this simple program for a Fully Associative Cache should be similar to the statistics for a Direct Map cache and a Set Associative cache.

Once you have the code working, run all three configurations on the xmpr and then the xmpc program and compare the results. For these examples, how does the hit rate compare? Even with a very small 8K cache, do you still get very high hit rates? Does C code do a pretty good job of maintaining locality? (No written answers required... just think about these questions.)

## Submitting your Code

When you are satisfied with your cache simulation, commit your cache.c file, push the results, and paste the resulting hash code on myCourses.

## Grading your Results

This lab submission is worth 10 points, and in general, you get 10 points for submitting a correct **cache.c** file. Deductions will occur for the following reasons:

If you submit late without an extension, there will be a three point deduction for every 24 hours you are late. Extensions are available only in special circumstances, and can only be given by the instructor or a TA.

There will be a four point deduction if there are compiler errors when compiling the cache executable.

There will be a three point deduction if your code does not run to completion processing memory traces from x86Math, xmpr, or xmpc.

There will be a two point deduction for each class of compiler warning message issued when compiling your code.

There will be a two point deduction if your function prints out any extra messages during execution.
