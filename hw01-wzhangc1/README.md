# Homework 01 - Swap

Due Monday, Feb. 4 at 11:59 PM

## Cloning your Repository

To clone your repository, first find the green button above this README file named "Clone or Download". When you press that button, there is a pop-up with a URL address. Copy that URL address. Then, go to your UNIX command window, in the "cs220" sub-directory, and type the command:

`git clone <URL from the "Clone or Download" Button>`
  
When this command finishes, you should be able to do an `ls` to see a "hw01-*userid*" directory.  CD to that directory and do an `ls` to list the files in the directory. You should see several files in the directory. You can now make changes to those files, but remember, those changes are not registered in the cloud yet. The rest of these instructions assume you are in your "hw01-*userid*" directory.

## Problem Statement

In this homework, we are investigating some basic properties of integers. We will see some of these properties by working on the problem of swapping two integers. Suppose we have two integer variables, `a` and `b`. We swap those variables by putting the original value of `a` into the `b` variable, and the original value of the `b` variable into the `a` variable. 

## Swap function prototype

We will investigate several different ways to do a swap, but they will all use the same basic function prototype: 

  ```void swap_xxx(int *aptr, int *bptr);```  

The C language only allows a single return value, and our swap function needs to modify both `a` and `b`, so we can't use the return value of a function in this case. Also, the C language uses a "call by value" paradigm, so if we pass the *values* of `a` and `b` as parameters, we can modify those *values*, but not the *variables* themselves. Therefore, we use *references* to `a` and `b` as parameters, not the values themselves. Our parameters will be *pointers* to `a` and `b`, the addresses of where `a` and `b` reside in memory. To make things clear, we will call these parameters `aptr` and `bptr`. When we call our swap function, we need to pass in the address of `a` and `b` using the C address-of operator, ```&a``` and ```&b```.  Inside the function, we can use the C indirection operator ```(*aptr)``` or ```(*bptr)``` to reference the *value* at that address, therefore the value at `a` or the value at `b` . We'll talk more about pointers and addresses in a future lecture, but that's enough to make this assignment work.

I have provided a file in the hw01-*userid* repository called "swap.c". This file contains a main function, and the prototype for three different swap functions. At the bottom of this file, there are empty definitions for these three swap functions. Your job for this assignment is to fill in the definitions for these three functions. 

I have also provided a "Makefile" In the hw01-*userid* repository. The Makefile enables the `make` command to compile swap.c into an executable file called `swap`, and then invoke the `swap` executable with two command line parameters, 47 and 13. If you look at the "swap.c" main function, you will see that the main function reads these command line parameters, puts 47 into `a` and 13 into `b`, prints out their values, and then invokes each of the three swap functions on &a and &b, and writes the values of `a` and `b` after each invocation. Since the three swap functions don't do anything yet, if you run make, you should get:
```
>make
gcc -g -Wall -o swap swap.c
./swap 13 47
A=13 B=47
After simple swap, A=13 B=47
After xor bit swap, A=13 B=47
After xor word swap, A=13 B=47
```

## Simple Swap - Using a temporary variable

The call-by-reference vs. call-by-value nature of C is often illustrated with a very simple swap function - one that puts the value of `a` in a temporary variable, e.g. ```int temp=(*aptr);```. Then puts the value at b into a's location, and the temp value into b's location. Hint: You can use the ```(*aptr)``` dereference operator on the left side of an assignment(=) operator to *write* a new value to the address in `aptr`, as well as on the right side of the assignment operator to *read* the value at the address in `aptr`.

See if you can edit the ```swap_simple``` function in "swap.c" to perform the swap using the algorithm described in the previous paragraph.  Once you have this working, when you run make, you should get the following:
```
>make
gcc -g -Wall -o swap swap.c
./swap 13 47
A=13 B=47
After simple swap, A=47 B=13
After xor bit swap, A=47 B=13
After xor word swap, A=47 B=13
```

## Swap without a temporary variable

Just because computer scientists are nerdy, we often get fascinated by the challenge of doing the same function but more "cleverly". Often this leads to totally unreadable code which is basically worthless, but it can also uncover some basic truths about the underlying concepts (for instance leaky abstractions) that improve our understanding of computer science. In this case, we are going to investigate performing a swap without using an extra temporary variable.

If you google "swap without temp", you will probably find this very common arithmetic algorithm:
```
a = a + b;
b = a - b; // really a+b-b
a = a - b; // really a+b-a
```
This algorithm works... unless a+b overflows, which does happen (but it turns out that even if you do overflow, a leaky abstraction enables two's complement arithmetic to get the right answer!) But let's see if we can find an alternative anyway.

One other much less common solution is to go through the bits of `a` and `b`.  If a specific bit is the same in both `a` and `b`, don't do anything to that bit. If a specific bit is different, if we flip that bit (without changing the other bits), then we "swap" that bit in `a` and `b`.  For instance, if the bit is 0 in `a`, and 1 in `b`, then if we flip that bit in both `a` and `b`, it becomes 1 in `a` and 0 in `b`. If we do this for all the bits which differ between `a` and `b`, the result is swapping the entire values of `a` and `b`.

Of course, to make this work, we need to do some bit-twiddling.  Specifically, lets look at two functions... ```int getBit(int *aptr,int i)``` that retrieves the value of the ith bit in the value that `aptr` is pointing at, and ```void flipBit(int *a,int i)``` that flips the i'th bit of the value that `aptr` is pointing at... if it was a 1, make it a 0... if it was a 0, make it a 1.

The getBit function isn't too hard, using the C idiom we learned in class... namely putting a 1 in the proper bit position by using the shift left operator, ```1<<i```. We can then bitwise and(&) the result with the value ```(*aptr)```. If the result is non-zero, then the associated bit in the value must have been a 1. If the result is a zero, the associated value must have been a zero.  Add a getBit function with the prototype ```int getBit(int *aptr,int i)``` to the "swap.c" file.

The flipBit function is a little harder.  We could use a very similar strategy...
```	
void flipBit(int *aptr,int i) {
	if ((*aptr) & (1<<i) !=0) { // Bit is on... turn it off
		(*aptr) &= ~(1<<i);
	} else { // Bit is off... turn it on
		(*aptr) != 1<<i;
	}
}
```
This works, but I want to show you a much more *elegant* solution (elegant in the mathematic sense, simpler and more powerful) which uses an exclusive or (XOR) operator.

An XOR is true if either A is true or B is true, but not if both are true. The truth table for exclusive or is:

| A	  | B	  | A XOR B | 
|:---:|:---:|:---:|
| 0	  | 0	  | 0   |
| 0   |	1	  | 1   |
| 1	  | 0	  | 1   |
| 1	  | 1	  | 0   |

Notice that given a bit, Y, which is either 0 or 1, then Y XOR 0 = Y, but Y XOR 1 = !Y. 

C has four bitwise operators, '~' is a bitwise invert, '&' is a bitwise AND, '|' is a bitwise OR, and '^' is a bitwise XOR. (Note that the first three have corresponding *logical* operators, '!', '&&', and '||', but that there is *not* a logical XOR operator in C.) If we make a "mask" value which has a 1 bit at the position we want to flip, and 0 bits elsewhere, we can use a bitwise XOR to flip a single bit. Using this strategy, our flipBit function becomes...

```void flipBit(int *aptr,int i) { (*aptr) ^= 1<<i; } //flip the ith bit```

Add this function to "swap.c".

Now that we have getBit and flipBit functions, we need to loop through the bits of `a` and `b`. Normally, I would let you get away with the assumption that the `int` data type has exactly 32 bits. That's probably a pretty good assumption for 99.999% of the machines you will ever run into. However, there were some machines in the past that had 16 bit integers, and the C spec allows different size integers as long as they are a minimum of 16 bits. In this lab, let's be pedantic and show how to run on ANY machine. (In the future, I won't make you do this, but it's worth seeing at least once.)

We can get the number of *bytes* in an integer by using the C `sizeof` operator. (We normally think of, and use `sizeof` as if it were a function, but it really is a C operator!) Can we assume there are 8 bits in a byte?  Normally, I would let you do so, but again, the C specification does not *require* 8 bits in a byte... just a minimum of 8 bits. I've never heard of a machine that *doesn't* have 8 bits in a byte, but just in case, there is a constant defined in `limits.h` called "CHAR_BIT" which contains the number of bits in a byte. Therefore, add ```#include <limits.h>``` to "swap.c", and calculate the number of bits in an integer using the formula ```sizeof(int) * CHAR_BIT```.

Now we can make a loop through each bit. If we use `i` to keep track of which bit, the body of this loop can be:
```
if (getBit(aptr,i)!=getBit(bptr,i)) {
	flipBit(aptr,i);
	flipBit(bptr,i);
}
```

Code this loop in the `swap_xorbit` version of the swap function in "swap.c" and test it.  You should now get:
```
>make
gcc -g -Wall -o swap swap.c
./swap 13 47
A=13 B=47
After simple swap, A=47 B=13
After xor bit swap, A=13 B=47
After xor word swap, A=13 B=47
```

## Swap using XOR on the entire integer

One last example... we can use a bitwise xor to figure out which bits in A are equal to bits in B, and which are different! Notice that for a single bit, A XOR B is true only if A and B are different - not equal.  Therefore, a bitwise `(*aptr)^(*bptr)` gives a vector of bits in which there is a zero if that bit in `a` is the same as that bit in `b`, and 1 if the associated bits in `a` and `b` are different. We can use the resulting mask, again using a bitwise XOR to flip the bits in a and the bits in b where they differ.

See if you can use this trick to make a three line solution to the swap problem in function `swap_xorWord` in "swap.c". This is easiest if you make a temporary mask integer variable, but there is a more difficult solution that takes three lines of code, each of which uses a bitwise XOR, but does not use any intermediate variables. Either the simple version with the temporary mask, or the more complicated version without a temporary variable is acceptable.

Once you have this working, you should get the results:
```
>make
gcc -g -Wall -o swap swap.c
./swap 13 47
A=13 B=47
After simple swap, A=47 B=13
After xor bit swap, A=13 B=47
After xor word swap, A=47 B=13
```

## Any bugs?

Can you find any values for which one of the swap functions does not work? Try negative numbers or zero, and try very large numbers (close to overflow... hint: 1000000000 (that's a 1 with nine zeroes) is very close to 2 to the 31st power. If you have arguments in the billions, you should test your function's ability to work on large numbers.

## Submitting your Homework

When you are finished editing and testing your code, make sure to add, commit, and push your repository using git.  Then, do a `git rev-parse HEAD` from the project directory to get the hash code associated with the commit. Cut and paste that hashCode into the HW 01 submission area in the Homework Submissions folder under Content for CS-220 in myCourses. You may submit as many times as you wish up to the deadline of midnight Feb. 4, only the last submission will be graded.

## Grading your Submission

If you turn in your code late, 4 points will be deducted, unless you have received an extension from the professor. Any submissions after Wednesday, Feb. 6 will receive a zero grade.

Missing or lack of attempt to solve the problem will result in a zero grade.

If your code does not compile, or has an obvious bug, you will get a 3 point deduction for each bug. Bugs include things like one of the swaps not working for a specific value. At most two bugs will be deducted, making the maximum bug deduction 6 points.

There will be a 1 point deduction for each class of compiler warning generated by your code. 
