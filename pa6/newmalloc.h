/* newmalloc.h
 * Assignment 6
 * Jeff DiPaola
 * Dec 14, 2014
 * 
 * Header file for newmalloc.c
 * Holds global variables, imports, and structure used for overall program
*/


//Includes for the program:
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> //for ceil


//Redefine functions to use my program instead of compiler defaults
#define malloc(x) mymalloc(x, __FILE__, __LINE__);
#define free(x) myfree(x, __FILE__, __LINE__);
#define calloc(x) mycalloc(x, __FILE__, __LINE__);



//Structure for memory storage, per instructions - Global Variable
char myblock[5000];



//Boolean Definition
typedef enum{false, true} bool;




/* currentmem struct - Stores location of all active memory
 * An entry will be added to the structure upon a malloc
 * An entry willl be removed from the structure upon a free
*/
typedef struct currentmem_ currentmem;
struct currentmem_{
    char *location;
    int arrayloc;
    int size;
    int next;
};
