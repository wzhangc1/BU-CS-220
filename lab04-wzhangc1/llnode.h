#ifndef LLNODE_H
#define LLNODE_H
typedef struct llnode* node;

node newNode(int value);
node addToHead(node head,int value);
	// Returns the new head node
node addToTail(node head,int value);
	// Returns a new head node if list was empty
node addIncreasing(node head,int value);
	// adds a node with value before node with higher value in list that starts at head
	// May put value at head of list, so return head
int listSize(node head);
void printList(node head);
void freeList(node head);
#endif