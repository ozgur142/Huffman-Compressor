#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

#define ASCII 256

typedef struct Dict Dict;
struct Dict
{
	char character;
	int occurrance;
};



// deleting characters that not exists where (occurrance = 0)






//quick sort
void sortOccurrance(Dict* T, int first, int last){
	int i, j, pivot, tempO, tempC;
   	if(first<last){
    	pivot=first;
    	i=first;
     	j=last;
      	while(i<j){
         	while(T[i].occurrance <= T[pivot].occurrance && i<last)
         		i++;
        	while(T[j].occurrance > T[pivot].occurrance)
         		j--;
        	if(i<j){
            	tempO = T[i].occurrance;
            	T[i].occurrance = T[j].occurrance;
            	T[j].occurrance = tempO;

            	tempC= T[i].character;
            	T[i].character = T[j].character;
            	T[j].character = tempC;
         	}
      	}
      	tempO = T[pivot].occurrance;
      	T[pivot].occurrance = T[j].occurrance;
      	T[j].occurrance = tempO;

      	tempC = T[pivot].character;
	   	T[pivot].character = T[j].character;
	    T[j].character = tempC;

	    sortOccurrance(T,first,j-1);
	    sortOccurrance(T,j+1,last);
   }
}

void initDictionnary(Dict* T){
	for (int i = 0; i < ASCII; ++i)
	{
		T[i].character = i;
		T[i].occurrance = 0;
	}
}

// calculation of the number of occurrences of each letter
void readFileOccur(char* name, Dict* T){
	FILE* file;
	file = fopen(name, "r");

	if (!file){ // file == NULL
		perror("Error: ");
		exit(1);
	}

	initDictionnary(T);
	int c;
	while (EOF != (c=fgetc(file))){
        T[c].occurrance++;
    }
    fclose(file);
}

int main(int argc, char *argv[])
{
	if (argc != 2){
		printf("Wrong usage:   %s file.*  !!!\n", argv[0]);
		exit(1);
	}

	Dict Occur[ASCII]; // char(ascii) : occurance    	
	char* fileName = argv[1];
	readFileOccur(fileName, Occur);

	for (int i = 0; i < 256; ++i)
	{
		if (Occur[i].occurrance != 0)
		{
			printf("%c : %d \n", Occur[i].character, Occur[i].occurrance);
		}
	}

	sortOccurrance(Occur, 0, ASCII-1);
	printf("*********************************************\n\n");

	for (int i = 0; i < 256; ++i)
	{
		if (Occur[i].occurrance != 0)
		{
			printf("%c : %d \n", Occur[i].character, Occur[i].occurrance);
		}
	}




	
	return 0;
}