# Lab05 - Visualizing Arrays and Pointers

We learned about arrays, and how arrays are laid out in memory in row major order, and we also learned about pointers to pointers, which look a lot like two dimensional arrays, and can be used like two dimensional arrays, but are implemented very differently under the covers. Today's lab is an attempt to demonstrate these differences, and to graphically portray how memory is organized for various different structures.

I will give you some pre-declared variables in C, as well as a library of functions to print out the underlying memory organization for these variables. I'm going to ask you to invoke the library functions on a representative subset of the variables I provide, and I will ask you to write some code to analyze the memory organization and the C capability of using these variables.

## Infrastructure Provided

The repository is initialized with several files:

- **showArray.c** - This is the C file that has the main function, the declaration and initializaiton of all of the variables we will deal with, and which you will need to edit to invoke the library functions and add the analysis code.

- **format.h** - The header file for the library functions I am providing... see the section below for more details. Please don't change this file.

- **format.c** - The definition of the library functions decribed below. Please don't change this file.

- **Makefile** - A standard make file to build, test, and debug (including gdb and valgrind targets) the showArray executable.

## The "format" library provided

I've been working on a library of functions to print out the memory organization and values for C variables. (Note - suggestions for improvement and/or simplifications are welcome!) These are provided in the float.h and float.c files.

The basic idea is to provide functions which deal with a list of strings to describe a variable. Each string in the list is NULL terminated (as all strings are in C), and the list itself has a variable number of strings, guarded by an empty string at the end of the list. This data type is used so often, there is a typedef for it called *fmtDesc* in the format.h header file. A description of type fmtDesc is created by the various library functions, and then printed using the `printFormat` function.

The formating functions typicially take a void pointer as an argument, as well as an enumerated data type (see the `fmtType` enumeration in format.h to see the valid types of data supported.) There are different formating functions to handle different kinds of variables.

- `fmtDesc formatData(void *data,enum fmtType type)` : creates a simple description with just the data value.
- `fmtDesc formatPtrScalar(void *data,enum fmtType type)` : Precedes the value with a boxed pointer value and an arrow (->) to show where the data resides in memory.
- `fmtDesc formatPtrVector(void *data,int len, enum fmtType type)` : Assumes that the data pointer points to a list of values that is `len` items long. Prints the list with a pointer, an arrow, and a comma separated list of values.
- `fmtDesc formatPtrMatrix(void * data,int rows,int cols,enum fmtType type)` : Assumes the data pointer points to a matrix of values in row major order with `rows` rows and `cols` columns. The description has the address, an arrow, and rows of data where columns are comma separated, and rows are on top of each other.
- `fmtDesc formatPtrPlistScalar(void **data,int nptrs,enum fmtType type)` : Assumes `data` points to a list of `nptrs` pointers, where each pointer points to a scalar value. The description has the `data` pointer pointing at a series of rows, one for each pointer in the pointer list. Each row contains the pointer value, and the value of the scalar pointed to by that pointer.
- `fmtDesc formatPtrPlistVector(void **data, int nptrs, int len, enum fmtType type)` : Same as `formatPtrPlistScalar`, except each pointer now points to a vector that has `len` elements, described as a comma separated list of values.
- `fmtDesc formatPtrPlistPlistScalar(void ***data,int nptr1,int nptr2,enum fmtType type)` : In this case, `data` points to a list of pointers that is `nptr1` items long. Each pointer points in this first list points at a second list of pointers that is `nptr2` items long. Each pointer in the second list points at a single value. The description starts with the `data` pointer, followed by an arrow, followed by a vertial list of pointers in the first list. Each pointer in the first list is followed by an arrow, pointing to a vertical list of pointers in the second list. Each pointer in the second list is followed by an arrow, which points to the value at that location.

Finally, there is the `printFormat` function, which takes the result of one of these previous functions and prints it to the screen.

If you have time and/or are interested, take a look inside the `format.c` file to see how this is all accomplished. There are several sub-functions defined in format.c to do things like concatenate to descriptions side by side (catFmt), or stack descriptions one on top of each other (stackFmt), or put a box arround a description (boxFmt). Each of these mallocs whatever space is required, and frees whatever space it has consumed.

## Working the Lab

After you have cloned the repository, edit the `showArray.c` file, and take a look at the declarations and initialization code provided. Then, add some lines that invoke the `printFormat` function on some of the variables. Try to pick one or two variables of each different flavor, and see if you can figure out which formating function to invoke for each one.  For instance, you could start out with somethign very simple like:

```
printFormat("f3:",formatData((void *)&f3,fmtFloat));
```

That's really boring, but you can get a little fancier with something like:

```
printFormat("&f6:",formatPtrScalar(&f6,fmtFloat));
```

Add code for at least one scalar, vector, matrix, pointer list, pointer list to a vector, and pointer list to a pointer list to a scalar.

When you are printing all this stuff out, can you recognized at least three different ways of putting the numbers from 0 to 11 in a 3 row, 4 column "array"?

## Analyzing the Variables in Code

There are five different questions I would like to get answered about these variables, and you will need to write some analysis code to determine the answers to these questions.  The questions are:

1. Can `matrix` and `plistVector` both be accessed using row and column array notation? If so, then for all valid rows and columns, you need to show that `matrix[row][col]==plistVector[row][col]`.

2. Are the values in `matrix` in row major order? If so, then if I go through all the valid `[row][col]` values in odometer order, then the address of the next `[row][col]` should be one more (using pointer arithmetic) than the previous `[row][col]` value.

3. Are the values in `plistVector` in row major order?

4. Can `plistPlist` be accessed using row/col array notation? If so, then for all valid rows and columns, it should be true that `matrix[row][col]==*(plistPlist[row][col])` for all valid rows and columns.

5. Is `plistPlist` in row major order? If so, `(float *)&(plistPlist[row][col])` should be one greater than the previous row and column in row major (odometer) order.

Your program should print out the answers to these five questions. I have done the first two for you, so all you need to do is compute the answers to questions 3, 4, and 5.

## Submitting your Lab

When you are finished debugging your code, make sure to commit and push your repository using git. Cut and paste the resulting hashCode into the Lab 05 submission area in the Lab Submissions folder under Content for CS-220 in myCourses. You may submit as many times as you wish up to the deadline of midnight tonight (Feb. 26), only the last submission will be graded.

## Grading your Submission

If you turn in your code late, 4 points will be deducted, unless you have received an extension from the professor. Any submissions after Thursday will receive a zero grade.

Missing or lack of attempt to solve the problem will result in a zero grade.

If your code does not compile, or has an obvious bug, you will get a 3 point deduction for each bug. Bugs include things like not invoking the format and print functions for a representative set of variables, not answering the five questions, or lots of extra debug prints occuring in your output. 

We will run `./showArray` and two points will be deducted for each incorrect response.
