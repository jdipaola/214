/* index.c
 * Assignment 3
 * Jeff DiPaola
 * October 20, 2014
 *
 * UPDATED: Assignment 4 - Nov 10, 2014
 * Fixes
 * * Added restriction of listing a max of 5 files per line
 * * Blank input text file or empty folder now outputs blank output.txt instead of one containing a 0
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include "tokenizer.c" - Opted to not use this and just tokenize using isalpha & isnum

/*
 * FileList - Sub Structure contained within List
 * List - Our main structure for storing our values
 */
typedef struct FileList_ FileList;
struct FileList_{
       char FileName[200];
       int count;
       FileList *next;
};

typedef struct List_ List;
struct List_{
       char word[50];
       FileList files;
       List *next;
};



//Global Variable for List of our final output;
List finalout;



/*
 * OutCreator - Creates output file
 * Checks if the file already exists and prompts for overwrite or rename
 * Creates/opens file & returns the file value for use
*/
int OutCreator(char filename[30]){
     
     //check if "filename" already exists:
     if(access(filename, F_OK) != -1){
          printf("The file \"%s\" already exists. \n Would you like to overwite it? (Y/N)\n", filename);
          char answer;
          scanf("%c", &answer);
          answer = tolower(answer);
          while(answer != 'y' && answer != 'n'){
               printf("Invalid Input: Please input either Y or N\n");
               scanf("%c", &answer);
               answer = tolower(answer);
          }
          if(answer == 'n'){
               while((access(filename, F_OK) != -1)){
                    printf("Enter the new filename:\n");
                    scanf("%s", filename);
               }
          }
     }
     
     //Create the file to write to
     FILE *file;
     file = fopen(filename, "w");
     List *curword = &finalout;
     
     //check for blank files - create blank output file
     if(finalout.files.count == 0){
          fclose(file);
          return 1;
     }
     
     //Keep track of the 5 per line restraint
     int counter = 0;
    
    //write to the file
    while(curword != NULL){
         fprintf(file, "<list> %s\n", curword->word);
         FileList *curfile = &(curword->files);
         counter = 0; //reinitalize at each new word
    
         while(curfile != NULL){
              //if statement keeping track of the 5 files per line constraint
              if(counter == 5){
                   fprintf(file, " \n");
                   counter = 0;
              }
              //writing to file
              fprintf(file, "%s %i ", curfile->FileName, curfile->count);
              printf("currfile is %s and the count is %i\n", curfile->FileName, curfile->count);
              curfile = curfile->next;
              counter++;
         }
         
         fprintf(file, "\n</list>\n");
         curword = curword->next;
    
    }
    
    fclose(file);
    return 1;
}






/*
 * FileOrDir - Check if the passed filename is a file, directory, or neither
 * Returns 1 for Directory, 2 for File, or 0 for Error
 * Should actuall pass string back
*/
int FileOrDir(char filename[100]){
    struct stat buf;
    
    if( stat(filename, &buf) == 0){
        if(buf.st_mode & S_IFDIR){
             //Directory was passed - call Directory Searcher recursively
             DirectorySearcher(filename);
             return 1; //was originally going to return type of file
        }
        else if(buf.st_mode & S_IFREG){
             //File was passed - call assembler
             FileReader(filename);
             return 2;//was originally going to return type of file
        }
    }
    
    //Neither a file or a directory was passed - Error Case
    return 0;

}





/*
 * WordAdder - Does our heavy lifting, addes word to structure in appropriate manner
 * If no words are in the structure, adds the first word
 * If word is already there checks for file
 * Increments counter for word & file = true
 * Adds filename and counter for word = true but file = false
 *
 * Takes arguments: infile (file name) & word (word we are adding
 */
int WordAdder(char *infile, char word[50]){
    
    //check that it's a word (error checking)
    if(!isalpha(word[0]) && !isdigit(word[0])){
         return 0;
    }
    
    //Is this the first word?
    if(finalout.word[0] == '\0'){
         strcpy(finalout.word, word);
         strcpy(finalout.files.FileName, infile);
         finalout.files.count = 1;
         finalout.next = NULL;
         finalout.files.next = NULL;
         return 1;
    }
    
    //List *first = &finalout;
    List *temp = NULL; //previous
    List *curr = &finalout; //current
    
    //while there are still elements in our structure
    while(curr != NULL){
         
         //if the word we are adding is alphabetically before our current word
         if(strcmp(word, curr->word) < 0){
              
              if(temp == NULL){ //new word would be new first word of the structure
                      List *new = malloc(sizeof(List));
                      strcpy(new->word, word);
                      strcpy(new->files.FileName, infile);
                      new->files.count = 1;
                      List *movement = malloc(sizeof(List));
                      strcpy(movement->word, finalout.word);
                      movement->next = finalout.next;
                      movement->files = finalout.files;
                      new->next = movement;
                      finalout = *new;
                      return 1;
              }
              
              //otherwise new word is somewhere in the middle
              List *new = malloc(sizeof(List));;
              strcpy(new->word, word);
              strcpy(new->files.FileName, infile);
              new->files.count = 1;
              new->next = curr;
              temp->next = new;
              return 1;
         }
         else if(strcmp(curr->word, word) == 0){ //if they're the same word
              
              //check for a matching filename
              FileList *tempfile = malloc(sizeof(FileList));
              tempfile = NULL;
              FileList *currfile = malloc(sizeof(FileList));
              currfile = &(curr->files);
              while(currfile != NULL){
                   
                   //same word in same file
                   if(strcmp(currfile->FileName, infile) == 0){
                       currfile->count++;
                       FileList *new = malloc(sizeof(FileList)); 
                       new = currfile;
                       tempfile = NULL;
                       currfile = &curr->files;
                       
                       //If the new count is the highest
                       if(new->count > currfile->count){
                            FileList *tempholder = malloc(sizeof(FileList));
                            strcpy(tempholder->FileName, currfile->FileName);
                            tempholder->count = currfile->count;
                            
                            //this accounts for curfile->next being equal to new (would cause infinite link)
                            if(currfile->next != new)
                                 tempholder->next = currfile->next;
                            else
                                tempholder->next = currfile->next->next;
                                
                            new->next = tempholder;
                            curr->files = *new;
                            return 1;
                       }
                       //incrementing to figure out where this file belongs 
                       while(new->count < currfile->count){
                            tempfile = currfile;
                            currfile = currfile->next;
                       }
                       if(tempfile != NULL)
                                   tempfile->next = new;
                       else
                           curr->files = *new;
                       new->next = currfile;
                       return 1;
                   }
                   
                   //increment
                   tempfile = currfile;
                   currfile = currfile->next;
                   
              }//while
              
              //add as last file (new, only has count 1)
              FileList *new = malloc(sizeof(FileList));
              strcpy(new->FileName, infile);
              new->count = 1;
              new->next = NULL;
              tempfile->next = new;
              return 1;
              
         }
         //our new word > current word, increment to next word
         else{
              temp = curr;
              curr = curr->next;
         }
    } //while
    
    //add as last element
    List *new = malloc(sizeof(List));
    strcpy(new->word, word);
    strcpy(new->files.FileName, infile);
    new->files.count = 1;
    new->files.next = NULL;
    new->next = NULL;
    temp->next = new;
    return 1;
}





/*
 * FileReader - Opens the file and reads each word, separating by toenizers
 * Passes the word, when found, to WordAdder
*/
int FileReader(char *infile){
    FILE * toread;
    toread = fopen(infile, "r");
    
    if(!toread){
         printf("Error Opening File: %s\n", infile);
         return 0;
    }
    
    char word[50];
    char tmp = fgetc(toread);
    int i = 0;
    
    //While we haven't reached the end of the file grab char by char
    while(tmp != EOF){
         if(isdigit(tmp)){ //is it a number? it's part of our word
              word[i] = tmp;
              i++;
              tmp = fgetc(toread);
         }
         else if(isalpha(tmp)){ //is it a letter? it's part of our word
              tmp = tolower(tmp);
              word[i] = tmp;
              i++;
              tmp = fgetc(toread);
         }
         else{ //if not, it's a separator
              word[i] = '\0';
              //call add word function
              WordAdder(infile, word);
              i = 0;
              tmp = fgetc(toread);
         }
    } //end: while(tmp)
    
    //add the last word:
    if(i!=0){
             word[i] = '\0';
             WordAdder(infile, word);
    }
    
    return 1;
}



/*
 * DirectorySearcher - Finds all files and directories in a given directory
 * RETURN UNIMPORTANT?
*/
int DirectorySearcher(char *in_name){
    
   FILE *fp;
   char path[1035];
   
   //create file parser
   char* dirsearch = malloc((8*sizeof(char)) + sizeof(in_name) + 1);
   strcpy(dirsearch, "/bin/ls ");
   strcat(dirsearch, in_name);
   fp = popen(dirsearch, "r");
   
   //change to our directory
   chdir(in_name);
   
   //search
   if(fp == NULL){
	printf("Error within DirectorySearcher\n");
   }
   else{
        while(fgets(path, sizeof(path), fp) != NULL){
            strtok(path, "\n");
            FileOrDir(path);
	    }
	//back out to old directory
	chdir("..");
	
	pclose(fp);
   }
   
   return 1;

}




/*
 * main function - implements all our other methods
*/
int main(int argc, char *argv[]){
    
    if(argc != 3){
            printf("Error: Input must be of form: index <inverted-index file name> <directory or file name>\n");
            return 0;
    }
    
    finalout.word[0] = '\0';
    char out_name[30];
    strcpy(out_name, argv[1]);
    char in_name[100];
    strcpy(in_name, argv[2]);
    
    //The below call will recursively traverse all directories/files for output
    //(As long as it's the proper type of input)    
    if(FileOrDir(in_name) == 0){
         printf("Incorrect directory/file as input\n");
         return 0;
    }
    
    //When we're back here, we can assume the structure is full of all the data
    //So we write it to the file before ending the program
    if(OutCreator(out_name) == 0)
         printf("Error writing to file\n");


    return 1;
}
