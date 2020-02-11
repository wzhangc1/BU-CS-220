#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct treeNode {
	char * payload;
	int count;
	struct treeNode * left;
	struct treeNode * right;
};

/* Insert your code for the btree methods here */

node newNode(char * word)
{
	node ptr = (node)calloc((4),sizeof(node));
	
	if (ptr != NULL)
	{
		ptr->payload=strdup(word);
		ptr->count = 1;
		ptr->left = NULL;
		ptr->right = NULL;
		
		return ptr;
	}
	else
	{
		return NULL;
	}
}

void addTree(node root,char * word)
{
	assert(root!=NULL);
	
	if (strcmp(root->payload, word) == 0)
	{
		root->count++;
		
		return;
	}
	else if (strcmp(root->payload, word) > 0)
	{
		if (root->left != NULL)
		{
			addTree(root->left, word);
		}
		else
		{
			node ptr = newNode(word);
			
			root->left = ptr;
		}
	}
	else
	{
		if (root->right != NULL)
		{
			addTree(root->right, word);
		}
		else
		{
			node ptr = newNode(word);
			
			root->right = ptr;
		}
	}
}

void printTree(node root)
{
	if (root != NULL)
	{
		printTree(root->left);
		
		printf("%4d : %s\n",root->count,root->payload);
		
		printTree(root->right);
	}
}

void freeTree(node root)
{
	if (root != NULL)
	{
		freeTree(root->left);
		freeTree(root->right);
		
		free(root->payload);
		free(root);
	}
}
