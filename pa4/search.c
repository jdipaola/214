/* search.c
 * Assignment 4
 * Jeff DiPaola
 * Nov 10, 2014
 *
 * Takes inverted index file from previous index.c as argument
 * Developed with reader.c - This calls on reader.c
 * Function(s): Implements the user interface & search functionality of the project
*/

#include "reader.c"
//#include <string.h>
List input;


/* AndSearch - Implements an sa call
 * Takes Argument: words (input string)
 * Returns: 1 - Success     0 - Failure
*/
int AndSearch(char words[500]){
    
    char *temp;
    temp = strtok(words, " "); //this would be the sa
    temp = strtok(NULL, " "); //this would be the first word
    
    if(temp == NULL || !isalpha(*temp)) //only search term was passed
        return 0;
    
    FileList output; //will store our search results
    List *currword = &input;
    bool first = true;
    
    //while there are still terms to search
    while(temp){
        
        /*printf("Comparison test of %s and %s yields %i\n", currword->word, temp, strncmp(currword->word, temp, strlen(temp)));
        if(strncmp(currword->word, temp, strlen(temp)) == 0)
            printf("MATCH\n");*/
        
        if(!isalpha(*temp)){
            temp = strtok(NULL, " "); //get the next word
            currword = &input;
        }
        else if(strncmp(currword->word, temp, strlen(temp)) < 0){ //we are before where the word would be found
            currword = currword->next;
        }
        else if(strncmp(currword->word, temp, strlen(temp)) > 0){ //past where the word would be found
            currword = &input; //reset to begining of all possible words
            temp = strtok(NULL, " "); //grab next word to search
        }
        else if(strncmp(currword->word, temp, strlen(temp)) == 0){ //word found
            FileList *placeholder = &currword->files;
            
            if(first){ //is this the first list of files being added to our output?
                first = false;
                strcpy(output.FileName, placeholder->FileName);
                output.next = NULL;
                placeholder = placeholder->next; //next filename
                FileList *truespot = &output; //hold our location in output
                while(placeholder){ //while there are files associated with this word
                    FileList *newest = malloc(sizeof(FileList)); //create new structure
                    strcpy(newest->FileName, placeholder->FileName);
                    newest->next = NULL;
                    truespot->next = newest; //add to end of the output list
                    truespot = newest; //repoint pointer to end
                    placeholder = placeholder->next; //increment to next filename
                } //end while(placeholder)
            }
            else{ //there are already elements in output
                FileList *truespot = &output;
                FileList *prevspot = NULL;
                while(truespot){
                    placeholder = &currword->files;
                    bool found = false;
                    while(placeholder){
                        if(strcmp(placeholder->FileName, truespot->FileName) == 0){ //same filename?
                            found = true;
                            break;
                        }
                        placeholder = placeholder->next;
                    } //end while(placeholder)
                    if(!found){
                        if(prevspot == NULL){
                            if(output.next == NULL)
                                return 0;
                            else
                                output = *output.next;
                            truespot = &output;
                        }
                        else{
                            prevspot->next = truespot->next;
                            truespot = truespot->next;
                        }
                    }
                    else{
                        prevspot = truespot;
                        truespot = truespot->next;
                    }
                } //end while(truespot)
            }
            currword = &input; //reset to begining of all possible words
            temp = strtok(NULL, " "); //grab next word to search
        } //end else if
        else{
            return 0;
        }
    } //end while(temp)
    
    //were any results obtained?
    FileList *outptr = &output;
    if(outptr == NULL || first)
        return 0;
    
    //output results
    printf("Your search word(s) were contained in the files: ");
    while(outptr){
        printf("%s ", outptr->FileName);
        outptr = outptr->next;
    }
    printf("\n\n");
    return 1;
}





/* AndSearch - Implements an so call
 * Takes Argument: words (input string)
 * Returns: 1 - Success     0 - Failure
*/
int OrSearch(char words[500]){
    
    char *temp;
    temp = strtok(words, " "); //this would be the so
    temp = strtok(NULL, " "); //this would be the first word
    
    if(temp == NULL || !isalpha(*temp)) //only search term was passed
        return 0;
    
    FileList output;
    bool first = true;
    
    while(temp){
        
        if(!isalpha(*temp))
            temp = strtok(NULL, " ");
        else{
            List *currpos = &input; //current position in our list of words
            
            while(currpos){
                if(strncmp(currpos->word, temp, strlen(temp)) == 0){ //the word has been found
                
                    if(first){ //first time files are being added to output
                        first = false;
                        FileList *inputpos = &currpos->files; //holds files to be put into output
                        strcpy(output.FileName, inputpos->FileName);
                        output.next = NULL;
                        inputpos = inputpos->next;
                        FileList *outputpos = &output; //holds the current position we're in in "output"
                        
                        while(inputpos){ //while there are more file names to add
                            FileList *newguy = malloc(sizeof(FileList));
                            strcpy(newguy->FileName, inputpos->FileName);
                            newguy->next = NULL;
                            outputpos->next = newguy;
                            outputpos = newguy; //current file is the new file
                            inputpos = inputpos->next; //move onto next file
                        } //end while(inputpos)
                    } //end if(first)
                    
                    else{ //if not the first time files are being added to output
                        FileList *inputpos = &currpos->files; //holds the files to be put into output
                        
                        while(inputpos){
                            FileList *outputpos = &output;
                            FileList *prevoutpos = NULL;
                            
                            while(outputpos){
                                if(strcmp(inputpos->FileName, outputpos->FileName) == 0)
                                    break;
                                prevoutpos = outputpos;
                                outputpos = outputpos->next;
                            }
                            if(outputpos == NULL){ //if we reached the end of our file list without finding the current file
                                FileList *newguy = malloc(sizeof(FileList));
                                strcpy(newguy->FileName, inputpos->FileName);
                                newguy->next = NULL;
                                prevoutpos->next = newguy;
                            }
                            inputpos = inputpos->next;
                        } //end else
                    } //end while
                
                break; //Added the files as necessary, break back out to check next word
                }
                else if(strncmp(currpos->word, temp, strlen(temp)) > 0) //temp is not in array
                    break;
                else //current searh term (temp) could be later in the array
                    currpos = currpos->next;
                        
            }//end while(currpos)
        } //end else
        temp = strtok(NULL, " ");
    } //end while(temp)
    
    //were any results obtained?
    FileList *outptr = &output;
    if(outptr == NULL || first){
        return 0;
    }
    
    //output results
    printf("Your search word(s) were contained in the files: ");
    while(outptr){
        printf("%s ", outptr->FileName);
        outptr = outptr->next;
    }
    printf("\n\n");
    
    return 1;
}








/*
 * main - Implements the primary part of our function
*/
int main(int argc, char *argv[]){
    
    //Wrong number of arguments
    if(argc != 2){
            printf("Error: Input must be of the form: search <inverted-index file name>\n");
            exit(0);
    }
    
    //Create our searchable structure - implementation of reader.c
    char filename[200];
    strcpy(filename, argv[1]);
    input = ReadIn(filename);
    
    //Now we can begin prompting the user for search input
    while(1){
        
        //Get input
        printf("Please enter your search\n");
        char words[500];
        char tmp[5];
        fgets(words, sizeof(words), stdin);
        strncpy(tmp, words, sizeof(tmp));
        strtok(words, "\n");
        
        //grab the search term
        char *term;
        term = strtok(tmp, " ");
        
        //Take the appropriate action
        if(strcmp(term, "sa") == 0 || strcmp(term, "Sa") == 0 || strcmp (term, "sA") == 0 || strcmp(term, "SA") == 0){
            if(AndSearch(words) == 0)
                printf("Your search yielded no results\n");
        }
        else if(strcmp(term, "so") == 0 || strcmp(term, "So") == 0 || strcmp (term, "sO") == 0 || strcmp(term, "SO") == 0){
            if(OrSearch(words) == 0)
                printf("Your search yielded no results\n");
        }
        else if(strcmp(term, "q") == 0 || strcmp(term, "Q") == 0 || strcmp(term, "q\n") == 0 || strcmp(term, "Q\n") == 0){
            //Quit
            printf("Exiting...\n");
            return 0;
        }
        else{
            printf("Incorrect search term. Must be of one of the below formats: \n");
            printf("sa <term>...\nso <term>...\nq\n\n");
        }
    
    }
    

return 0;
}
