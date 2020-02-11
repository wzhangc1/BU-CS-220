# Lab10 - Stack Tracing

We have been learning about the x86 stack, and the use of the stack to keep invocation records for functions in [Lecture 14](http://www.cs.binghamton.edu/~tbartens/CS220_Spring_2019/lectures/L14_x86_Stack.pdf). In today's lab, we demonstrate the use of stacks using our old standard recursive function - the factorial function. 

Today, I have provided you with a very special C utility function that formats and prints the entire call stack, from the "main" stack frame down to the "current" stack frame. (Actually, current+1 because the utility function has it's own stack frame that we leave out of the stack frame dump.) The function is called `printStackInfo()`, and as long as you include stack.h, and compile with stack.c, you can invoke the `printStackInfo` function to print out the current stack. This function uses some special C library functions to get more information about the stack, but most importantly, this function uses some special C constructs that allow us to use X86 registers as if they were C variables.

I've added an invocation of `printStackInfo` into our recursive factorial function, after the result has been calculated, but before the result is returned.

As with the last few labs and homeword assignments, I'm not asking you to write any code for this lab... just build and run existing code, look at the results, and figure out answers to the set of questions in the **lab10_questions.txt** file. 

## Files Provided
The lab10 repository comes with the following files:

**fact.c** : An recursive implementation of a factorial number calculator, as well as a main function that invokes factorial(4). This file should not be modified.

**stack.h** : A header file that defines the printStackInfo function along with some constants that control printing. This file should not be modified.

**stack.c** : The C code that implements the printStackInfo function. This file should not be modified, but you will need to read through this file and understand how the code works in order to answer all the lab questions.

**Makefile** : A make command Makefile to produce and test a fact executable from the fact.c file, that also has targets to create file fact.s that contains the x86 assembler code in the fact executable. The test target is called `stack.txt` because the test of the fact binary saves results to a file called **stack.txt** to make it easier to browse through the results of running the fact command. Note that every time you make the `stack.txt` target, the stack.txt file will get overwritten with the results of a new invocation of the fact executable.

**lab10_questions.txt** : A list of questions to be answered. Please edit this file, filling in the answers, and commit your changes to this file.

## Working the Lab

You will need to make `fact.s` and `stack.txt` in order to start answering the questions in **lab10_questions.txt**. You will also need to look through **stack.c** to see how the code works. 

What you see on the screen when you make `stack.txt` is also saved in the file `stack.txt`. (The Makefile pipes the standard output from the fact executable into the Unix `tee` command, which writes it's standard input to both standard output AND the file specified as a parameter.) What you see is the dump of the stack, from highest address to lowest, divided into stack frames. Each line of output describes an 8 byte chuck of the stack in terms of the address of the 8 byte chunk, the 8 byte chunk formated as a 64 bit hexadecimal value, followed by two 4 byte chunks of stack memory, formated as two 32 bit hexadecimal values.

Each stack frame printed is delimited with a special frame divider line that contains the size of the frame, the name of the executable, function, and function offset of the return address associated with this frame, and the hex value of that return address.

To make things a little clearer, I've also added a blank line and a line of output before each invocation of the `printStackInfo` function that indicates the value of the `n` parameter when the stack is printed.

## Submitting your Solution

After you have added answers for all 10 questions in lab10_questions.txt, commit and push your changes to that file, Cut and paste the resulting hashCode into the LAB10 submission area in the Lab Submissions folder under Content for CS-220 in myCourses. You may submit as many times as you wish up to the deadline of midnight April 9, only the last submission will be graded.

## Grading your Submission

If you turn in your code late, 4 points will be deducted, unless you have received an extension from the professor. Any submissions after Wednesday, April 10 will receive a zero grade.

Questions 1-10 are worth 1 point each for a total of 10 points. For qeustions with sub-parts, if you get most of the sub-parts correct, you will get full credit for the question.
