# Lab04 - Dynamic Linked List

We have been learning about run time memory allocation, including how to allocate memory for structures, and keep track of that memory using a pointer to a structure. One aspect we haven't talked about are *self-referential* structures... structures which contain, as a field, a reference to another structure of the same type. It turns out that self-referential structures are extremely useful in order to make various different data structures. Today we will use a self-referential structure to make a simple data structure called a *linked list*.

A simple linked list is a data structure that is built up from nodes. Each node has a *payload* - the data that the node carries, and a pointer to the next node.  If the pointer to the next node is `NULL`, then there is no next node... such a node is the last node in the list, or the *tail* node.  One other important node in the list - a node that no other node in the list points to - is the *head* node of the list. We can traverse the entire list by making our "current" node the head node, and as long as the current node's next pointer is not null, go to the next node.

In C, we can implement a single node of a linked list with a simple integer payload using the following structure definition:

```
struct llnode {
  int payload;
  struct llnode * next;
};
```

This is a self-referential structure because the `next` pointer refers to another instance of the same structure. Notice that C bends the rules a little bit to allow self-referential structure. Ordinarilly in C, everything must be declared or defined before it can be used. For instance, a variable must be declared before it can be used in an expression, or a function must be defined before you can invoke that function.  In a self-referential structure, part of the definition of the structure refers to the structure. In a sense we are referencing the structure before we have finished definiting the structure. The C language explicitly allows such a "violation" of the define before use rule just so that we can make self-referential structures.

One reason to use a linked list is that a linked list is easy to grow and shrink.  All we have to do is manipulate a couple of next pointers, and we're done. One alternative to a linked list is something like an array.  To grow an array, we need to allocate a bigger array, and then copy ALL the old values into the new memory. Another reason is to allow us to insert a value in the middle of the list. If I want to put something in an array in between array\[3\] and array\[4\], I have to move array\[4\], array\[5\], and so on all the way to the end of the array. Manipulating a couple of pointers is much more efficient.

Add to the efficiency of growing, shrinking, and inserting, the capability to create new nodes using heap space, and return those nodes, and linked lists work very well in situations where it is hard to predict how many values your program needs to track.

This lab introduces one more C programming technique, which is formally called an *abstract data type*. An abstract data type is about as close as you can get in C to object oriented programming, without using C++. The basic concept is as follows.  Make a structure defniition for the "fields" of the object you are interested in, and keep this data structure "private".  We won't publish the names of the fields. We will also put the definition of the data structure and all of the functions that deal with the internals of that data structure in a single C file.  This file acts like a "class" in an object oriented language, and the functions act like "methods" in that class. Associated with the class is a "header" file for the class, in a .h file. The header file will include everything that needs to be public for the class, including the definition of the data type associated with that class, and the prototypes for all the functions (e.g. public methods) associated with the class. This header file will be included in programs that "use" the class.

The real trick behind "abstract data types" is the ability in C to use a *typedef* statement that allows us to reference a structure type of a structure that has not been defined! In our example, we use:

```
typedef struct llnode * node;
```

This typedef statement basically says "type *node* is a synonym for a pointer to an instance of the llnode structure". What is especially surprising is that C allows us to create this synonym **without** specifying the definition of the llnode structure. Thus, any program that inludes this header file can use `node` as a type, but cannot reference individual fields within that structure. Only the "class" .c file has the full definition of the llnode structure and can manipulate fields in that structure. After this typedef, we can use `node` just like we use any other type in C, as the type of an argument, or as the return type from a function, so all of the "methods" can be invoked from an outside .c file, but the fields themselves are "private".

## Infrastucture given to you

When you cloned your repository, it has four files:

- **llnode.h** - The header file that defines the `node` data type, and all of the methods associated with linked lists that we will use in today's lab. You shouldn't need to edit this file today.

- **llnode.c** - The .c "class" file for linked lists.  This is the file you will be working on the most, writing the "methods" for dealing with linked lists.

- **usell.c** - An example main C function that include llnode.h, and invokes the linked list methods. This code is provided to you to save time, but much of the code is commented out because we haven't written the methods yet.  As we write new methods in llnode.c, we will uncomment this code to invoke those methods and make sure they work.

- **Makefile** - Our standard make file to build and test an executable called "usell".

Note that the main function in usell.c will use command line arguments as data to put into linked lists. The number of command line arguments will be the number of elements in our linked list, and we will run atoi on these arguments (we expect them to be numbers) to get the payload for each node in the list.

## Working the Lab

For this lab, we will be writing the "methods" for the llnode "class".  The prototypes for the methods are already defined in llnode.h, but the following instructions give more detail on how to write the functions that implement these "methods".

### The newNode method

The first method we need is a `newNode` method.  The `newNode` function takes an integer argument called `value` which will become the payload of the new node we will create.  Use `malloc` to obtain memory for the new node, and cast the resulting pointer to data type `node`. Then, set the payload of the node based on the argument, and set the next pointer to `NULL`. Finally, return the new node we have created (really a pointer to an instance of the llnode structure, but we are calling these pointers "node"s.)

### The addToHead method

Now that we can create new nodes, we need the capability to add these nodes into a list. The addToHead method does just this by taking a list (or really the node that is at the head of the list) as an argument, and a new value. The addToHead method needs to make a new node from the value argument, and add that new node at the front of the list (before the current "head" node.) This is really simple. If our new node is node `n`, then all we have to do is connect the next pointer of node `n` to the previous head of the list. By convention, the addToHead function returns the new head node of the list, which is now the `n` node.

### The printList mothod

Now that we can create a list, it would be nice to be able to print out that list to make sure we got things right. We will print a list by first printing the address of the head of the list, and then printing an arrow (->) followed by the payload of each node. To start with, print the address of the head of the list using:

```
  printf("Head@%p ",head);
```

Notice, we are using a "%p" format string in our printf statement.  The %p format string formats pointers with a hexadecimal printout like: "0x6000005d0".

Next we need a loop to go through the list. Since C is call by value, the "head" parameter is a local copy of the node, so we can update it's value without worrying about affecting our caller. In other words... if we re-assign "head" within this printList function, it won't change the head of the list for our caller. This allows us to use "head" as the pointer to the current node in the list we are working on.  That makes the loop very simple...

```
  while(head!=NULL) {
    printf(" -> %d",head->payload);
		head = head->next;
	}
	printf("\n");
```

### The listSize method

Just to check to see if things are correct, let's also write a function to count the number of items in a list.  This turns out to be a very simple function if we think recursively.  The main idea is that the size of an empty list is zero.  Then, the size of a non-empty list is just 1 more than the length of the sub-list that is pointed to by `head->next`. The recursive code for this function is just:

```
int listSize(node head) {
  if (head==NULL) return 0;
	return 1+listSize(head->next);
}
```

### The freeList method

Now that we have created nodes, made a list, and printed that list, we also need a function to free all the nodes in the list. It turns out that this is very simple to do if we think recursively. The basic concept is that we get the head of the list as a parameter. If that head is NULL, we're done... there is no list to free.  If not, then we first free the sub-list that the head pointer is pointing to (which may be NULL), and then we free the head node, and we're done. In code, here's how that looks:

```
  if (head==NULL) return;
	freeList(head->next);
	free(head);
```

### Testing newNode, addToHead, printList, listSize, and freeList

Once you have coded and compiled the implementation of these five methods, look at the usell.c file. This file has four large block comments. Inside each block comment is some code which uses the linked list "methods" we have been working on.  The first block, labeled 

```
/* Test newNode, addToHead, printList, and freeList
```

This block encloses the code which tests the methods we have just finished.  Uncommment this code and run make. Did you get what you expected? Notice that if your code is working correctly, it will reverse the order of the arguments, acting like a "Last In / First Out" (or LIFO) queue.

### The addToTail method

The addToHead method created a linked list by adding each new node to the head of the list. One alternative is to add each node to the tail of the list. I'll leave the code up to you, but remember, the head passed in may be a NULL pointer, indicating that the current list is empty.  In this case, you should return the new node as the new head of the list.  In all other cases, the head of the list won't change, so you need to return the current head of the list. (That means you can't modify `head`... if you do, you won't know what the orignial head of the list was.) There are a couple of different strategies... either code this recursively, or code a loop that walks through the list until it gets to the current tail.

When have coded and compiled the addToTail method, uncomment the section of code in usell.c labeled:

```
/* Test addToTail 
```

And run make to see what happens.  Notice that when we add to the tail, our linked list acts like a "First in / First Out" or "FIFO" queue.

### The addIncreasing method

One final list creation method.  The addIncreasing method should add a new node to the list in such a way that the list ends up in numeric order. The value parameter should result in a new node, and that node should be added into the list in a position such that the next point points to a node with a higher value payload. If the list starting at head is empty, head will be null, and the new node will become the new head. If the list starting at head is not empty, but the payload at the head node is greater than the `value` parameter, then the new node should become the head of the list, and point to the old head. Otherwise, the new node will be inserted somewhere later in the list... possibly even at the ltail of the list.  Again, this can be implemented either with a loop, or using a recursive call.

After this compiles, uncomment the final block of code in usell.c, labeled with:

```
/* Test addIncreasing
```

Notice the resulting list is now sorted in numerically increasing order.

### Running valgrind

One final check... make sure you are free-ing everything you malloced by running this code under valgrind.  This can be done using the command:

```
valgrind --leak-check=full usell 56 42 29 51 29 42 3
```

Look at the results of valgrind, and if there are any problems, you should fix these problems.

## Submitting your Lab

When you are finished debugging your code, make sure to commit and push your repository using git. Cut and paste the resulting hashCode into the Lab 04 submission area in the Lab Submissions folder under Content for CS-220 in myCourses. You may submit as many times as you wish up to the deadline of midnight tonight (Feb. 19), only the last submission will be graded.

## Grading your Submission

If you turn in your code late, 4 points will be deducted, unless you have received an extension from the professor. Any submissions after Thursday will receive a zero grade.

Missing or lack of attempt to solve the problem will result in a zero grade.

If your code does not compile, or has an obvious bug, you will get a 3 point deduction for each bug. Bugs include things like lists ending up in the wrong order, or lots of extra debug prints occuring in your output. 

We will run `./usell` with some unpublished command line parameters in your repository, and two points will be deducted for each incorrect response.

We will also run valgrind on your code, and deduct three points if valgrind reports any errors.
