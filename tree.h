#ifndef TREE_H
#define TREE_H

#define MAX_TREE_HT 255

typedef struct node node;
struct node
{
	int occurrence;
	char letter;
	node* left;
	node* right;
};

node* new_node(int data, char letter){
	node *temp = (node*)malloc(sizeof(node)); // Allocating memory to the node
	temp->occurrence = data;
	temp->letter = letter;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}



int isLeaf(node* root){ 
    return !(root->left) && !(root->right); 
} 

//stocking letters and their new binary code
typedef struct codes codes;
struct codes
{
	char car;
	int size;
	int code[MAX_TREE_HT];
};


#endif

