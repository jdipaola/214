/* reader.c
 * Assignment 4
 * Jeff DiPaola
 * Nov 10, 2014
 *
 * Designed to be used with search.c
 * Function(s): Read and parse an index file into a memory structure
 *
 * Note: The Assignment 4 requirements do not make use of the file counts, that is,
 *       the amount of times each word is used in the file. Therefore, the structure
 *       created ignores this. Listing just words and the files they are contained within.
 *
 * Assumption: User will not manually input a faulty inverted index file
 *              (Index file is always generatred by the indexer)
 */
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 

//Structure definitions (Will be used in search.c as well)

//FileList is a substructure (LinkedList) of List that holds the file names
typedef struct FileList_ FileList;
struct FileList_{
       char FileName[200];
       FileList *next;
};

//List is a LinkedList holding a list of words with references to their filenames
//This is our primary structure.
typedef struct List_ List;
struct List_{
       char word[50];
       FileList files;
       List *next;
};

//Boolean Definition
typedef enum{false, true} bool;




List ReadIn(char invertedindex[200]){
     
     //Declarations
     FILE *file;
     char line[1000];
     List output;
     bool first = true;
     List *currword; //keeps track of what word we're on
     FileList *currfile; //keeps track of what file we're on
     currfile = NULL;
     
     //Open file if possible, error if not
     file = fopen(invertedindex, "r");
     if(file == NULL){
          printf("Error opening inverted index: %s\n", invertedindex);
          exit(0);
     }
     
     //Test to see if the inverted index file is a blank file
     FILE *testfile;
     testfile = fopen(invertedindex, "r");
     fseek(testfile, 0, SEEK_END);
     if(ftell(testfile) == 0){
            printf("Error: Inverted index %s is an empty file.\n", invertedindex);
            fclose(file);
            fclose(testfile);
            exit(0);
    }
    fclose(testfile);
     
     //Read data into structure, line by line
     while(fgets(line, 1000, file) != NULL){
              
          strtok(line, "\n"); //remove newline character at the end of each line
          char *startword = "<list>";
          char *start = strstr(line, startword);
          char *endword = "</list>";
          char *end = strstr(line, endword);
          
          if(start != NULL){ //at location <list> word
               start = start+(7*sizeof(char)); //move to grab just the word
               
               if(first){ //first word being added to the structure?
                    strcpy(output.word, start);
                    output.next = NULL;
                    currword = &output;
                    first = false;
               }
               else{ //not the first word
                    List *newword = malloc(sizeof(List));
                    strcpy(newword->word, start);
                    newword->next = NULL;
                    currword->next = newword;
                    currword = newword;
               }
          }
          else if(end != NULL){ //end of the file listing for the word
               currfile = NULL;
          }
          else{ //on a line that is a listing of files
          
               char *temp;
               temp = strtok(line, " ");
               while(temp){
                    if(!isalpha(*temp)){//is it not a letter (number, some other char)
                        //Do Nothing
                    }
                    else if(currfile == NULL){ //Is this the first file for this word?
                         strcpy(currword->files.FileName, temp);
                         currword->files.next = NULL;
                         currfile = &(currword->files);
                    }
                    else{ //or just an additional file for this word
                        FileList *newfile = malloc(sizeof(FileList));
                        strcpy(newfile->FileName, temp);
                        newfile->next = NULL;
                        currfile->next = newfile;
                        currfile = newfile;
                    }
                    temp = strtok(NULL, " ");
               } //end of while(temp)
          }
     
     } //end of while(fgets)
     
     
     
     fclose(file);
     return output;
}
