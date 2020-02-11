typedef struct treeNode* node;
node newNode(char * word);
void addTree(node root,char * word);
void printTree(node root);
void freeTree(node root);