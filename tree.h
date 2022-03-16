#ifndef TREE_H
#define TREE_H

typedef struct node node;
struct node
{
	int occurence;
	char letter;
	node* left;
	node* right;
};

node* new_node(int data, char letter){
	node *temp = (node*)malloc(sizeof(node)); // Allocating memory to the node
	temp->occurence = data;
	temp->letter = letter;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}



int isLeaf(node* root){ 
    return !(root->left) && !(root->right); 
} 

#endif

