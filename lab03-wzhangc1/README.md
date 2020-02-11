# Lab03 - Deep dive into gdb

Today, we are going to investigate some of the features of the Gnu Debugger, gdb. I'm not going to ask you to write any C code in this lab... just write up some gdb command files, mostly to learn about some of the many features of gdb, many of which will save you lots of time and effort while working on other labs, homework, and projects.

To demonstrate gdb, we are going to use some code which implements a highly recursive function called the *Ackermann* function. (Actually, the variant we are using is officially the Ackermann-Peter function, which is a simplified variant of Wilhem Ackermann's original function.) The Ackermann function does not serve any practical purpose other than illustrate a "total computable function" (a function we can prove will always return an answer, given infinite resources) which is not "primitive recursive". (For a complete definition of "primitive recursive", see https://en.wikipedia.org/wiki/Primitive_recursive_function.)

For our purposes, here is the definition of the Ackermann function we will use:
```
Ack(m,n)={
  n+1 if m=0,
  Ack(m-1,1) if m>0 and n=0,
  Ack(m-1,Ack(m,n-1)) if m>0 and n>0
}
```
... where m and n are non-negative integers.

The most important feature of the Ackermann function is that it grows *very* quickly. For example, A(4,2) is an integer of 19,729 decimal digits! Clearly, even though I have implemented the Ackermann function using long (64 bit) integers, overflow will occur very quickly. Furthermore, in order to grow so quickly, it turns out that the Ackermann function is *very* highly recursive. In order to calculate A(4,2) requires well over 19,000 recursie invocations of the Ackermann function. Each recursive call will take some amount of memory, and long before we can calculate the value of A(4,2), even if we had enough bits to hold the result, we will run out of main memory to support the recursion. For much more detail on the Ackermann function itself, see https://en.wikipedia.org/wiki/Ackermann_function.

While the Ackermann function is not very practical, it is a good mechanism to demonstrate gdb and the debugging process. We will use gdb to try to get a deeper understanding of how the Ackermann function works, as well as introducing a variety of debugging techniques supported by gdb.  This lab relies heavily on the GDB User manual, as referenced in the Supplemental Material of the class home page, as well as here: https://www.gnu.org/software/gdb/documentation/

## Files Provided to you

Included in this repository are the following files:

- **ackermann.c** - C code which implements the Ackermann function.  This code uses two command line arguments. It then invokes the `ack` function with those arguments as `m` and `n`. The `ack` function implements the Ackermann function defined above, using long (64 bit) integer values, and returns a long result. If nothing overflows or memory does not fill up, the result of the `ack` function will get printed to the terminal window.

- **Makefile** - A set of make rules to build and test the ackermann code. Note that the Makefile also has a target called `gdb` which invokes the debugger for you with various special options, discussed below.

- **gdb_cmds.txt** - A file to hold gdb start-up commands, referenced in the Makefile "gdb" target recipe.

## Debugging Techniques

To start with, run ```make test```, and make sure your function compiles and runs OK.  Notice that the test target in the Makefile does not run very complicated invocations, and all the invocations should work.  However, if you run ```./ackermann 4 1```,  things will go quiet for a while, and then, depending on the resources on the machine, you will either get a segmenation violation because recursion has used up all the memory on the computer, or you will finally get a result. (```./ackermann 4 2``` will always cause a segmentation fault, but will take a couple of minutes before it responds.)

The following list is a list of techniques we can use with gdb that demonstrate the capabilities of gdb, and give some insight about what is going on in the Ackermann function.

## Technique 1: Simple Debug

Run the ackerman function under gdb by typing the command:

```>gdb ackermann```

When the (gdb) prompt appears, type in:

```(gdb)run 4 2```

On machines with lots of memory, it will take quite a while before you get a response... possibly several minutes. If you get tired of waiting, hit "Ctrl-C" to interrupt the running program. If you were patient and wait for a response, you will eventually get something like:

```
Starting program: /home/Thomas/cs220/labs/lab03/ackermann 4 1
[New Thread 1548.0xb98]
[New Thread 1548.0x4630]

Thread 1 "ackermann" received signal SIGSEGV, Segmentation fault.
0x00000001004011a4 in ack (m=1, n=0) at ackermann.c:25
25              if (n==0) return ack(m-1,1);
(gdb)
```

In either case, to figure out where in your code you currently are, use the gdb ```where``` command. (where is a synonym for ```backtrace```, which is documented in the GDB User Manual at https://sourceware.org/gdb/current/onlinedocs/gdb/Backtrace.html#Backtrace)

After hitting enter a few times to scroll through a few screens, you get the idea... we've recursed pretty deep! In fact, there are thousands of screens of backtrace info gdb is trying to give you.  At some point, just hit 'q' to quit out of the list, and then quit gdb. We got the point... the function blew up because too much recursion was going on.

## Technique 2: Specifying command line arguments on the command line

Normally, when we run the gdb debugger, we need to specify command line args as part of the `run` command in gdb, but sometimes those arguments are long and complicated and difficult to retype all the time.  Instead, gdb has the capability to specify the entire command to be tested under gdb using the `--args` flag. When you use `--args`, it must be the last gdb option specified on the gdb command line. That allows passing flags to the program being tested. `--args` is documented in https://sourceware.org/gdb/current/onlinedocs/gdb/Mode-Options.html#Mode-Options

Reproduce the results above, but this time, start gdb with:

```
>gdb --args ./ackermann 4 2
```

When the gdb prompt appears, now you can just type `run` instead of `run 4 1`, and get the same results as above.  That doesn't seem like a big deal for this lab, but it gets more useful with more complicated command line arguments.

Don't forget to quit out of gdb before going on to the next technique.

## Technique 3: Putting Initialization Commands in a Text File

If you are running gdb on the same executable file many times, and debugging the same basic problem, then you will find that you are often restarting gdb. And if you have developed a long list of breakpoints to set, and actions to perform at start up to get prepared for debugging, you will find that it becomes pretty tedious having to retype those same start-up gdb commands over and over again. One way to avoid that problem is to put the start-up commands for gdb in a file.  

The "-x *cmdfile*" flag in gdb tells gdb to load your program, then run all the commands in the *cmdfile*, and then continue with normal debugging. See https://sourceware.org/gdb/current/onlinedocs/gdb/File-Options.html#File-Options for details on -x, and see https://sourceware.org/gdb/current/onlinedocs/gdb/Command-Files.html#Command-Files for details on what you can put in a command file. (Hint... lines that start with a hash (#) are comments, and ignored.)
  
I've created a text file in this repository, called **gdb_cmds.txt**, and put a print command in that text file to demonstrate this capability. So now try the command:

```
gdb -x gdb_cmds.txts --args ./ackermann 4 2
```

Notice that the results of the print command show up before the first gdb prompt. Otherwise, it's the same debug scenario as before.

Now, add a new line at the end of gdb_cmds.txt that says `run`, and re-run the above command.  Notice that the `run` command causes gdb to execute instructions from the ackerman binary without having to type in the `run` command on the command line.

## Technique 4: Invoking gdb from a Makefile

We've started adding lots of flags and details to our gdb command, which means there are lots of opportunities to type it wrong. One way to fix that problem is to provide a rule in the Makefile for gdb.  We will use a pseudo-target (gdb) that does not represent a file in the current directory, so the recipe will always run whenever we specify the target.  We will also create a dependency on the binary executable file, ```ackermann``` in this case, so that if the source code changed, we will recompile before running the debugger. See the Makefile delivered with the repository for an example make rule for the gdb target.  Now, we can accomplish our scenario just by typing the command:

```>make gdb```

## Technique 5: Repeating the Same GDB Command

If we want to get some idea of how the `ack` function is called every time we call it, one way to do that is to set a breakpoint at the beginning of the ack function. We can add, the following line BEFORE the `run` command in our gdb_commands.txt file:

```break ack```

And then make gdb. You will find that you have stopped at the first call to the `ack` function... and gdb is good enough to show you the parameters to the function... ack(4,2).

Now, type "continue" and hit enter. We stopped again, but this time at ack(4,1).

If we type "continue" again, we stop at ack(4,0). But I'm getting tired of typing "continue" over and over again.  (I actually don't have to type the entire command "continue".  GDB lets us abbreviate commands as long as it can still distinguish the command from all others.  I can just type "c" and hit enter.  Try it.)

Now I'm at ack(3,1)... but even typing a single letter is too much for me. (I'm really feeling lazy.)  In gdb, the convention is that if you have already typed a gdb command, then just hitting the enter key will execute that command again. Try just hitting enter with no command at all.

That should get you to ack(3,0).  Keep hitting enter a few times to see how this works, and how the parameters to the ack function change. In fact if you hold down the enter key, you will see invocations of ack scroll by on your screen.

This technique is very useful when single stepping through code in gdb as well, using the "next" or the "step" command.

There are literally thousands and thousands of invocations of ack in this debug session, so just quit out of gdb to go on to the next technique.

## Technique 6: Attaching Commands to a Breakpoint

In gdb, if we use the command `commands` immediately after setting a breakpoint with the `break` command, then everything we type after "commands" and before the word "end" (on its own line) will get executed after we reach the breakpoint, but before the next prompt occurs. See https://sourceware.org/gdb/current/onlinedocs/gdb/Break-Commands.html#Break-Commands.

Notice that in the technique 5 scenario, we want to continue after we reach the breakpoint at the beginning of the ack function.  Add the following lines in gdb_cmds.txt AFTER the `break ack` command, and BEFORE the `run` command:

```
commands
  continue
end
```

I have indented the `continue` command just to make the file more readable. The indent is not required by gdb, but gdb will skip over the indent if it's there. Save the file, and run make gdb again.

Now you don't have to continue... that's done for you. But gdb is nice enough to stop when you've filled up your screen to let you read what's there. Hit the enter key to get a next screen-full.  Hit enter a few times.... see how the ack function is called? To get out of this (it goes on for pages and pages and pages), type "q" and hit enter.  Then you can quit out of gdb.

## Technique 7: Convenience Variables

It would be nice to know how many times the ack function has been invoked while debugging the code in ackermann.c, but there is no variable in the code to keep track of that.  We could add a variable in the C code, but that defeats the purpose of having a debugger in the first place. Fortunately, gdb allows us to define new "variables" that can be manipulated by the debugger.  Convenience variables are distinguished from normal variables in C because they start with a "$" prefix. (C variables cannot start with a $, so there will never be a conflict between a C variable and a gdb convenience variable.) See also https://sourceware.org/gdb/current/onlinedocs/gdb/Convenience-Vars.html#Convenience-Vars

Convenience variables do not need to be declared, but come into existence the first time we use them. Any variable, including convenience variables and C variables can be set to a specific value in gdb with the `set variable` command. See https://sourceware.org/gdb/current/onlinedocs/gdb/Assignment.html for more detail.

Let's keep track of how many times the `ack` function is called in a convenience variables called `$nack`. We need to set it to zero to start with, so we need to add the following line to our gdb_cmds.txt file, before the break ack line:

```
set varaible $nack = 0
```

Then in the commands attached to the ack breakpoint, before the `continue` command add the following lines:

```
  set variable $nack = $nack + 1
  printf "ack function invoked %d times\n",$nack
```

Notice that gdb has a `printf` command that works almost exactly like the C printf library function!

Now run ```make gdb``` and see what gets printed.

To get some more practice, add a second convenience variable called `$rlev`. We will use this to keep track of the recursion level of the `ack` function. Just like $nack, initialize $rlev to zero at the start of the session, and increment it when we enter the `ack` function.  The difference is, when we leave the ack function, we want to decrement the $rlev variable.  In order to do this, we need to add another breakpoint at line 27. (Note that even though there are multiple "return" statements in the ack function, gdb always goes to line 27 before actually returning.)  The breakpoint at line 27 should decrement $rlev.  Add the recursion level to the printf statement as well so we can see how deep the recursion gets.

If you run at this point, you will see lots of output, too much in fact. To cut down on this amount of output, add a `silent` command after the `commands` command in both places it occurs in gdb_cmds.txt. This will turn off the gdb breakpoint message whenever a breakpoint is encounterred.

## Technique 8: Using Conditional Breakpoints

So far, we have stopped *every* time the ack function is called.  We have seen this information a lot already, learning the first 7 techniques.  What if we want to see information for just a selected set of invocations of the ack function?  We can do that with a *conditional breakpoint*. We can attach conditions to breakpoints to tell gdb, every time you reach the breakpoint, evaluate the condition. If the condition is true, do the breakpoint processing. If the condition is false, ignore the breakpoint and keep executing. See https://sourceware.org/gdb/current/onlinedocs/gdb/Conditions.html#Conditions for complete documentation.

Let's add a new conditional breakpoint after the `break ack` line and commands section...

```
break 24 if (0==$nack%100)
```

This causes a breakpoint at line 24 of file ackerman.c to get taken every time that the $nack convenience variable is evenly divisible by 100. Rember, $nack%100 gives the remainder after dividing $nack by 100.

Now lets add a commands section to our new conditional breakpoint. Move the printf command from the `break ack` commands section to the `break 24` commands, and add a `silent` and `continue` to the break 24 section.  (Note that `silent` must be the first command after the `commands` command in order to take effect.) Try a `make gdb` now.

## Technique 9: Using a Variable Conditional Limit

Our conditional breakpoint uses a fixed condition, and causes a print every 100 invocations of `ack`. Often when we are debugging, we are interested in breaking fairly regularly at first, but as we get deeper into the debug session, it's nice to break less and less often. We can implement this behavior by introducing a new convenience variable which controls how often we break (and print). And just to keep things interesting, I'm curious about how deeply nested the recursion gets, rather than how many times the `ack` function is called.

Create a new convenience variable called "$rlim" for "recurssion limit", and initialize this variable to 16.  Then change the condition on the breakpoint at line 24 to:

```
break 24 if ($rlev >= $rlim)
```

Also, in the `commands` section for the breakpoint at line 24, add the commmand:

```
  set variable $rlim = $rlim * 2
```
... and remove the `continue` command from this commands list.

Now, run `make gdb` again. When you get a prompt, continue. Notice that the time between each `continue` commad basically doubles each time. Eventually, it will get too long to wait. (The debug version runs much slower than the non-debug version because gdb is doing so much extra work for you under the covers.) I probably wouldn't suggest going more than a recursion level of about 256 or 512.

We need to mention that gdb is extremely powerful and gives us lots of flexibility to implement creative debug scenarios, but there is a performance penalty. At the beginning of this lab, we ran the ackerman function with parameters of 4 and 1, and it didn't take too long for the segmentation violation to occur.  However, now we have three breakpoints, and even though these breakpoints are both silent and (in one case) conditional, that requires gdb to take control three times for each invocation of the `ack` function. You can see, the `ack` function is invoked hundreds of thousands of times before we get the segmentation violation. With the extra debug breakpoints, that means that we'd have to wait for probably a couple of hours to get all the way to the segmentation violation. I won't ask you to do that.

## Submitting your Lab

You will get graded for today's lab based on the final version of your gdb_cmds.txt file. When you are finished debugging your code, make sure to commit and push your repository using git. Then, do a `git rev-parse HEAD` from the project directory to get the hash code associated with the commit. Cut and paste that hashCode into the Lab 03 submission area in the Lab Submissions folder under Content for CS-220 in myCourses. You may submit as many times as you wish up to the deadline of midnight tonight (Feb. 12), only the last submission will be graded.

## Grading your Submission

If you turn in your code late, 4 points will be deducted, unless you have received an extension from the professor. Any submissions after Thursday will receive a zero grade.

Missing or lack of attempt to solve the problem will result in a zero grade.

We will run `make gdb` in your repository, and expect to see a message about the recursion level (which should be 16) and the number of invocations of the `ack` function. If we continue, we should stop again with the recursion level doubled. Any deviation from this behavior will result in a deduction of up to 6 points (depending on how far off you are from the correct behavior.)
