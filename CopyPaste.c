#include <stdio.h>

void myfunct(char *str){
   printf("%s\n",str);
}

 int main()
{
	int x = 7;
	int i = 0;
	x = x + 5;
	x = x + 5;
	printf("X = (12?) %d\n ",x);
	x = x + 3;
	
	printf("X = (15?) %d\n ",x);
	printf("X = (15?) %d\n ",x);
	printf("X = (15?) %d\n ",x);
	printf("X = (11?) %d\n ",x);
	
	char *str = "Aloha";
	printf("hello, world\n");
	printf("hello, world\n");
	myfunct(str);
//	printf("goodbye, world\n");
	myfunct(str);
        for(i=2; i <6 ; i++) {
		printf ("Hello LLVM+CLANG!");
        }	
	if (x==3) {
		x = 8;
	} 
	return (x);
}
