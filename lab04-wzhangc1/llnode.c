#include "llnode.h"
#include <stdio.h>
#include <stdlib.h>

struct llnode {
	int payload;
	struct llnode * next;
};

/* Add code for methods here */
node newNode(int value)
{
	node ptr = (node)calloc((8),sizeof(node));
	
	if (ptr != NULL)
	{
		ptr->payload = value;
		
		ptr->next = NULL;
		
		return ptr;
	}
	else
	{
		return NULL;
	}
}

node addToHead(node head,int value)
{
	node ptr = newNode(value);
	
	if (head != NULL)
	{
		ptr->next = head;
	}
	
	return ptr;
}

node addToTail(node head,int value)
{
	node prev = NULL;
	node ptr = head;
	
	if (head != NULL)
	{
		while (ptr != NULL)
		{
			prev = ptr;
			ptr = ptr->next;
		}
		
		node ptr = newNode(value);
		
		prev->next = ptr;
	}
	else
	{
		head = newNode(value);
	}
	
	return head;
}

node addIncreasing(node head,int value)
{
	node prev = NULL;
	node ptr = head;
	
	if (head != NULL)
	{
		while (ptr->payload < value && ptr != NULL)
		{
			prev = ptr;
			ptr = ptr->next;
		}
		
		if (prev != NULL)
		{
			node ptr = newNode(value);
			ptr->next = prev->next;
			prev->next = ptr;
		}
		else
		{
			head = addToHead(head, value);
		}
	}
	else
	{
		head = newNode(value);
	}
	
	return head;
}

int listSize(node head)
{
	if (head==NULL) 
	{
		return 0;
	}
	else
	{
		return 1+listSize(head->next);
	}
}


void printList(node head)
{
	printf("Head@%p ",head);
	
	while(head!=NULL)
	{
		printf(" -> %d",head->payload);
		head = head->next;
	}
	
	printf("\n");
}

void freeList(node head)
{
	if (head==NULL)
	{
		return;
	}
	
	freeList(head->next);
	
	free(head);
}
