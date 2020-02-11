# Lab06 - Debugging Wumpus

Today, we will work on more debug techniques, but to make things a little more interesting, we will work on a computer game I derived from one of the old vintage computer games called "Hunt the Wumpus". (See https://en.wikipedia.org/wiki/Hunt_the_Wumpus for more detail.)

But there's a new challenge today. Today, I am **not** going to give you the source code for wumpus. (I will publish the source code when I publish the solution, because I think it's interesting, but for the purposes of this lab, no source.) That has one major implication. I have compiled wumpus to run on the LDAP machines (either the machines in G-7, or remote.cs.binghamton.edu.) The binary executable will not run on other platforms. That means you **have** to do today's lab in the lab, or if you don't finish in the lab, using ssh or PuTTy to get to the remote.cs.binghamton.edu servers. This is a little less flexible than previous labs and homework, but get used to it; as we go deeper and deeper into the architecture, the more we need to restict the hardware we run on.

## Files provided

Your lab06 repository comes, as usual, with several files:

- **wumpus** - This is the wumpus game executable file (the result of gcc -g -Wall -o wumpus wumpus.c). See below for details on how to run this code.

- **input.txt** - an empty file that you can use to save wumpus directives. You will need to edit this file so that when you submit, this contains all the directives required to win the wumpus game.

- **gdb_cmds.txt** - A file to hold gdb commands used to debug wumpus.

- **Makefile** - A standard make file, but since you don't have the source, it will not build the wumpus executable - however, there are useful test and gdb targets.

## Playing the Wumpus Game

The Wumpus game is a maze-solving game. You start at a room at the beginning of the maze, the game tells you a little bit about where you are, and asks you where to go next.  Your choices are to move to the room to the left, right, forward or back, signified by letters "l", "r", "f", or "b". The maze is fairly dangerous. If you move outside the range of the maze, you fall and die. Several of the rooms contain lethal traps, and if you walk into that room, you fall into the trap. Furthermore, there is a "wumpus" in the maze as well, wandering randomly through the maze. If the wumpus ends up in the same room as you are, the wumpus eats you, and the game is over. The object is to figure out how to navigate through the maze and get to the final goal.

Please resist the temptation at this point to go through a trial and error process of solving the game. You can probably do that, but hopefully we will show some gdb techniques that make the process a little less tedious, as well as get you some good experience using gdb.

You will find that it is fairly difficult to get through the maze succesfully, especially using just trial and error techniques. Furthermore, it's a pain in the neck to have to restart at the beginning, and retrace your steps up to the point where you made a mistake last time. For that reason, the wumpus game has a special feature... rather than having to retype all your choice letters (l, r, f, or b), the wumpus game allows you to specify the name of a file as the first (and only) command line paramter. For this lab, we will use **input.txt** to keep track of our choices. The wumpus game will read choices (one letter per line) out of the input.txt file until it hits the end of file for input.txt. At that point, if more choices are required, wumpus will read from the terminal. Thus, you can put choices in the input.txt file that will get you up to a certain point in the maze. You can then make choices manually to progress further. If you find a promising next choice, add that to your input.txt file to "remember" that choice.

## Working the Lab

If you had access to the wumpus.c code, you could just read that code and figure out how to get through the maze successfully. Even though I haven't given you the code, I have compiled the code with the `-g` option which tells the compiler to keep cross-reference information in the executable file. Can't we just run gdb, and use the `list` command to print out the code? Try that by running `make gdb`. Note that the gdb_cmds.txt file already has a "break main" and a "run" command in it. Every program must have a "main" function, so "break main" will always work in gdb, stopping before we run the first instruction in the main function. 

Notice that when we hit the breakpoint in main, we get a message `wumpus.c: No such fiel or directory`, and usually we print the line of code... but now gdb just tells us we are at wumpus.c:56 (line 56 of file wumpus.c), but not what the instruction is that is about to get executed! If we try the gdb `list` command at this point, all gdb tells us is that we in wumpus.c. In fact, you can run the gdb command `info source` to get more information about the source code for wumpus. It will even tell you where the wumpus.c file was when it was compiled, but you don't have access to that file!

What this highlights is the fact that the cross-reference information kept in the wumpus executable includes what file the source was in, and what line numbers in that file are, but the cross-reference information does *not* include the source code itself. The gdb debugger normally reads the source file to figure out what the source code looks like, but if gdb doesn't have access to the source, it can't print out the actual source code.

However, the cross-reference information kept in the wumpus executable with the -g option *does* include a list of function names and variable names. You can get a list of all the function names in the wumpus executable with the gdb command `info functions`.  Try it.

You will find that the list of functions goes on for pages and pages because the wumpus executable includes not only code from wumpus.c, but also from all the C libraries that are used by wumpus.c. (And these libraries invoke lower level libraries, and so on. It's amazing how many library functions are sucked in to a simple C program!)

At this point, I'm going to give you a hint to make things go faster. You will notice that every time you get into trouble in the wumpus game, your player character gets killed. Is it possible that there is some function in wumpus.c that gets called every time you die? If so, would the name of the function maybe have the verb "kill" in it?  The gdb `info functions` command accepts a third argument which can be used to filter the results by using "regular expression" matching techniques. Regular expressions are very useful, but off-topic at this point.  Just try the command `info functions kill` to see if you can find any functions that contain the word "kill" in the name.

There is a system library function called `killpg`, but that is clearly not what we are looking for.  The only other candidate is in file wumpus.c, and is called `killPlayer`. That looks more interesting! Add a breakpoint for the `killPlayer` function, and add commands to that breakpoint to invoke the gdb `where` command to see what the call stack looks like when the killPlayer function is enterred. Save gdb_cmds.txt, and rerun `make gdb`. Now continue past main, and start playing the game. When you make a wrong move, you will stop at killPlayer and see where you were called from.

Notice that in the call stack, there are several levels of functions. The last one listed is `main`, and main has called function `getMove`.  Notice also that `getMove` has two parameters, one called "player" and the other called "wumpus", both of which have values that are addresses. These are probably interesting. Use the gdb `up` command until you are in the context of the `getMove` function. Now you can use commands like `print player` to print the value of player. But all you see is an adress in memory.  To find out what this is pointing to, use the command `print *player`. Or, to find out what type of variable `player` is, try the gdb command `ptype player` to print the type of player.

Clearly, we can see that the wumpus game keeps track of both the player and the wumpus by using a two-dimensional array, and as long as we know the row and column number in that array, we know where the player (and the wumpus) are. It would be nice to be able to keep track of where we are, so add the following to your gdb_cmds.txt file:

```
break getMove
commands
silent
print *player
print *wumpus
continue
end
```

This will cause gdb to print the location of the player and the location of the wumpus every time you make a move.

Use this information to navigate through the maze.  You might want to keep track of what kind of room is at each row and column you run into. If you run into a trap, write that down so you will know to avoid that trap next time. Keep track of succesful moves by adding them to input.txt. If you are running under gdb, and you get to the killPlayer function, edit input.txt as appropriate, and then restart using the gdb `run` command. It should be much easier to navigate through the maze and eventually reach the end.

Good luck, and happy wumpus hunting.

When you find a path to the end of the maze, make sure the entire path is in input.txt. If the entire path is correct, you should be able to run make test and get to the end of the maze every time. (Actually, the wumpus takes a random path, so it's possible the wumpus will get you, but if you rerun, since the random number generator is reseeded every time, you should get through.)

## Submitting your Code

When you are finished debugging your code, make sure to commit and push your repository using git. Cut and paste the resulting hashCode into the Lab 06 submission area in the Lab Submissions folder under Content for CS-220 in myCourses. You may submit as many times as you wish up to the deadline of midnight tonight (Mar. 05), only the last submission will be graded.


## Grading your Submission

You will receive full credit for this lab if we can run `make test` and get to the end of the maze successfully. (If the wumpus gets you, we will rerun a couple of times to make sure your path can be successful.) If you reach the end of the maze, you get the full 10 points. If you are not succesful, but your input.txt file is not empty, five points will be deducted from your score. (If input.txt is empty, you didn't even try, so get zero points.)

