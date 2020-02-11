# Lab12 - Rock/Paper/Scissors Tournament

## BackGround - The Ancient Game

![rockPaper](https://github.com/Binghamton-CS140-B1-Fall-2018/images/blob/master/rockpaper.jpg) 

Many of us have played the child's game of "Rock/Paper/Scissors". For complete details and a history of the game, see [Wikipedia Rock-paper-scissors](https://en.wikipedia.org/wiki/Rock%E2%80%93paper%E2%80%93scissors). The basics are as follows... for each round, each player chooses either rock, paper, or scissors. If both players choose the same object, the round is a draw - nobody wins. If one player chooses rock, and the other scissors, the rock breaks the scissors, and the rock player wins. Scissors cuts paper, and paper wraps up rock, so scissors beats paper, and paper beats rock.

A game consists of multiple rounds - in our case, a game will consist of 50 rounds. The reason to play multiple rounds is to try to determine your opponent's strategy, and come up with a strategy that will consistently beat your opponent.

We will each code a C file that contains a function to play a rock-paper-scissors game. A game playing infrastructure will be provided, along with some very simple player strategies. You will be able to code your function and test it during the lab. When you are satisfied with your strategy, you commit your player function to your repository. On Wednesday, we will conduct a tournament - each player against every other player - to determine who coded the winning-est function in your section. We will also run an all-time tournament against students who took CS-220 in previous semesters to find an all-time champion.

## Files Provided

Your lab12 repository will contain several files, as follows:

- **round.c** - A C file that contains a main function that runs a single round of the rock/paper/scissors game. The command line arguments to this file specify the two registered players to play against each other.

- **match.c** - A C file that contains a main function that runs an entire 50 round match of the rock/paper/scissors game. The command line arguments to this file specifiy the two registered players to play against each other.

- **tournament.c** - A C file that contains a main function that runs a double-elimination tournament using an entire list of registered players as participants in the tournament. This file conducts matches between the players until it selects a single winner - a player that either beat all others, or lost once, but on every other player lost twice.

- **rps.h** and **rps.c** - A header file and C file that contains basic rock/paper/scissors functions.  You may wish to use the eval_round function, which takes two rps values as the first two arguments. Leave the second two arguments NULL to avoid printing extra messages. The return value is of type "winner", which is an enumerated type that can have the value `Player1`, `Player2`, or `Tie`. For debug, you might also wish to use the fmt_rps function which takes the rps enumerated type, and returns a string for that type, either "Rock", "Paper", "Scissors" or "Unknown".

- **pool.h** and **pool.c** - A header and C file that contain functions to deal with pools of players (to support a tournament). You should not need to use any of these functions.

- **player_rock.c**, **player_scissors.c**, **player_paper.c**, and **player_random.c** - Four very basic rock/paper/scissors player functions. For instance, `player_rock` returns a `Rock` value every time it is called.

- **players.h** - A header file to define all of the players to be included in a tournament. This file is included in tournament.c, and contains the four basic players "rock", "paper", "scissors" and "random" (see the player_rock.c, etc files) as well as the contents of the **student.h** file.  The **student.h** file is built by the Makefile if it does not already exist.

- **Makefile** - A make comment make file to build and run all of the rock/paper/scissors executables. This file defines some global variables (something we haven't seen in Makefiles before, but it should be fairly evident what is going on), as well as several targets, as follows:

  - `test` - Runs a tournament by invoking the `tourney` executable, and saves the results in a file called **tourney.txt**
  
  - `testMatch` - Runs a test match between the rock player and the scissors player using the `match` executable.
  
  - `testRound` - Runs a round between the rock player and the scissors player using the `round` executable
  
  - `round` - Builds the round executable, which takes two registered player names as arguments, and runs a single rock/paper/scissors round between those two players.
  
  - `match` - Builds the match executable, which takes two registered player names as arguments, and runs a 50 round match between those two players.
  
  - `tourney` - Builds a tournament executable, which uses the current values in **players.h** to determine the list of uses in the tournament.
  
  - `student.h` - Creates a new file called **student.h** which includes a single player from file "player_<user>.c", where <user> is your LDAP user id.
  
  - `player_<user>.c` - Creates an empty new file called "player_<user>.c", where <user> is your LDAP user id.  This is the file you will need to edit for this lab to put your rock/paper/scissors player code in.
  
  - `clean` - Removed all the files created by this Makefile (except player_<user>.c so it won't clobber your edits to that file.)

## Getting Started

Run `make testRound` to build the simplest code. You should see the invocation of the ./round executable with parameters rock and scissors.

The code for the rock player is in player_rock.c. The code for the scissors player is in player_scissors.c. If you look at the player_rock.c file, you will see two things... First, a function called player_rock which takes some arguments, and returns a data type called rps. (You will find the definition of rps in rps.h.) This function returns the choice that the player will make. The rock player is really dumb. It returns Rock every time, no matter what.

The second thing in the player_rock.c file is an invocation of a function called register_player. This function takes two argument... first the name of the function that returns an rps value (in this case, player_rock), and second, the name that will be used to identify this player in a game, in this case "rock".

The main function in round.c does a `#include players.h` to suck in the code for all possible players. Then it uses the names specified as arguments to invoke the player function for the first player to find out what it chose, followed by the player function for the second player. It then figures out who the winner was.

Run `./round scissors paper` to run a round between a player that always chooses scissors and a player that always chooses paper.

Run `./round rock rock` to see what happens for a tie.

Run `./round rock random` several times. Take a look at player_random.c to see how it works.

Run `make testMatch` to run an entire match of 50 rounds between a player that always chooses scissors and a player that always chooses a rock.

A match consists of 50 rounds. To start off, the bet on the middle of the table is worth 1 point. If a player wins the round, he gets the bet, the loser gets nothing, and a new bet of 1 point is put on the table. If the players tie, the bet is doubled. After fifty rounds, the player with the most points wins the match. Note that it is possible to tie in a match, but not likely. The most points you can get in a match is when the first 49 rounds are a tie, and you win the last round. In this scenario, you get 249 points, and your opponent gets zero!

Run `./match rock random` several times. Do the results surprise you at all?

## The "rps" Data Type

In this lab, we define a type named `rps` which can have one of four values... Rock, Paper, Scissors, or Unknown.

A variable of type `rps` must have one of those four values. For instance, you can declare a variable like `rps guess;`, and then assign a value like `guess=Paper;`, or check the value of guess as in `if (guess==Scissors)...` The rps type is defined in file rps.h.

There is also function called `fmt_rps` declared in rps.h, and defined in rps.c which returns the character string that describes an rps value. This can be used to print an rps value. For instance...

```
rps choice;
...
printf("My rock, paper, scissors choice is: %s\n",fmt_rps(choice));
```

## Player Functions

A player function is a function which will play the rock-paper-scissors game. Each player function must return a rock paper scissors choice. A single round of rock-paper-scissors consists of invoking two different player functions, comparing the choice of the fist player with the choice of the second player, and figuring out which player won the round. If both players chose the same rps value, for instance, if both chose Rock, then the round is a tie, and neither player wins.

Four player functions have been provided to you, **player_rock.c**, **player_scissors.c**, **player_paper.c**, and **player_random.c**. 
In general, each player must have the same function prototype... namely:

```
rps player_<XXX>(int round, rps * myhist, rps * opphist) 
```

where <XXX> is a unique player ID.

Player functions get more interesting during match play. The three arguments to the player function become more useful.

The first argument is an integer that tells the player function which round is currently being played. The first time the player function is called, the round will be round 0. Then round 1, round 2, and so on, up to round 49.

The second argument is a pointer to a vector of the rps values this player function chose for previous rounds, starting with round 0. C allows you to interpret this pointer as either a pointer to a list of rps values, or as an array of rps values. Therefore, if I wanted to check if I chose "Rock" in round 3, I could use either the following code: ```if (myhist[2]==Rock) ...``` Or, I could use lists and pointer arithmetic to do the same thing: ```if (*(myhist+2)==Rock) ...```

The third argument is a pointer to the choices my opponent player function made in previous rounds, which again can be used either as a pointer or an array.

Remember, both myhist and opphist do not contain the choices for the current or future rounds, and are only valid for index 0 to `round-1`.

Hint: If you want to remember something between calls to your player function, you must declare a variable as static. For instance, if I want to keep track of the number of times I chose "Rock" without looking at myhist, I could define `n_rock` as a static variable and use it as follows:

```
static int n_rock=0;
...
if (next_rps==Rock) n_rock++;
...
return next_rps;
```

You may wish to determine, for previous rounds, which player function "won" the round in your player function. You may use the "eval_round" function declared in rps.h and defined in rps.c. The eval_round function takes four arguments; the first two are the rps choice for player 1 and player 2 respectively. The last two arguments are pointers to the names of player1 and player2 respectively. If these last two arguments are NULL, the "eval_round" function does not print any messages. If the name arguments are not NULL, then eval_round will print a message saying who was playing, what their choices were, and who won. In either case, the eval_round returns a value of the "winner" enumerated type (as defined in rps.h), which can have values of either Player1, Player2, or Tie. If you want to specify non-null third and fourth arguments while debugging feel free to do so, but please null out these arguments before submitting your code so your code doesn't print debug information out when it is run in a tournament.

## Player Registration

Our rock-paper-scissors infrastructure contains a list of all players, a "player registry". Each entry in the registry consists of a player name, and a pointer to a player function. In order to play a round, a match, or participate in a tournament, a player must first be registered. Registration occurs by invoking the "register_player" function declared in rps.h and defined in rps.c. The register_player function takes two arguments

- `player p` : The "player" data type is defined in rps.h as a pointer to a player function with a signature that correctly matches the signature of a player function. In our case, after you define your player function, all you need to do is put the name of that function into the first argument of the call to register_player. Note that your player function must have a different name than any other player function name. Therefore, please use your LDAP userId as your player name. That way, we can be sure no one will have the same player name. (Note... if you want to submit more than one player, use your LDAP userid as a prefix, and add a unique suffix, like "player_tbarten1_try2tie" and "player_tbarten1_winEmAll".)

- `char *p_name` : The name of the player, to be used in all messages and when choosing players. To keep things straight, the easiest way to manage this is to use your LDAP userId as above. As an example, if my LDAP userId is "tbarten1", I would code a player function called "player_tbarten1", and invoke register_player(player_tbarten1,"tbarten1");.


## Writing your Player Code

Now it's time to actually make some player code. Here are the steps...

1. Edit your player_<userid>.c file.
2. Copy the lines from player_rock.c into your player_<userid>.c file
3. Change the name of the player function in the file from player_rock to player_<userid>, for instance, player_tbarten1.
4. Change the arguments in the invocation of "register_player" from `register_player(player_rock,"rock")` to `register_player(player_<userid>,"<userid>");`. For instance, `register_player(player_tbarten1,"tbarten1");`.
5. Figure out a strategy for playing a rock-paper-scissors match, and replace the code "return Rock;" with a C implementation of that strategy. Your strategy can use myhist and opphist, but it should not return a random result. There must be some logic to determine what choice to make.

Your function is not allowed to change the random number seed or any values in the myhist and opphist arrays.

Note: Your C file will be included inside the rock-paper-scissor code, so you will not need any "#include" statements in your file unless you are using some very special library functions.

You may print debug information from your player function while you are writing and testing it, but when you are finished, *please remove all the printf statements or invocations of functions that cause the player function to print something out*. Since we are going to run you against hundreds of other students, extra debug info is a humongous waste of disk space.

It is considered a violation of tournament rules to modify or override any functions used by the infrastructure. Students in the past have tried to cheat by creating their own version of the eval_round function, or even coding their own rand function. This will be checked and disallowed.

The Makefile has already created a file called student.h in your repository that includes your player_<userid>.c file. The student.h file is included in the round, match, and tournament executable automatically. If you want to make a second player function, you will need to edit student.h and add an include for the second function.
  
When you invoke the round executable, the arguments you need to specify are the registered names of the two players you want to play against each other. So, for instance, to run a round of the rock player against the scissors player, you would run the command ./round rock scissors. Using the example above, to run a round of the "tbarten1" player against the random player, you would run the command 

`./round tbarten1 random`

The match executable has the same conventions as the round executable... it takes two registered player names as arguments, and plays a match between those two players. For instance, to play a 50 round match between the scissors player and mu player, I would use the command 

`./match scissors tbarten1`

You may want to change the Makefile `testRound` or `testMatch` rules to play your code agains player_random or player_rock to debug your player. To do this, edit the recipe from `./match rock scissors` to something like `./match rock tbarten1`.


## Running a Tournament

The tourney executable does not take arguments. It runs a tournament among all registered players by running a match between pairs of players. By default, a tournament will run between all the registered players, which are rock, paper, scissors, random, and the player function you code.

The tournament is a double-elimination tournament. That means all players start out in the "winners bracket". A tier of the tournament consists of pairing all players in the winners bracket to play against each other, and all players in the losing bracket are paired against each other. If you are in the winners bracket and win your match, you stay in the winners bracket. If you are in the winners bracket and lose a match, you move to the losers bracket. If you are in the losers bracket and win a match, you stay in the losers bracket. If you are in the losers bracket and lose a match, you are eliminated from the tournament. If two players tie, one of those players is randomly chosen as the "winner", and the other becomes the "loser". This is required because otherwise, tournaments might never end.

If there are an odd number of players in a bracket for a specific tier, one randomly picked player gets a "bye" and is assumed to have "won" his match.

Eventually, the tournament narrows down to one of two situations. The first is a single player in the winners bracket and a single player in the losers bracket. In this case, a final match is played against these two players to determine the tournament winner and champion. The other situation is if there are no players left in the winner's bracket, and exactly two players left in the loser's bracket. In this case, the two players play against each other and the winner of that match becomes the tournament champion.

## Submitting your Player

When you are satisfied with your player function, commit your player_<userid>.c file, push the results, and paste the resulting hash code on myCourses.

The professor will then create and run two tournaments during Wednesday's lecture. The first tournament will be a tournament between all players in your section. The second tournament will be the grand tournament between all students from both sections, as well as students from previous semesters. 

## Grading your Results

This lab submission is worth 10 points, and in general, you get 10 points for submitting any viable player function. Deductions will occur for the following reasons:

- If you submit late without an extension, there will be a three point deduction for every 24 hours you are late. Extensions are available only in special circumstances, and can only be given by the instructor or a TA. Furthermore, you will not be included in the tournaments.
- There will be a three point deduction for submissions that do not follow the required format, or violate the tournament rules (e.g. reseed the random number generator, or override existing infrastructure functions.)
- There will be a four point deduction if there are compiler errors when compiling round, match, or tourney.
- There will be a three point deduction if your code does not run to completion during an execution of round, match, or tourney.
- There will be a two point deduction for each class of compiler warning message issued when compiling your code.
- There will be a two point deduction if your function prints out any extra messages during execution.

All deductions will be canceled if you reach the final round of either the section tournament, or the grand tournament.
