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
	file = fopen(name, "rb");

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
	node* tmpTree = (node*)malloc(sizeof(node));

	Tree[*taille] = tmpTree;
	*taille += 1;
	int swapped;
	for (int i = 0; i < *taille-1; ++i)
	{
		swapped = 0;
		for (int j = 0; j < *taille-i-1; ++j)
		{
			if (Tree[j]->occurrence > Tree[j+1]->occurrence)
			{
				tmpTree = Tree[j];
				Tree[j] = Tree[j+1];
				Tree[j+1] = tmpTree;
				swapped = 1;
			}
		}
		if (swapped == 0)
			break;
	}

}


node* Huff(node** Tree, int taille, const int length){
	node* tmp = (node*)malloc(sizeof(node));
	const char internalNode = '$';

	while(taille != 1){ // because at the end last internal node size will be the length of the text
		printf("%d \n", taille);
		tmp = new_node((Tree[0]->occurrence + Tree[1]->occurrence), internalNode);
		tmp->left =  Tree[0];
		tmp->right = Tree[1];
		Tree[0] = Tree[taille-1];
		Tree[1] = Tree[taille-2];
		taille = taille - 2;
		
		insert(tmp, Tree, &taille);
	
	}
	return tmp;
}

void initCodes(codes** code, node* root, int arr[], int top, int* cpt){
	if (root->left) {
		arr[top] = 0;
		initCodes(code, root->left, arr, top + 1, cpt);
	}
	if (root->right) {
		arr[top] = 1;
		initCodes(code, root->right, arr, top + 1, cpt);
	}
	if (isLeaf(root)) {
		codes* tmp = (codes*)malloc(sizeof(codes));
		tmp->size = top;
		tmp->car = root->letter;
		for (int i = 0; i < top; ++i)
			tmp->code[i] = arr[i];
		code[*cpt] = tmp;
		*cpt += 1;
	}
}

void show(const int nbrCharacter, codes** binary, Dict* T, const int length){
	printf("\nSome informations: \n");
	printf("character  :  binary(code)  :  frequency of character \n");

	for (int i = 0; i < nbrCharacter; ++i)
	{
		printf("     %c     :     ", binary[i]->car);
		for (int j = 0; j < binary[i]->size; ++j)
		{
			printf("%d", binary[i]->code[j]);
		}
		for (int j = ASCII-1; j >= 0; --j)
		{
			if (binary[i]->car == T[j].character)
				printf("      :   %%%d ",  (T[j].occurrence*100) / length);
		}
		printf("\n");
	}
}

void writeFile(char* file1, char* file2, codes** binary, int nBinary){
	FILE* fileIn;
	FILE* fileOut;
	fileIn = fopen(file1, "rb");
	fileOut = fopen(file2, "wb"); // w = write, b = binary

	if (!fileIn || !fileOut){ // file == NULL
		perror("Error: ");
		exit(1);
	}

	char c;
	while (EOF != (c=fgetc(fileIn))){
        for (int i = 0; i < nBinary; ++i)
        {
        	if (c == binary[i]->car){
        		fwrite(binary[i]->code, binary[i]->size, 1, fileOut);
        		break;
        	}
        }
    }


    fclose(fileIn);
    fclose(fileOut);
}


int main(int argc, char *argv[])
{
	if (argc != 3){
		printf("Wrong usage:   %s input.* nameFileCompressed.*  !!!\n", argv[0]);
		exit(1);
	}
	clock_t t1;
	clear();
	printf("Compression started... \n");
	t1 = clock();

	Dict Occur[ASCII]; // char(ascii) : occurance    	
	char* fileName = argv[1];
	char* fileName2 = argv[2];
	const int length = readFileOccur(fileName, Occur); // return number of file length 

	/*printf("length: %d \n", length);
	sortOccurrence(Occur, 0, ASCII);
	for (int i = 255; i > 256-length; i--)
	{
		printf("%c : %d \n", Occur[i].character, Occur[i].occurrence);
	}
	return 0;*/
	
	sortOccurrence(Occur, 0, ASCII);

	node** Tree = (node**)malloc(sizeof(node) * ASCII);
	const int nbrCharacter = initTree(Tree, Occur); // return number of different type of characters

	printf("%d \n", nbrCharacter);
	return 0;
	// ******

	node *HuffTree = Huff(Tree, nbrCharacter, length);

	codes* binary[nbrCharacter]; // stocking every char and their new binary code
	int cpt = 0;
	int arr[MAX_TREE_HT];
	initCodes(binary, HuffTree, arr, 0, &cpt);

//********************************************* TODO

	//Todo
	// dosyaya binary yazma	ve yeni dosyanın sonununun .bin ile bittiğini kontrol etme
	//writeFile(fileName, fileName2, binary, nbrCharacter);


	
	

	t1 = clock() - t1;
	printf("Compression terminated. \n");
	printf("Time taken by compression: %f seconds.\n", ((double)t1) / CLOCKS_PER_SEC);
	

	// TODO :  eski ve yeni dosyaların büyüklükeri ve sıkıştırılmadan kazanılan oran
	// tek tip karakter için ve boş dosya için güvenlik
	show(nbrCharacter, binary, Occur, length);

	
	//freeler
	
	return 0;
}
