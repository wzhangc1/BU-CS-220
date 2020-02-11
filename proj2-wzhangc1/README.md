# Project 2 - Warehouse Simulations

## Project Description

For this project, we will be writing a C program to simulate a warehouse environment.  The job in the warehouse is to assemble kits to send out to customers.  Each kit is made up of many different parts.  Parts are kept in bins in the warehouse.  There are 1000 different kinds of parts in this warehouse, each of which has a part number between 0 and 999. There are 100 bins in the warehouse, bin 0 through 99, and each bin has 10 different kinds of parts. To make things easy, the parts are put sequentially into the bins, so bin 0 has part numbers 0-9, bin 1 has part numbers 10-19, and so on, up to bin 99, which has part numbers 990-999.

The parts that go into each kit are defined by orders that come into the warehouse.  The orders consist of a list of part numbers.  Each part number on an order indicates that you should take one part from the bin that holds that part, and put it in the kit.  The same part may appear multiple times in an order, but you need to put the parts into the kit in the order specified on the order.  For instance, if the order specifies “10 15 10 10 7 9 15 12”, then the kit needs to have one part number 10 from bin 1, followed by one part number 15, followed by two more part number 10’s, followed by one part number 7, and one part number 9, both from bin 0, and  one more part number 15 from bin 1, and finally part number 8 from bin 0.  Customers pay an average of 10 cents per part, so in this example kit, there are 8 parts, so the total revenue is $0.80. You may assume that there are always enough copies of a specific part number in a bin to allow you to fulfill any order.

But here is the trick.  The workbench at which you assemble the kits has exactly 5 slots, where each slot can hold a single bin. Therefore, the workbench can only hold 5 bins at a time.  It starts out with no bins, but once you have filled all five slots on the workbench, and you need a bin that is not already on the workbench, you need to send a bin that *is* on the workbench back to the warehouse and replace it with the new bin that you need.  For instance, for the above order, you first need to fetch bin 1 and put it in a slot on the workbench to fill the first order entry. Then fetch bin 0 to another slot to complete the order.  It costs 10 cents to fetch a bin, and another 5 cents to return a bin. In the above scenario, we needed to fetch two bins but with this simple order, we didn’t need to return any bins, so the total cost was $0.20 to assemble this kit, and the total revenue is only 80 cents, so the net revenue is $0.60. (This is where your salary comes from, so the higher the net profit, the more you make!)

We make money in our warehouse because most kits consist of the same parts over and over again.  In fact, the probability is very high that the next part that goes into the kit is a part that we retrieved for the kit recently. Your job is, knowing that most kits use the same parts, to figure out how to best use the slots on the workbench so that you can maximize your profit.

## Files provided to you

You have been provided with the basic infrastructure for the C code to simulate the warehouse described above. The initial repository contains the following files:

- **warehouse.c** – C source code that contains the main function which simulates the warehouse.  This is the file and function that you need to modify.  The main function, as it is delivered, performs the following tasks:

  -	includes order.h and bench.h.  This provides the prototypes for the functions in order.c and bench.c that you will need to invoke from your warehouse main code.
  - Checks the command line argument to make sure it includes the name of an order file.
  - Invokes the openOrder function to start reading the order file.
  - While there are more part numbers in the order for the kit, main will:
      - Get the next part number in the order, and figure out which bin the part is in.
      - Check to see if that bin is already in slot 0 of the work bench. If not, if there is already a bin in slot 0, return the bin, and fetch the required bin.
      - Invoke the fetchNextPart function which will get the existing part from the bin on the workbench, and put it in the kit, and then check to see if there is another part number in the order. If so, fetchNextPart will return a “1” (true) value.
   - When the order is complete, invokes the closeOrder function which will print out the total cost and revenue for this kit.

- **order.h** and **order.c** – The order.h file is a header file that contains prototypes for the functions that deal with orders for kits. The definition of those functions is in order.c.  Feel free to look over these files to see how they work, but *do not modify these files*. There are four functions of interest, all of which are invoked by the main function in warehouse.c (so you can see how they are used.)

  - `openOrder` – This function takes a character string (an array of characters) that contains the name of an order file as an argument, opens that file, and reads the first part number in the order. It returns a 1 (true) if successful, and a 0 if there is a problem.
  - `nextPartNumber` – This function returns the current part number in the order.
  - `fetchNextPart` – This function virtually picks the current part number from a bin on the bench and puts it into the kit. If the current part number is not in one of the bins on the bench, this function prints a message and halts the program. If the bin is on the bench, the function then checks to see if there is another part number in the order. If there is one, the function reads the part number and returns a 1 (true) value. If not, the function returns a 0 (false) value.
  -	`closeOrder` – This function prints a message that indicates the cost and net earnings for the kit and closes the order file.
  
- **bench.h**, **benchOrder.h**, and **bench.c** – These files contain the code to manage the workbench in the warehouse. The bench.h file contains prototypes for the functions in bench.c that you will need to use in warehouse.c.  The benchOrder.h file contains prototypes for functions in bench.c that are used in order.c, but are not needed in warehouse.c. The definitions for both sets of functions are in bench.c. Feel free to look over this code to see how things work, but *do not modify this code*. The bench.h file contains a pre-processor #define for NUMSLOTS to define the number of slots available on the workbench.  There are also prototypes for two important functions:

  - `fetchBin` – A function that takes a bin number, and a slot number (between 0 and NUMSLOTS-1) as arguments. If there is already a bin in the specified slot, that bin is returned (and the return charge of $0.05 is added to the cost of this kit). Then, the requested bin is virtually fetched (adding another $0.10 to the cost of the kit) and put into the specified slot on the work bench. This function also prints the configuration of bins on the bench each time it changes.
  - `binInSlot` – This function returns the bin number of the bin currently in the slot specified as the argument. If there is no bin in that slot, a -1 value is returned.
  
- **order1.txt**, **order2.txt**, and **order3.txt** – These are sample order files you can use to test your code.

- **Makefile** – a make file that contains several targets, as follows:

  - `test` : A pseudo-target to invoke the warehouse executable file for order1.txt, and again for order2.txt
  - `warehouse` :  Creates the warehouse executable file using both the code in warehouse.c and the code in order.c and bench.c
  - `clean `: A pseudo-target to remove the warehouse executable file.

## Working the Project

The repository, as delivered, already implements a very simple (and not very efficient) technique to manage the slots on the workbench in the warehouse. The existing code only uses slot 0, and whenver the required bin is *not* in slot 0, then the bin that is in that slot is returned, and the required bin is fetched from the warehouse. You can make and test this very simple implementation, and discover that you actually *lose* money on the three example orders provided to you if you use this strategy. In fact, this is about the worst you can do.

Your job is to modify the code in warehouse.c to implement a more efficient strategy that will make more money. The trick is to send back the bin that is *least* likely to be needed again in the near future.  If you can do so, then you can reduce the cost of kits, and decrease your cost (or increase your profit).

You may add new functions and / or variables to warehouse.c, and change the code in the main function.  The functions in order.c are intentionally designed to prevent you from looking ahead on the order list. You must fill each order entry as it arrives, before looking at the next order entry.  You may not modify the functions in bench.c or order.c (which keeps track of the cost and benefit of packing a kit.)  I have given three sample orders in the files “order1.txt”, “order2.txt” and “order3.txt”.  Your program should run with any valid order (arbitrary list of part numbers between 0 and 999), but any orders I test with will have the property that part numbers tend to be sequential, but when the sequence is not followed, there is a high probability that the next part number showed up recently on the order.

## Strategies to Think About

Even though you can't look ahead to see what part numbers are coming up in the future for a specific order, you do have information about the parts that have come up in the past.  However, you don't know how many parts are in any given order (I have not limited the number of parts in an order), so you probably won't be able to table all the previous parts in the order.  However, if you think about it for awhile, you don't need the entire previous order. You may be able to get away with keeping statistics just on the bins that are currently on the workbench. Or you may want to keep statistics on each bin (you know there are exactly 100 bins altogether, so you can table info about each bin), or even each part number (which also has a limit of 1000.)

Most important for success in this project is to use the single piece of information I have given you about part numbers yet to come in an order, namely:  part numbers tend to be sequential, but when the sequence is not followed, there is a high probability that the next part number showed up recently on the order.

## Project Sumbission

When you are finished editing and testing your code, make sure to commit and push your repository (which should contain the same set of files I provided to you) using git. Then, do a git rev-parse HEAD from the project directory to get the hash code associated with the commit. Cut and paste that hashCode into the Project 2 submission area in the Project Submissions folder under Content for CS-220 in myCourses. You may submit as many times as you wish up to the deadline of midnight on March 8, but only the last submission will be graded.

## Project Grading

Project 2 is worth a total of 100 points.  After the due date, your repository will be downloaded onto an LDAP machine, and your infrastructure files (order.c order.h, bench.c, bench.h and benchOrder.h) will be replaced by vanilla versions of these files just to make sure you haven't changed anything. Your code will then be built and run against the three published test cases (order1.txt, order2.txt, and order3.txt) as well as 2 unpublished test cases (order4.txt and order5.txt). Deductions will occur as follows:

- Your code will be compared against all other CS-220 student submissions. If you wrote your own project code, it will be different enough from other students so that there will be no problem - even if you worked as a team on the concepts. If you cut and paste your code, or if your code is cut and pasted by someone else, even if you modify variable names, comments, white space, etc. it will be considered plagiarism, and result in a zero grade.
- There will be a 10-point deduction for each 24 hours your project is late up to a maximum of 5 days. (After 5 days, you will receive a zero.)
- If you do not try to address the problem in this project, you will get a 100 point deduction.
- If your code has a compiler errors or logic errors, I will try to fix your code.  For each simple fix, I will make that fix, subtract up to 20 points (depending on how bad the bug is), and continue testing.  If there is no simple fix, or more than 3 compiler or logic errors, you will get a grade of 40 for this project.  
- Once you code compiles, 5 points will be subtracted for each type of compiler warning in your code. 
- If your code does not fill any of the published or unpublished orders, no matter what the resulting net earnings is, you will get a 15-point deduction per unfilled order.
-	Your net *cost* will be compared to the minimum possible net cost for each order (without look-ahead). If your cost is greater than the minimum, you will get a deduction of up to 10 points per order, pro-rated based on the minimum cost. (If you are close to the minimum cost, only a point or two will be deducted, but if you are far off, nine or ten points may be deducted.)

Note: This is the second of four projects, and your lowest project grade will get dropped.  So don’t despair if you don’t do well on this project… that means you will just have to work harder on projects 3, and 4.
