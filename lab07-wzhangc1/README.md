# Lab07 - Encode/Decode Revisited

# Background

Last week's test had a programming problem that dealt with encoding some clear text into a secret format, that, with the proper key, can be decoded back into clear text. So many students struggled with this problem that it makes sense to revisit the problem, give you lots more background on what is going on, and give you another chance to solve the problem.

On the test, the problem was stated as follows:

  Consider the program in what was printed on the tearoff page on the test, but is contained in **encode.c** in this repository. If this code is compiled as executable file `encode`, then the command

  ```echo "This is a test" | ./encode 3```

  ... would (possibly) write the following string to stdout... ```Thbilps jidgs rajx tceuyste```

  Write a C program (including the main function) which takes a single number as the command line argument, and decodes the encoded result as specified by the encode function above. For instance, the command...

  ```echo "This is a test" | ./encode 3 | ./decode 3```

  ...would write the string ```This is a test``` to stdout.
  
This question highlighted several concepts, some of which we have not covered in great detail in this class, but which did appear on the practice test. In this lab, we will give you more detail about these concepts.

## Command Line Arguments

Command line arguments are the values specified after the command name when we invoke a command. For instance, when we invoke ```./encode 3```, there are two command line arguments; the first is ```.\encode``` and the second is ```3```. When UNIX processes a command, it reads the command line and divides the command line up into multiple arguments. This is done by looking at the "white space" (blanks, tabs, new lines, etc.) associated with the command line. Every time UNIX runs into one or more white space characters, it recognizes that white space as a divider between two arguments. Remember that the command itself is a command line argument!

We can override parsing on white space by quoting arguments. A quoted argument is evaluated as a single argument, even if it contains white space.  So, the command line invocation ```.\encode 3 "this is phrase 1" abc``` gets evaluated in UNIX as 4 command line arguments - 0) ```.\encode```, 1) ```3```, 2) ```this is phrase 1```, and 3) ```abc```.

When UNIX runs a command that is a C program, it creates two arguments to the ```main``` function of that program. The first argument is the number of command line arguments on the command line, usually referred to as ```argc```. The second is a list of strings (where each string is a null-delimited list of characters) that contain the values of the command line arguments, usually referred to as ```argv```. Remember, ```argc``` is a count of the number of arguments, so when we refer to the actual argument values, we use indexes, such as ```argv[0]``` to reference the first command line argument (which is the commnad name itself.). We can use ```argv[i]``` to talk about the i<sup>th</sup> command line argument, where ```i``` can be any value between 0 and ```argc-1```.

It is usually bad practice to try to use a specific command line argument without first checking to make sure that the user has specified enough command line arguments. C programs very often have code such as:

```
if (argc<3) {
  printf("Please invoke as %s <parm1> <parm2>\n",argv[0]);
  return 1;
}
```

This code checks to make sure the user has specified two parameters other than the command itself. If the user has not specified at lest two parameters, ```argc``` will be less than 3, the message will print out, and the main function will end.

Notice that the ```main``` function in **encode.c** does not perform such a check. (I left it out because I am assuming the user will always specify two command line parameters when running the encode command, and I wanted to save space and keep the code as simple as possible.)

If you are ever confused about command line parameters, compile and run the **cmdParms.c** program included in this repository. This program simply prints out each command line parameter specified when the command was run.

Please be careful and do not confuse the concept of command line parameters with standard input and standard output streams. These are two totally different concepts. Command line arguments are the list of values specified on the command line itself.

## Input and Output Streams

In C, you can read or write from something outside your program (usually a file) using *streams*.  A stream is simply a list of ASCII characters. The C standard library, as defined in the library header file **stdio.h**, has several functions which read characters from an input stream, or write characters to an output stream.  Often we connect a file to an input stream. When we do this, the contents of the file are fed into the input side of the stream, and the program can invoke standard library functions to consume data from that stream - in other words, to read the contents of that file. 

We can also use an output stream in a C program, and connect a file to that output stream. The program then invokes standard library functions to write characters to that stream, and those characters end up being consumed by putting them into the file - in other words, we write from the program to the file.

It's easiest to think of streams connected to a file, but streams may also be connected to devices connected to your computer. For instance, an input stream may be connected to the keyboard on your computer.  When your program consumes this input stream, it waits for you to type something on your keyboard (and hit enter to send those characters to the input stream.) Output streams may be connected to your terminal window. That way, when your program writes to an output stream, the results will show up on your screen.

We've been using this standard library stream function all along, without describing it in detail.  The ```printf``` function is one of those functions that connects your program to an output stream, and that ouput stream is connected to the terminal. When we run ```printf``` we see the results in the terminal window on our screen.

## Standard Input and Output

When UNIX runs a C program, it automatically creates three streams for you. One of these streams, called *standard input* is an input stream your program can use to read from. By default, the standard input stream is connected to the keyboard on your computer, so if you read from the standard input stream, you are expecting your user to type something on the keyboard. The second stream is called *standard output*, and is connected, by default, to the terminal window. The ```printf``` function always writes to this standard output stream, so the results of a printf are almost always displayed in the terminal window of your screen. The third stream is another outpu stream called *standard error*, which is also connected to your terminal window. (There are reasons for this third stream, but we'll just deal with standard input and standard output for now.)

The two simplest stdio functions are ```getchar``` and ```putchar```. The ```getchar``` function reads a single character from the standard input stream, and returns the value of that character.  The ```putchar``` function has a single argument - the letter to write to the standard output stream. We expect the returned value from ```getchar``` to be a printable ASCII character, and the library expects the argument to ```putchar``` to be a printable ASCII character.

Notice that the main function in **encode.c** uses ```getchar``` to read the clear text to be encoded from the standard input stream, and uses ```putchar``` to write the encoded version of the text to the standard output stream. Similarly, we expect a decode program in **decode.c** should read the encoded version of the text from the standard input stream, and write the decoded clear text out to the standard output stream using ```getchar``` and ```putchar```.

Try building the encode command compiled from **encode.c** provided to you in this repository, and running it as:

```./encode 3```

Notice that when you do, the keyboard opens up. The encode command has invoked ```getchar```, and is waiting to consume something from the standard input stream, which is connected to your keyboard.  Type in:

```This is a test```

... and hit enter. That will cause the characters 'T', 'h', 'i', ... to get put into the standard input stream, and will cause ```getchar``` to read those characters.  The encode command will then invoke ```putchar``` to write the encoded text to your terminal window, where it shoud show something like...

```
>./encode 3
This is a test
Thbilps jidgs rajx tceuyste
```

(Since the encode.c function uses a random function, seeded by the current time, then different fill characters will show up... but the basic structure of the result will be similar.)

Notice that encode continues to call ```getchar``` in a loop until getchar returns a value called ```EOF``` (which stands for *end of file*), so encode will still be waiting.  We can "type" and end of file character by holding down the control key (Ctrl), and simultaneously hitting the "d" key. Try it. This will send an end of file character to the encode command, end the loop, and end the program. The UNIX system will then give you a prompt for your next command, so you will see something like:

```
>./encode 3
This is a test
Thbilps jidgs rajx tceuyste
>
```

## Redirecting Standard Input and Standard Output

You may have noticed that I keep saying that standard input is connected *by default* to the keyboard, and standard output is connected *by default* to the terminal window. How can we override that default, and connect standard input or standard output to something different? The answer is that UNIX supports the concept of *rediction*, to allow us to override these defaults. We tell UNIX to redirect standad input and/or standard output by adding some special characters to the command line.

The simplest form of redirection is to connect standard input or standard output to a file. We do this by using either a less than sign (<) to redirect standard input, or a greater than sign (>) to redirect standard output. In either case, the less than sign or greater than sign is followed by the name of the file we want to connect to either standard input or standard output.  For instance, there is a file in the repository called **input.txt** that contains the characters "This is a test". We can invoke the encode command by typing 

```
./encode 3 < input.txt
```

This connects the file **input.txt** to the standard input stream of the encode command. The encode command reads the entire file (including the "end of file" character), encodes the result, and writes out something like:

```
>./encode 3 <input.txt
Thoijms xikqs macq taewystr
>
```

:warning: Note, when you use redirection, the redirection characters are **not** command line parameters. UNIX parses the command line, and realizes that the redirection should override standard input or standard output, and is not a command line argument. However, all command line arguments must come *before* any redirection.

We could leave standard input connected to the keyboard, and redirect standard output instead, using a command like:

```
./encode 3 >encoded.txt
```

If we do so, since standard input is still connected to the keyboard, the program will wait for us to type something and hit enter, and eventually to hit Ctrl-d to send an end of file character, but the results will no longer show up on the terminal window. Now, the results will be written to the file **encoded.txt** in the current directory. Note that if no encoded.txt file exists in the current directory, one will be created for us, but if encoded.txt does exist, it will be overwritten with the results of the command.  Try the above command, and type "This is a test" followed by an enter, and then a Ctlr-d.  Then type the command ```cat encoded.txt``` to see what the results look like. You should see something like:

```
>./encode 3 >encoded.txt
This is a test
>cat encoded.txt
Thjikfs piies gasm tmenhstw
>
```

We can redirect both standard input and standard output to a file at the same time.  (Just not the same file, for obvious reasons.) Try the command: ```./encode 3 <input.txt >encoded.txt```, and then cat encoded.txt to see what the results are.

One final redirection... It is possible to run one command which writes to standard output, and connect the output of the first command to the input of a second command. This is called a *pipe* in UNIX, and is represented with a vertical bar (|) between the two commands. For instance, the ```cat``` command reads a file and writes that file to standard output. We have used the cat command above to see the results in the encoded.txt file. Try the following command instead:

```
>cat input.txt | ./encode 3 >encoded.txt
```

Note that UNIX parses two command lines in this example.  The first command line is ```cat input.txt```, so for this command, ```argc=2``` and ```argv[0]="cat"``` and ```argv[1]="input.txt"```. The second command is ```encode 3```, so for this command, ```argc=2``` and ```argv[1]="./encode"``` and ```argv[1]="3"```.

Another example is the example we used in the test question. The test question uses the UNIX ```echo``` command, which writes its arguments to standard output. Thus, 

```
>echo "This is a test" | ./encode 3
```

... has two command lines. The first is ```echo "This is a test"```, so ```argc=2```, and ```argv[0]="echo"``` and ```argv[1]="This is a test"```. The second is ```./encode 3```, which is parse by UNIX as decribed above.

We could have written the test question to run two steps, as in:

```
>echo "This is a test" | ./encode 3 > encoded.txt
>cat encoded.txt | ./decode 3
```

But instead, we just made one big (three stage) pipeline by stringing all these commands together to make:

```
>echo "This is a test" | ./encode 3 | ./decode 3
```

If we code our decode function properly, we should then see:

```
>echo "This is a test" | ./encode 3 | ./decode 3
This is a test
>
```

## Running the Lab

With all of this background, solving the problem on the test should be much easier.  Notice that the encode program works by inserting extra, random, alphabetic characters in between the clear text characters of the input text. Specifically, if the first command line argument is "3", then encode will insert 0 characters between the first and second input, 1 between the second and third, and 2 between the 3rd and 4th.  Then encode will recycle, and insert 0 between the 4th and 5th characters, 1 between the 5th and 6th, and 2 between the 6th and 7th... and so on.

Our decode function needs to remove these characters from the input.  But since decode is reading input using getchar, and writing output using putchar, the trick is to invoke getchar to consume both the real characters and the random characters, but *not* to invoke putchar on those random characters... only invoke putchar on the real characters.

With this big hint, and with all the above background so you can understand what is going on, can you now write the decode.c function so that it will correctly decode text encoded by the encode function?

## Submitting your Code

When you are finished writing and debugging your decode.c code, make sure to commit and push your repository using git. Cut and paste the resulting hashCode into the Lab 07 submission area in the Lab Submissions folder under Content for CS-220 in myCourses. You may submit as many times as you wish up to the deadline of midnight tonight (Mar. 12), only the last submission will be graded.


## Grading your Submission

You will receive full credit for this lab if we can run `make test` and compile your code without warnings, and decode text encoded by encode correctly (using the same command line argument for both encode and decode.) We will run a couple of different invocations with different command line argument values to make sure your decode.c code works correctly. If not, you will get up to a five point deduction, depending on how often or how badly your code fails.
