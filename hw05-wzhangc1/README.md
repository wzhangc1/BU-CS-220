# HW-05 X86 Stack Frames

We've been learning about the X86 stack in [Lecture 14](http://www.cs.binghamton.edu/~tbartens/CS220_Spring_2019/lectures/L14_x86_Stack.pdf), and this lab gives us a chance to put some of that to use. Stack frames are most interesting on recursive functions, so I have coded one of the classic recursive functions - one that calculates a Fibonacci number. Fibonacci numbers are fascinating, and surprisingly show up in nature over and over again. For an in-depth review of Fibonacci numbers, see the Wikipedia [Fibonacci Numbers](https://en.wikipedia.org/wiki/Fibonacci_number) article. I've coded the recursive definition of Fibonacci numbers quoted in that article, namely:

  ```fib(0)=0; fib(1)=1; fib(n)=fib(n-2)+fib(n-1) for n>1```
  
But the real point of this lab is not Fibonacci numbers, but the X86 stack. I have provided the code, and a series of questions about how that code uses the stack. In order to answer these questions you will have to look at the x86 expansion of the code, and run under gdb to look at specific stack data.

## Files Provided

The hw05 repository comes with the following files:

**fib.c** : An recursive implementation of a Fibonacci number calculator that takes a single integer command line argument, and returns the Fibonacci number associated with the argument. This file should not be modified.

**Makefile** : A make command Makefile to produce and test a `fib` executable from the fib.c file, that also has targets to create file fib.s that contains the x86 assembler code in the fib executable, and a gdb target to run gdb.

**gdb_cmds.txt** : A file that contains gdb commands to be executed after loading the fib executable, referenced by the Makefile gdb target.

**hw05_questions.txt** : A list of questions to be answered. Please edit this file, filling in the answers, and commit your changes to this file.

## Working the Lab

Use the fib.s target in the Makefile to produce the x86 assembler code for reference. Then, start answering the questions in hw05_questions.txt. For this lab, you will have to run gdb to get the absolute addresses asked for in the questions. The gdb target in the makefile is set up for question 1. You will have to edit the gdb_cmds.txt file to get set up to answer questions 2-5.

## Submitting your Solution

After you have added answers for all 6 questions in hw05_questions.txt, commit and push your changes to that file, Cut and paste the resulting hashCode into the HW05 submission area in the Homework Submissions folder under Content for CS-220 in myCourses. You may submit as many times as you wish up to the deadline of midnight April 15, only the last submission will be graded.

## Grading your Submission

If you turn in your code late, 4 points will be deducted, unless you have received an extension from the professor. Any submissions after Wednesday, April 17 will receive a zero grade.

Questions 1-5 are worth 2 points each for a total of 10 points. Partial credit of 1 point will be granted of more than half of the answers are correct.

There is a sixth extra credit question. If you answer this question wrong, no points will be deducted, but if you answer it correctly, up to 2 points may be added to your grade, up to a maximum of 10 points.
