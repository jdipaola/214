/* books.h 
 * Assignment 5
 * Jeff DiPaola
 * Nov 26, 2014
 *
 * Designed to work with books.h
 * Holds some global variables, imports, and a structure for the overall program
 * Needs reader.c to function - Reads userdb into structure
 *
*/

#include "reader.c"
#include <stdlib.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* bookorder structure
 * Designed to hold a book order (i.e. one line of the book order input file)
 * Holds all information for use in the program
*/
typedef struct bookorder_ bookorder;
struct bookorder_{
    char *title;
    double price;
    int custid;
    char *category;
    bookorder *next;
};

/***************************************
* books.h Global Variable Declarations:*
****************************************/

//Structure to hold book orders for processing
//Will be dimensioned into a 2D array of size 3 - one "bookorder" struct per consumer
//This serves as our queue
bookorder **orders;

//Will be used to tell if the producer is still running
//Necessary for the consumers (Do I need to wait or not?)
bool running = true; 

//Will hold customer database once it has been imported
List custdb; 

//Will hold the category names
char ** categories;

//Revenue holds the running total of sales
//Per the assignment instructions: "The program should also print out the total revenue gained from all sucessful book orders"
//This is incremented properly each time a successful book order is made
//It is then output twoards the end of the main function
double revenue = 0;

//Count will enact our queue sizes
//We don't want to overflow memory with too much data in the queue
//Will be maxed out at 5
int count[3];

//Mutex locks
//We don't want to cause a situation where a book order is being added while one is being taken away
//We also don't want to have two parts of the program accessing run at once
pthread_mutex_t runchk;
pthread_mutex_t orderlock;
