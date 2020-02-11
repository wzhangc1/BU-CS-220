# Lab11 - Buffer Overflow Attack

We have been learning about buffer overflow attacks, specifically in [Lecture 16](http://www.cs.binghamton.edu/~tbartens/CS220_Spring_2019/lectures/L16_Buffer_Overflow_Attack.pdf). Today, we will implement a very simple buffer overflow attack. It is simple because I will provide you with a target function to execute. Instead of asking you to figure out how to add new code to memory and branching to that new code, in this simplified version of a buffer overflow attack, all you need to do is to branch to a function which already exists in the code. Furthermore, the function you branch to does not depend on a clean environment. Your buffer overflow attack can leave things such as the stack registers in an incorrect state, and the buffer overflow attack will still work.

## Mixing ASCII and Binary Data

In our lecture about buffer overflow attacks, we made the point that in order to perform a buffer overflow attack, we need to mix both ASCII and binary data in a single file, and then feed that file (using UNIX redirection) into the program. We also talked about the fact that all the editors we typically use *only* edit ASCII text, and don't deal very well with mixtures of ASCII and binary data. 

For this lab (and for the buffer overflow project), I am providing you with a C program that allows you to create a file that contains both ASCII and binary data. The code for this program is in file **mix.c**, and there is a make rule in the Makefile provided called `mix` which builds the mix binary file from the C code.

The mix program reads a set of specifications from standard input, and writes its results to standard output. In the Makefile, there is a target called `solution.txt` which runs the mix program, and redirects the file **mix_in.txt** to standard input, and redirects standard output to the file **solution.txt**.  Therefore, we provide specifications in mix_in.txt, and use the result in solution.txt as input for our buffer overflow attack. (The test target in the Makefile redirects standard input from the solution.txt file.)

The mix file reads interprets its input as follows:

  - The mix program checks the first character of each line of input.
  - If the first character is a pound sign (#), this input line is treated as a comment, and ignored
  - If the first character is an 'S', then the second character is ignored, but the third character - up to the newline that ends this line - is treated as an ASCII string, and that ASCII string is written to the output. Note that the newline character that ends this line is *not* written to the output.
  - If the first character is an 'X', then the second character is ignored, but the remainder of the line must consist of pairs of hexadecimal digits (numbers from 0-9, or letters from A-F), separated by spaces. Each pair of hexadecimal digits is translated to a single byte of binary data, and that byte is written to the output. Note that since we are working on little-endian hardware, if you intend the result of an 'X' line to be treated as a 64 bit value (such as an address or a long integer), you must specify the hex byte values in reverse order.
  - If the first character is an 'A', then the second character is ignored, but the next blank delimited word on the line is assumed to consist of up to 16 hexadecimal digits. These hexadecimal digits are assumed to represent a 64 bit address, and that address is written (in reverse byte order because we are working on a little-endian machine) to the output.
  
As an example, suppose you have the following input specification:

```
# The following is an example input file for the mix program
# ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+----A...
S This is a character string. Hex=>
X A0 A1 A2 A3
S <= and address =>
A 00007FFFFFFFA0AC
S <=.
X 0A
```

If I run this input specification through the mix program, the output looks like:

```
This is a character string. Hex=>????<= and address =>??????<=.
```

If I print this output using the octal dump (od) command with parameters to show both hex and ASCII values, I get:

```
000000   T   h   i   s       i   s       a       c   h   a   r   a   c
        54  68  69  73  20  69  73  20  61  20  63  68  61  72  61  63
000010   t   e   r       s   t   r   i   n   g   .       H   e   x   =
        74  65  72  20  73  74  72  69  6e  67  2e  20  48  65  78  3d
000020   > 240 241 242 243   <   =       a   n   d       a   d   d   r
        3e  a0  a1  a2  a3  3c  3d  20  61  6e  64  20  61  64  64  72
000030   e   s   s       =   > 254 240 377 377 377 177  \0  \0   <   =
        65  73  73  20  3d  3e  ac  a0  ff  ff  ff  7f  00  00  3c  3d
000040   .  \n
        2e  0a
000042
```

Notice the comment in the input file has a scale that makes it easy to count the number of characters in a string. 

We can use the mix program to provide input for the buffer overflow attack. I have provided a file called **mix_in.txt** that you will need to edit to perform your buffer overflow attack, but you will need to add information to this file to succesfully complete the lab.

## Files Provided

The lab11 repository comes with the following files:

  - **taget.c** : This is the C code which is vulnerable to a buffer overflow attack, and has a `target` function which needs to be the target of your buffer overflow attack.  The target program reads strings from standard input, and prints them out until it hits end-of-file on standard input.  Then the program prints the message `Yech.... you failed to run the target.`, and quits with a non-zero (error) return code. You are not allowed to change the code in this file for today's lab.
  
    There is a target function in target.c that is never invoked by the existing code. Your job today is to perform a buffer overflow attack which branches to that target function. If you are succesfull, the target function will print a message that says `Whooopeeee... you ran the target!`, and exits the code with a zero (no error) return code.
  
  - **mix.c** : The C code to implement the mix program defined above.
  
  - **mix_in.txt** : A file that contains specifications for the mix program. These specifications are translated by the mix program into an output file called **solution.txt**, wich is fed into the target program. Edit this file to provide specifications that create a solution.txt file that will perform your buffer overflow attack.
  
  - **gdb_cmds.txt** : A set of gdb commands to be run after loading the target program. Note that the commands include a run command which redirects the **solution.txt** file to standard input. You probably want to edit this file as you learn more about the target program, and your buffer overflow attack.
  
  - **Makefile** : A make command makefile for this project, that includes the following targets:
    - test : Runs the target executable (without Address Space Layout Randomization), redirecting the **solution.txt** file to standard input.
    - gdb : Runs the gdb command on the target executable, using gdb_cmds.txt as a commands file.
    - target.s : Runs objdump -d on the target executable file, and saves the result in target.s
    - target : builds the target executable.
    - showhex : Runs the od command with the correct flags on the solution.txt file
    - solution.txt : Runs the mix executable redirecting **mix_in.txt** to standard input, and redirecting standard output to **solution.txt**
    - clean : Remvoes all the intermediate files produced by the Makefile
    - mix : Builds the mix executable.
    
## Working the Lab

Start off by making the test target. This will build the mix executable, run the mix_in.txt file into the mix command to produce solution.txt, build the target executable, and run the target executable with the solution.txt file as input. Since we haven't changed mix_in.txt yet, you will not be successful in a buffer overflow attack.

Next, make target.s and look at it to see what you need to provide in your input file to succesfully perform a buffer overflow attack. Use this information to modify the mix_in.txt file.

It is very useful to run gdb to figure out what is going on with your buffer overflow attack.  For instance, after the gets function is run, what does your stack look like?  Have you overwritten the return address?  If so, what value is there? Is it what you expected it to be?  If so, then when you get to the "ret" instruction in the getString function, you should be able to do a nexti gdb command and see where you end up.

## Submitting your Solution

When your buffer overflow attack is sucessful, and commit and push your changes (most importantly, changes to mix_in.txt). Cut and paste the resulting hashCode into the LAB11 submission area in the Lab Submissions folder under Content for CS-220 in myCourses. You may submit as many times as you wish up to the deadline of midnight April 16, only the last submission will be graded.

## Grading your Submission

If you turn in your code late, 4 points will be deducted, unless you have received an extension from the professor. Any submissions after Wednesday, April 17 will receive a zero grade.

If your buffer overflow attack is sucessful, you will get the full 10 points. If the buffer overflow attack is not succesful, but you made a valid attempt at a buffer overflow attack, you will get 6 points.
