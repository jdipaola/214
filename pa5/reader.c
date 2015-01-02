/* reader.c
 * Assignment 4 - Modified for Assignment 5
 * Jeff DiPaola
 * Nov 26, 2014
 *
 * Designed to be used with books.c
 * Function(s): Read and parse an index file into a memory structure
 *
 * Note: The Assignment 4 requirements do not make use of the file counts, that is,
 *       the amount of times each word is used in the file. Therefore, the structure
 *       created ignores this. Listing just words and the files they are contained within.
 *
 * Assumption: User will not manually input a faulty inverted index file
 *              (Index file is always generatred by the indexer)
 *
 * Modified for pa5. Changes made:
        - Data structure(s) modified to fit this program better
        - Comments editied to reflect the details of this assignment
        - Storage and handling of input data changed to reflect details of this assignment
        - Broke out part of ReadIn to be used in other places - Created FileEmptyCheck
        - Added ReadInCats - a file designed to read in the categories file
 */
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 

//Structure definitions (Will be used in books.c as well)

//Will hold list of successful book orders
typedef struct purchase_ purchase;
struct purchase_{
    char *title;
    double price;
    double credit;
    purchase *next;
};

//Will hold list of unsuccessful book orders
typedef struct rejects_ rejects;
struct rejects_{
    char *title;
    double price;
    rejects *next;
};

//List is a LinkedList holding user information for each user
//This is our primary structure.
typedef struct List_ List;
struct List_{
       int IDNum;
       double budget;
       char name[50];
       char street[100];
       char state[50];
       char zip[10];
       purchase *good;
       rejects *bad;
       List *next;
};

//Boolean Definition
typedef enum{false, true} bool;



/*
 * FileEmptyCheck - Checks if a text file is empty
 * Returns 1 if file is ok 0 if it is empty
*/
int FileEmptyCheck(char invertedindex[200]){
    
     FILE *testfile;
     testfile = fopen(invertedindex, "r");
     fseek(testfile, 0, SEEK_END);
     if(ftell(testfile) == 0){
            fclose(testfile);
            return 0;
    }
    
    fclose(testfile);
    return 1;

}




List ReadIn(char invertedindex[200]){
     
     //Declarations
     FILE *file;
     char line[1000];
     List output;
     bool first = true;
     List *current; //keeps track of what word we're on
     
     //Open file if possible, error if not
     file = fopen(invertedindex, "r");
     if(file == NULL){
          printf("Error opening customer database: %s\n", invertedindex);
          exit(0);
     }
    
    //If File is empty
    if(FileEmptyCheck(invertedindex) == 0){
            printf("Error: Customer Database %s is an empty file.\n", invertedindex);
            fclose(file);
            exit(0);
    }
     
     //Read data into structure, line by line
     while(fgets(line, 1000, file) != NULL){
            
            if(first){ //first word being added to the structure?
                char *temp;
                temp = strtok(line, "\""); //removing quotations
                strcpy(output.name, temp);
                temp = strtok(NULL, "|");
                output.IDNum = atoi(temp);
                temp = strtok(NULL, "|");
                output.budget = atof(temp);
                //address storage 
                temp = strtok(NULL, "\"");
                strcpy(output.street, temp);
                temp = strtok(NULL, "\"");
                temp = strtok(NULL, "\"");
                strcpy(output.state, temp);
                temp = strtok(NULL, "\"");
                temp = strtok(NULL, "|");
                temp[strlen(temp) - 2] = '\0';
                strcpy(output.zip, temp);
                output.next = NULL;
                output.good = malloc(sizeof(purchase));
                output.good = NULL;
                output.bad = malloc(sizeof(rejects));
                output.bad = NULL;
                current = &output;
                first = false;
            }
            else{
                List *newguy = malloc(sizeof(List));
                char *temp;
                temp = strtok(line, "\""); //removing quotations
                strcpy(newguy->name, temp);
                temp = strtok(NULL, "|");
                newguy->IDNum = atoi(temp);
                temp = strtok(NULL, "|");
                newguy->budget = atof(temp);
                //address storage
                temp = strtok(NULL, "\"");
                strcpy(newguy->street, temp);
                temp = strtok(NULL, "\"");
                temp = strtok(NULL, "\"");
                strcpy(newguy->state, temp);
                temp = strtok(NULL, "\"");
                temp = strtok(NULL, "|");
                temp[strlen(temp) - 2] = '\0';
                strcpy(newguy->zip, temp);
                newguy->next = NULL;
                current->next = newguy;
                current = newguy;
            }
              
     } //end of while(fgets)
     
     
     
     fclose(file);
     return output;
}





/*
 * ReadInCats - Created to read in the categories files
 * According to the Assignment Specs doc there will be no more than the 3 expected types
*/
int ReadInCats(char filename[200], char *output[3]){
    
    //Declarations
     FILE *file;
     char line[20];
     bool first = true;
     int count = 0; //keeps track of what word we're on
     
     //Open file if possible, error if not
     file = fopen(filename, "r");
     if(file == NULL){
          printf("Error opening inverted index: %s\n", filename);
          exit(0);
     }
    
    
    //Check that the file isn't empty
    if(FileEmptyCheck(filename) == 0){
            printf("Error: Category file %s is an empty file.\n", filename);
            fclose(file);
            exit(0);
    }
    
    //populate the list:
    while(count < 3){
    
        if(fgets(line, 20, file) == NULL){
            printf("Error reading data from the category file %s\n", filename);
            fclose(file);
            exit(0);
        }
        
        strtok(line, "\n");
        strcpy(output[count], line);
        count++;
        
    
    }
    
    return 1;
    

}
