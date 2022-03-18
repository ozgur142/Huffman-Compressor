#include <stdio.h>
#include <stdlib.h>

void tets(int* a){
	*a = 5;
}

int main(int argc, char const *argv[])
{
	/*int a = 1;
	printf("a = %d\n", a);
	tets(&a);
	printf("a = %d\n", a);*/

	int* Tree = (int*)malloc(8* sizeof(int));
   int a[8] = {2, 3 ,4, 5, 19, 20, 21, 22};
   for (int i = 0; i < 8; ++i)
   {
      Tree[i] = a[i];
   }
	int tmp = 12;
	int* tmpTree = (int*)malloc(7* sizeof(int));;
	int taille = 8;

	for (int i = 2; i < taille; ++i){		
		if (tmp > Tree[i])	
			tmpTree[i-2] = Tree[i];
		else {
			tmpTree[i-2] = tmp;
			for (int j = i; j < taille; ++j)
				tmpTree[j-1] = Tree[j];
			break;
		}
		
	}
   Tree = (int*)realloc(Tree, sizeof(int) * 7);

   for (int i = 0; i < 7; ++i)
   {
      Tree[i] = tmpTree[i];
   }

	for (int i = 0; i < 8; ++i)
	{
		printf("%d : %d \n", i, Tree[i]);
	}




}
