# HW04 - X86 Control Flow

## Background

Lecture 13 covers X86 control flow. This lab takes a short C program that we worked on in lab01, and demonstrates how the C compiler translates that simple C code into X86 assembler. Your job is to analyze the assembler produced and answer the questions about that assembler code in **hw04_questions.txt**.

## Files Provided

The hw04 repository comes with the following files:

- **leftBit.c** : The Professor's solution for the leftBit program developed for lab01. This file should not be modified.

- **Makefile** : A make command Makefile to produce and test a leftBit executable from the leftBit.c file, that also has targets to create file **leftBit.s** that contains the x86 assembler code in the leftBit executable, and a `gdb` target to run gdb.

- **hw04_questions.txt** : A list of questions to be answered. Please edit this file, filling in the answers, and commit your changes to this file.

## Working the Lab

Use the `leftBit.s` target in the Makefile to produce the x86 assembler code. Then, start answering the questions in **hw04_questions.txt** by analyzing the x86 assembler code. If you get stuck, run under gdb and see if that helps clear things up.

## Submitting your Solution

After you have added answers for all 10 questions in **hw04_questions.txt**, commit and push your changes to that file, Cut and paste the resulting hashCode into the HW04 submission area in the Homework Submissions folder under Content for CS-220 in myCourses. You may submit as many times as you wish up to the deadline of midnight April 3, only the last submission will be graded.

## Grading your Submission
If you turn in your code late, 4 points will be deducted, unless you have received an extension from the professor. Any submissions after Friday, April 5 will receive a zero grade.

Each question answered correctly is worht 1 point for a total of 10 points.
