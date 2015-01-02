/* main.c
* Jeff DiPaola
* CS 214 - Assignment 2
*/

#include    <stdio.h>
#include    <stdlib.h>
#include	<string.h>
#include    <stdint.h>
#include	"sorted-list.h"

int compareInts(void *p1, void *p2)
{
int i1 = *(int*)p1;
int i2 = *(int*)p2;

return i1 - i2;
}

int compareDoubles(void *p1, void *p2)
{
double d1 = *(double*)p1;
double d2 = *(double*)p2;

return (d1 < d2) ? -1 : ((d1 > d2) ? 1 : 0);
}

int compareStrings(void *p1, void *p2)
{
char *s1 = p1;
char *s2 = p2;

return strcmp(s1, s2);
}

//Destructor functions
void destroyBasicTypeAlloc(void *p)
{
 //For pointers to basic data types (int*, char*, double*,...)
 //Use for allocated memory (malloc, calloc, etc.)
 
 free(p);
}

void destroyBasicTypeNoAlloc(void *p)
{
 //For pointers to basic data types (int*, char*, double*,...)
 //Use for memory that has not been allocated (e.g., "int x = 5;SLInsert(mylist,&x);SLRemove(mylist,&x);")
 return;
}

int main()
{
//////////////////////////
//Test Case 1: Integers//
////////////////////////
SortedListPtr joe = SLCreate(compareInts, destroyBasicTypeAlloc);
int intins[7] = {4, 1, 5, 2, 3, 8, 10};
int i;
for(i = 0; i < 7; i++){
		int *num= (int*)malloc(sizeof(int));
        *num = intins[i];
        if(SLInsert(joe,num) == 0)
			printf("Error in Test Case 1 Insert\n");
}
/*
*num = 4;
SLInsert(joe, num);
*num = 1;
SLInsert(joe, num);
*num = 5;
SLInsert(joe, num);
*num = 2;
SLInsert(joe, num);
*num = 3;
SLInsert(joe, num);
*num = 8;
SLInsert(joe, num);
*num = 10;
SLInsert(joe, num);
*/


//print the list
printf("Test Case 1: Integers\n");
SortedListIteratorPtr current = SLCreateIterator(joe);
void *item;
item = SLNextItem(current);
while(item != NULL){
               printf("%i ", *((int*)item));
               item = SLNextItem(current);
}
printf("\n");

//second part of test case 1
printf("Removing the 4:\n");
int remove = 4;
SLRemove(joe, &remove);

//print the list again
current = SLCreateIterator(joe);
item = SLNextItem(current);
while(item != NULL){
               printf("%i ", *((int*)item));
               item = SLNextItem(current);
}
printf("\n\n\n");
SLDestroy(joe);




/////////////////////////
//Test Case 2: Doubles//
///////////////////////
SortedListPtr bob = SLCreate(compareDoubles, destroyBasicTypeAlloc);
double doubins[7] = {4.28, 4.29, 1.7, 2.0, 3.9, 8.1, 10.2};
for(i = 0; i < 7; i++){
		double *num2= (double*)malloc(sizeof(double));
        *num2 = doubins[i];
        if(SLInsert(bob,num2) == 0)
			printf("Error in Test Case 2 Insert\n");
}

//print the list
printf("Test Case 2: Doubles\n");
current = SLCreateIterator(bob);
item = SLNextItem(current);
while(item != NULL){
               printf("%f ", *((double*)item));
               item = SLNextItem(current);
}
printf("\n");

//second part of test case 2
printf("Removing the 4.29\n");
double remove2 = 4.29;
SLRemove(bob, &remove2);

//print the list again
current = SLCreateIterator(bob);
item = SLNextItem(current);
while(item != NULL){
               printf("%f ", *((double*)item));
               item = SLNextItem(current);
}
printf("\n\n\n");
SLDestroy(bob);




////////////////////////////
//Test Case 3: Characters//
//////////////////////////
SortedListPtr fred = SLCreate(compareStrings, destroyBasicTypeAlloc);
char charins[7] = {'c', 'r', 'q', 'a', 'z', 'd', 'e'};
for(i = 0; i < 7; i++){
	char *ch = (char*)malloc(sizeof(char));
	*ch = charins[i];
	if(SLInsert(fred,ch) == 0)
		printf("Error in Test Case 3 Insert\n");
}

//print the list
printf("Test Case 3: Characters\n");
current = SLCreateIterator(fred);
item = SLNextItem(current);
while(item != NULL){
	printf("%c ", *((char*)item));
	item = SLNextItem(current);
}
printf("\n");

//second part of test case 3
printf("Remove non existent f:\n");
char remove3 = 'f';
SLRemove(fred, &remove3);

//print the list again
current = SLCreateIterator(fred);
item = SLNextItem(current);
while(item != NULL){
	printf("%c ", *((char*)item));
	item = SLNextItem(current);
}
printf("\n\n\n");
SLDestroy(fred);



//////////////////////////////////
//Test Case 4: Ordered Integers//
////////////////////////////////
SortedListPtr sally = SLCreate(compareInts, destroyBasicTypeAlloc);
char moreints[6] = {1, 2, 3, 4, 5, 6};
for(i = 0; i < 6; i++){
	int *num= (int*)malloc(sizeof(int));
	*num = moreints[i];
	if(SLInsert(sally,num) == 0)
		printf("Error in Test Case 4 Insert\n");
}

//print the list
printf("Test Case 4: Ordered Integers\n");
current = SLCreateIterator(sally);
item = SLNextItem(current);
while(item != NULL){ 
	printf("%i ", *((int*)item));
	item = SLNextItem(current);
}
printf("\n");

//second part of test case 4
printf("Remove 6:\n");
remove = 1;
SLRemove(sally, &remove);

//print the list again
current = SLCreateIterator(sally);
item = SLNextItem(current);
while(item != NULL){
	printf("%i ", *((int*)item));
	item = SLNextItem(current);
}
printf("\n\n\n");
SLDestroy(sally);



///////////////////////
//Test Case 5: Words//
/////////////////////
SortedListPtr ali = SLCreate(compareStrings, destroyBasicTypeAlloc);
char *words[4] = {"cat", "dog", "bird", "spider"};
for(i = 0; i < 4; i++){
	char *word = (char*)malloc(strlen(words[i])+1);
	word = words[i];
	if(SLInsert(ali, word) == 0)
		printf("Error in Test Case 5 Insert\n");
}

//print the list
printf("Test Case 5: Words\n");
current = SLCreateIterator(ali);
item = SLNextItem(current);
while(item != NULL){
	printf("%s ", ((char*)item));
	item = SLNextItem(current);
}
printf("\n\n\n");



/////////////////////////////////////
//Test Case 6: Duplicate Insertion//
///////////////////////////////////
printf("Test Case 6: Duplicate Insertion\n");
SortedListPtr mike = SLCreate(compareInts, destroyBasicTypeAlloc);
int inserttwo[4] = {1, 2, 3, 1};
for(i = 0; i < 4; i++){
	int *num= (int*)malloc(sizeof(int));
    *num = inserttwo[i];
    if(SLInsert(mike,num) == 0)
		printf("Error in Test Case 6 Insert\n");
}
printf("\n\n\n");
SLDestroy(mike);


//Print Ending Statement
printf("End of test cases\n");
fflush(0);

return 0;
}
