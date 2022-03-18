#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tree.h"

#define ASCII 256

void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}

typedef struct Dict Dict;
struct Dict
{
	char character;
	int occurrence;
};

void initDictionnary(Dict* T){
	for (int i = 0; i < ASCII; ++i)
	{
		T[i].character = i;
		T[i].occurrence = 0;
	}
}


//quick sort
void sortOccurrence(Dict* T, int first, int last){
	int i, j, pivot, tempO, tempC;
   	if(first<last){
    	pivot=first;
    	i=first;
     	j=last;
      	while(i<j){
         	while(T[i].occurrence <= T[pivot].occurrence && i<last)
         		i++;
        	while(T[j].occurrence > T[pivot].occurrence)
         		j--;
        	if(i<j){
            	tempO = T[i].occurrence;
            	T[i].occurrence = T[j].occurrence;
            	T[j].occurrence = tempO;

            	tempC= T[i].character;
            	T[i].character = T[j].character;
            	T[j].character = tempC;
         	}
      	}
      	tempO = T[pivot].occurrence;
      	T[pivot].occurrence = T[j].occurrence;
      	T[j].occurrence = tempO;

      	tempC = T[pivot].character;
	   	T[pivot].character = T[j].character;
	    T[j].character = tempC;

	    sortOccurrence(T,first,j-1);
	    sortOccurrence(T,j+1,last);
   }
}

// calculation of the number of occurrences of each letter and return length of file
int readFileOccur(char* name, Dict* T){
	FILE* file;
	file = fopen(name, "r");

	if (!file){ // file == NULL
		perror("Error: ");
		exit(1);
	}

	initDictionnary(T);
	int c;
	int length = 0; //length of file or number of caracter in file
	while (EOF != (c=fgetc(file))){
        T[c].occurrence++;
        length++;
    }
    fclose(file);

    return length;
}


int initTree(node** Tree, Dict* T){
	int nbrCharacter = 0;
	for (int i = 0; i < ASCII; ++i)
	{
		if (T[i].occurrence != 0)
		{
			Tree[nbrCharacter] = new_node(T[i].occurrence, T[i].character);
			nbrCharacter++;
		}
	}
	return nbrCharacter;
}

void insert(node* tmp,node** Tree,int* taille){
	node** tmpTree = (node**)malloc(sizeof(node) * (*taille-1));

	for (int i = 2; i < *taille; ++i){		
		if (tmp->occurrence > Tree[i]->occurrence)	
			tmpTree[i-2] = Tree[i];
		else {
			tmpTree[i-2] = tmp;
			for (int j = i; j < *taille; ++j)
				tmpTree[j-1] = Tree[j];
			break;
		}
	}
	free(tmp);
	//free(Tree);
	*taille -= 1;
	//Tree =  (node**)malloc(sizeof(node) * (*taille));
	// copying
	for (int i = 0; i < *taille; ++i)
		Tree[i] = tmpTree[i];
}


node* Huff(node** Tree, int taille, const int length){
	node* tmp = (node*)malloc(sizeof(node));
	const char internalNode = '$';

	//taille != 1
	while(Tree[taille-1]->occurrence != length){ // because at the end last internal node size will be the length of the text
		tmp = new_node((Tree[0]->occurrence + Tree[1]->occurrence), internalNode);
		tmp->left =  Tree[0];
		tmp->right = Tree[1];
		insert(tmp, Tree, &taille);
	}
	return tmp;
}

void initCodes(codes* code, node* root, int arr[], int top, int* cpt){
	if (root->left) {
		arr[top] = 0;
		initCodes(code, root->left, arr, top + 1, cpt);
	}
	if (root->right) {
		arr[top] = 1;
		initCodes(code, root->right, arr, top + 1, cpt);
	}
	if (isLeaf(root)) {
		code[*cpt].size = top;
		code[*cpt].car = root->letter;
		for (int i = 0; i < top; ++i)
			code[*cpt].code[i] = arr[i];
		*cpt += 1;
	}
}

void show(){

}


int main(int argc, char *argv[])
{
	if (argc != 3){
		printf("Wrong usage:   %s nameFileSource.* nameFileCompressed.*  !!!\n", argv[0]);
		exit(1);
	}
	clock_t t1;
	clear();
	printf("Compression started... \n");

	t1 = clock();
	Dict Occur[ASCII]; // char(ascii) : occurance    	
	char* fileName = argv[1];
	const int length = readFileOccur(fileName, Occur);

	sortOccurrence(Occur, 0, ASCII);

	node** Tree = (node**)malloc(sizeof(node) * ASCII);
	const int nbrCharacter = initTree(Tree, Occur);

	node *HuffTree = Huff(Tree, nbrCharacter, length);

	codes binary[nbrCharacter]; // stocking every char and their new binary code
	int cpt = 0;
	int arr[MAX_TREE_HT];
	initCodes(binary, HuffTree, arr, 0, &cpt);




	for (int i = 0; i < nbrCharacter; ++i)
	{
		printf("%c : ", binary[i].car);
		for (int j = 0; j < binary[i].size; ++j)
		{
			printf("%d", binary[i].code[i]);
		}
		printf("\n");
	}

	

//********** Test ****/
	for (int i = 0; i < 256; ++i)
	{
		if (Occur[i].occurrence != 0)
		{
			printf("%c : %d \n", Occur[i].character, Occur[i].occurrence);
		}
	}

	
	printf("*********************************************\n\n");

	for (int i = 0; i < nbrCharacter; ++i)
	{
		printf("%c : %d \n", Tree[i]->letter, Tree[i]->occurrence);

	}
//************** fin de test ****/
	t1 = clock() - t1;
	printf("Compression terminated. \n");
	printf("Time taken by compression: %f seconds.\n", ((double)t1) / CLOCKS_PER_SEC);
	printf("Some informations: \n");

	
	return 0;
}