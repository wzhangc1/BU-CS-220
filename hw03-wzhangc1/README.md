# HW03 - Counting words with trees

## Concepts

Many of the concepts in this homework are very similar to those in lab-04, and this description assumes that you have already been through lab04. (For those of you trying to get a head start, sorry about the confusion.)

As in lab04, we will make an abstract data type to encapsulate a data structure. In lab04, we made a linked list. In this assignment, we will make a binary tree. A binary tree is a data structure that has a node (like the linked list), but instead of a "next" pointer, the tree has two pointers to other tree nodes - a left branch and a right branch.  We think of these branches as being either below and to the left of the current node, or below and to the right of the current node. We call this a *binary* tree because each node has at most two sub-nodes. Binary trees are upside down from biological trees. The "root" of the tree is at the top, and the "leaves" are at the bottom.  We typically start with the node designated as the root of the tree. The root is the top of the data structure, and all we need to know about the tree is the root. The root contains a left and right pointer. These pointers point may be NULL, indicating there is no left or right branch of the tree.  If the left or right pointer are not null, we think of the node that it points to as the root of a sub-tree. Eventually we get to nodes where both the left and right pointers are NULL. These are called the "leaves" of the tree.

Trees are really easy to program if you think recursively.  Most often, all we need to do is know how to handle NULL trees, how to handle the payload at the node itself, and how to recursively call for the left and right sub-trees. (The only subtelty is what order to process, left sub-tree, payload, or right subtree first.)

Your job for this homework is to write the "methods" to implement a binary tree. The infrastructure I provide has a main function which uses the command line arguments as file names, reads each of these files, tokenizes the lines of each file into words, and invokes binary tree methods to add those words to a binary tree. Then `manin` invokes the methods to print the tree and free it before returning.

The payload of each node in the tree consists of a tokenized word, and a count - the number of times that word appears in the input files.

## Files provided to you

As in lab04, I am providing several files:

- **countWords.c** - This is the C code with the `main` function provided for you. You do not need to modify this file.

- **btree.h** - This is the header file that contains the definition of the `node` data type, and the prototypes for the functions (methods) that work with/on binary trees. You do not need to modify this file.

- **btree.c** - This is the C code that implements the functions that work on binary trees. You will need to edit this file to add the code for the binary tree methods.

- **Makefile** - Standard build, test, and clean targets for the countWords binary

- **franz_kafka.txt** - A test input file that contains a quote from Franz Kafka

- **claude_shannon.txt** - A test input file that contains a quote from Claude Shannon - the inventor of "Information Theory".

- **linus_torvalds.txt** - A test input file with a couple of quotes from Linus Torvalds - the main coder of the Linux operating system.

## Defining the Binary Tree Methods

To complete this homework, you need to define the methods declared in btree.h, in file **btree.c** using the treeNode structure defined in btree.c. Details on the specific methods, and hints on how to implement these methods follow.

### `node newNode(char * word)`

The `newNode` method should malloc space for a new instance of the treeNode structure, and initialize the payload by copying the word argument into newly malloced space (the memory that the word argument is pointing to will change, so we can't just use the pointer passed in to us... we really need to copy it.) It turns out that there is a library function called `strdup` that will take a null-delimited string as an argument, malloc space for a new copy of that string, and copy the value for us, all in one function call. Using this function, if our new node is `n`, then we can just:

```
n->payload=strdup(word);
```

We need to remember to free the memory reserved for the payload when we free the tree, but that comes later.

The `newNode` function should set the `count` for this node to 1; this is the first time we have seen this word. The function should also set the left and right pointers to NULL.

Finally, the `newNode` function should return the new node that has been created.

### `void addTree(node root,char * word)`

The `addTree` function takes two arguments - the root of an existing tree, and a word to add into that tree. This function should assume that the root is not NULL, and may want to `assert(root!=NULL);` just to enforce that assumption. 

The addTree function should first check to see if the argument word matches the payload of the root node. It can do this by running the library function `strcmp(char *str1,char *str2)`.  The strcmp function returns an integer value; 0 if `str1` matches `str2`, <0 if `str1` comes *before* `str2` in alphabetical order, and >0 if `str1` comes *after* `str2` in alphabetical order. 

If the argument `word` is equal to the payload of the root, the `count` of the root should be incremented by 1, and `addTree` should return.

If the argument comes alphabetically *before* the payload of the root, then we need to look at the left branch of the root.  If the left branch is NULL, we just make a new node from the argument `word`, and make that our new left branch of the root.  If the left branch is *not* NULL, then we invoke addTree recursively to add the argument `word` to the left sub-tree of the root.

If the argument comes alphabetically *after* the payload of the root, then we need to look at the right branch of the root. If the right branch is NULL, make a new node from `word` and make that the right branch of the root. If the right branch is not NULL, invoke addTree recursively to add the word to the right sub-tree of the root.

By adding words to the tree in this way, we ensure that everything in the left sub-tree of the root comes before the root in alphabetical order, and everyting in the right sub-tree of the root comes after the root in alphabetical order. This means that on average, when we compare the argument to the root value, we either match right away, or we divide the number of nodes we have to look at roughly in half. The same holds true for all the sub-trees, so on average, if there are `n` words in our tree, we need to look at about log<sub>2</sub>(n) nodes until we find the proper place to put the current word. This turns out to be an extremely efficient way to keep a dictionary of words in alphabetical order.

### `void printTree(node root)`

When we print the tree, we want to print out each word and its count in alphabetical order. That means we always want to print out the left sub-tree first, then the current node, then the right sub-tree. If we do this recursively, it's really easy...

First, if the root is null, return right away.

Secondly, invoke printTree on the left branch of the root. (If the left branch is null, printTree will return right away... no harm done. If its not null, we will print out all the words in the left sub-tree in alphabetical order with our recursive call.)

Third, we will print out the payload of the root node. This can be done with:

```
printf("%4d : %s\n",root->count,root->payload);
```

Finally, invoke printTree on the right branch of the root.

### `void freeTree(node root)`

The freeTree function again is simplest if coded recursively.  If the root is NULL, just return.  If you didn't return, the root must be non-null, so first free the left branch of the tree using a recursive call, then free the right sub-tree of the tree using a recursive call, then free the payload by invoking ```free(root->payload)``` (remember, we had to copy that value when we made the new node.), and finally free the node itself.

## Testing your Code

Run make until your code runs ands tests succesfully. Then try your code with the other test files provided for you. Try specifying multiple commadn line arguments. Does the order of the arguments make any difference in the output? Run under valgrind to make sure we are freeing everything that we malloced.

## Submitting your Homework

When you are finished editing and testing your code, make sure to commit and push your repository using git. Then, do a git rev-parse HEAD from the project directory to get the hash code associated with the commit. Cut and paste that hashCode into the HW03 submission area in the Homework Submissions folder under Content for CS-220 in myCourses. You may submit as many times as you wish up to the deadline of midnight Feb. 25, only the last submission will be graded.

## Grading your Submission

If you turn in your code late, 4 points will be deducted, unless you have received an extension from the professor. Any submissions after Wednesday, Feb 27 will receive a zero grade.

Missing or lack of attempt to solve the problem will result in a zero grade.

If your code does not compile, or has an obvious bug, you will get a 3 point deduction for each bug. Bugs include things like results not in alphabetical order, incorrect counts, or valgrind errors.

Your code will be run with two input files (probably different than those distributed to you), and up to six points will be deducted for incorrect responses (incorrect words or counts.)

There will be a 1 point deduction for each class of compiler warning generated by your code.
