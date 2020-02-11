# Lab 01 - Leftmost 1 bit

If you have not done the Demo homework, I've repeated some of the startup instructions here. If you have done the Demo homework, skim through the setup instructions, but start the actual instructions at [Cloning your Repository](#cloning-your-repository)

## Environment

We assume you are logged on to one of the computer science machines in the G07 lab. These are Unix based systems on which the git command is available and installed. You can check this by running the command ```git --version``` from a terminal window. You will get a message like ```git version 2.17.0``` as a response. We also assume you are looking at this README.md file using a web browser after having accepted the lab01 assignment, and successfully creating the lab01-*userid* repository in the cloud.

How you manage your home directory is up to you, but I suggest creating a ```cs220``` subdirectory of your home directory to contain all the work we will do in this class. From now on, we will assume you have a ```cs220``` subdirectory, and that is your current directory.

## Configuring GitHub 

The first time we use GitHub, we need to set up our git credentials. Enter the following two commands to the terminal:

```
git config --global user.name "Firstname Lastname"
git config --global user.email "email@binghamton.edu"
```

You need to do this only one time. From now on, GitHub will remember your name and email.

## Cloning your Repository

To clone your repository, first find the green button above this README file named "Clone or Download". When you press that button, there is a pop-up with a URL address. Copy that URL address. Then, go to your UNIX command window, in the "cs220" sub-directory, and type the command:

`git clone <URL from the "Clone or Download" Button>`
  
When this command finishes, you should be able to do an `ls` to see a "lab01-*userid*" directory.  CD to that directory and do an `ls` to list the files in the directory. You should see several files in the directory. You can now make changes to those files, but remember, those changes are not registered in the cloud yet. The rest of these instructions assume you are in your "lab01-*userid*" directory.

## Problem Statement

Sometimes it is very useful to be able to find the leftmost 1 bit in an integer. Mathematically, the position of the leftmost 1 bit in x is the least integer in *log<sub>2</sub>(x)*, so that if the position of the leftmost 1 is *n*, then *2<sup>n</sup><=x<2<sup>n+1</sup>*.

The simplest way to find the leftmost 1 bit in an integer is to check each bit, starting from the leftmost bit (the 31st bit position in a 32 bit integer), and incrementing to the right until we find a 1 bit on. We can use the trick of shifting to get a 1 at each position, and use the trick of bitwise AND-ing to see if the bit at that position is on.  The basic code might look something like the following:

```
int leftBit(int x) {
  int n;
  for(n=31;n>=0;n--) {
    if ((x & (1<<n))!= 0) return n; // The nth bit is on
  }
  return -1; // No bits are on
}
```

This works, but often forces us to loop through lots of bits until we find the first 1. A much more efficient algorithm is to assume that the leftmost 1 bit in x is in a *range* of bits, starting at a specific bit position, and within a specific width of bits. To start with, we can assume our range starts at position 0, and has a width of 32 bits. If we can cut the width of our range in half, then we reduce the problem to 16 bits. Cut it in half again to reduce the problem to 8 bits, then 4, then 2, then 1. Now, instead of needing up to 32 iterations of a loop, we can always find the answer in just 5 steps.

## Files provided to you

I have provided several files for this lab. The top level C code is in a file called "leftBit.c". This contains a "main" function that reads an integer from the command line argument, prints out the binary representation of that number (using the print binary example we talked about in class, coded in the printBin function), then invokes the `leftBit` function on the number.  You will need to code the `leftBit` function.

I have also provided a "Makefile" that enables building and testing of a C executable called leftBit. The Makefile contains a test target as well as a "leftBit" target to build the leftBit executable, and a "clean" target to remove the leftBit executable.

## Working the lab

You will need to edit the leftBit.c file and write the code for the leftBit function. For this lab, you are not allowed to use the simple test-each-bit version of the leftBit function described above. You must figure out how to work on a range of bits.

Before you get started, think about one special condition... if all bits in x are zero. In that case, what is the leftmost 1 bit?  There **is** no leftmost 1 bit. Let's handle that case up front. If x is zero, return a -1 to indicate that no bits are a 1. From now on, we can assume that at least one bit in x is a 1.

Next, I need to keep track of a range of bits, which is defined by the rightmost bit position of the range, and the width of the range. Note that "bit positions" in integers are defined from the right side of the integer... the rightmost bit is bit 0, then bit 1, and so on, all the way up to bit 31 (which in a signed integer is the sign bit.)

I used two integer variables to keep track of the range... `n` to keep track of the rightmost bit position of the range, and `w` to keep track of the width (or size) of the range. To start with, I initialized `n` to 0, and `w` to 32 to indicate that my leftmost 1 bit might be *any* bit in x. 

Next, I want to loop, cutting my range in half in each iteration of the loop until 'w' is 1... so I made a loop `while(w>1)`.  

The body of that loop needs to figure out if the left half of the range contains a 1 or not. In order to manage the left half, I made a variable called `hw` for "half width" which is set to `w/2`.  Often we would have to worry about the fact that integer division truncates remainders, but in this case, 32=2<sup>5</sup>, so there will never be a remainder until w=1.

The next step is to determine if there is a 1 bit in the left half of my current range. The first step in doing this is to make a mask (I used variable `mask`) that has ones in the left half of the range, and zeroes everywhere else. We learned how to make a string of ones in class... `(1<<hw)-1`. The resulting string of ones is right justified, and may be in the wrong place, but I can fix that by shifting them to the left by `n+hw` bit positions; the `n` to get the rightmost 1 in the mask at position `n` and the `+hw` to shift the mask to the *left* half of my current range. Thus, my mask for the left half of my range can be coded as:

```int mask=((1<<hw)-1)<<(n+hw);```

In order to get a feel for how this algorithm works, let's print the binary representation for the mask we've calculated:

```printf("M= "); printBin(mask); printf(" hw=%d n=%d\n",hw,n);```

Now that we have this mask, we can bitwise AND the mask with x to and check the result to see if it is zero or not.  If the result is non-zero, then at least one bit in x under the 1's in the mask was a one. That means that the leftmost 1 bit in x is in the left half of our original range. We can reset the range parameters by setting `n=n+hw`, and by setting `w=hw`.

If the result of the bitwise AND with the mask is zero, then none of the bits under the 1's in the mask were on. That means that the leftmost one bit must be in the right half of the range. We don't have to change n, but can set `w=hw`.

In either case, we have reduced our range by half, and can now go through another iteration of the loop.

When the loop ends, the width of the range will be 1, which means n now contains the location of the leftmost 1 bit in x!

Here is an example of the output from this function when everything is coded correctly:

```
>./leftBit 321
X= 0000 0000 0000 0000 0000 0001 0100 0001
M= 1111 1111 1111 1111 0000 0000 0000 0000  hw=16 n=0
M= 0000 0000 0000 0000 1111 1111 0000 0000  hw=8 n=0
M= 0000 0000 0000 0000 1111 0000 0000 0000  hw=4 n=8
M= 0000 0000 0000 0000 0000 1100 0000 0000  hw=2 n=8
M= 0000 0000 0000 0000 0000 0010 0000 0000  hw=1 n=8
The leftmost bit of 321 is at position 8
```

## Submitting your Lab

When you are finished editing and testing your code, make sure to use the following git commands while in your lab01-*userid* directory:

```
git add leftBit.c
git commit -a -m 'Coded leftBit function`
git push
git rev-parse HEAD
```

The `git rev-parse HEAD` will print the hash code associated with the commit. Cut and paste that hashCode into the Lab 01 submission area in the Lab Submissions folder under Content for CS-220 in myCourses. You may submit as many times as you wish up to the deadline of midnight tonight (Jan. 29), only the last submission will be graded.

## Grading your Submission

If you turn in your code late, 4 points will be deducted, unless you have received an extension from the professor. Any submissions after Thursday will receive a zero grade.

Missing or lack of attempt to solve the problem will result in a zero grade.

If your code does not compile, or has an obvious bug, you will get a 3 point deduction for each bug. Bugs include things like not returning the correct leftmost bit, or not printing the correcte mask. We will not look for more than two bugs, so at most, 6 points will be deducted for bugs.

There will be a 1 point deduction for each class of compiler warning generated by your code. 
