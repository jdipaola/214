/* tokenizer.c
 * Jeff DiPaola
 * CS 214
 * 9/24/14
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
       char *syms;
       char *front;
       char *start;
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 * 
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate(char *separators, char *ts) {
           
           //initalize all:
           TokenizerT *seps = malloc(sizeof(TokenizerT));
           seps->front = malloc((strlen(ts)));
           seps->syms = malloc((strlen(separators)));
		   seps->start = malloc((strlen(ts)));
           
           //assign all passed values to each part of the tokenizer
           strcpy(seps->front, ts); 
           strcpy(seps->syms, separators);;
		   seps->start = seps->front; 
		   
           
  return seps;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {
     //using the free command to release all of tk
	 free(tk->syms);
	 free(tk->front);
	 free(tk->start);
     free(tk);
}

/*
* FindTheSymbol is a nested for-loop
* Outer loop: traverses the input string
* Inner loop: traverses the symbols, compares each to the current letter of the string
*/

char *FindTheSymbol(char *instr, char *symbols){
	char x, *y;
	
	for(x = *instr; x !=0; instr++, x = *instr){ //traversing through input string
          for(y = symbols; *y !=0; y++){ //traversing through separators
                if (x==*y) //if they are the same (symbol found)
                   return instr; //return the current point in the string
          }
    }
                   
	return (NULL); //Null when there are no separators 
}

/*
 * EscChk checks for escape characters
 * First set of IF statements checks for single char escape characters ('\n')
 * Inner set of IF statements checks for 2 char escape chars ('\', 'n')
 * If found it swaps them for their proper hex code given by the assignment
 * Note: I made this before the announcement that told us we'd be passing escape characters as 2 characters
 */

char *EscChk(char *word){

     char *curr; //current position in the token
     char *first = malloc(sizeof(char)); //first character (esc char or \)
     char *second = malloc(sizeof(char)); //second character (if first char is \)
     int counter = 0;
     
     for(curr = word; *curr; curr++)
     {
                counter++;
                strncpy(first, curr, 1); 
                curr++;
                second = strncpy(second, curr, 1);
                curr--;
                
                if(*first == '\n') { //newline char
                         char *newtoken = malloc(sizeof(word) + 4);
                         strncpy(newtoken, word, counter);
                         strcat(newtoken, "[0x0a]");
                         curr++;
                         strcat(newtoken, curr);
                         return newtoken;
                }
                else if(*first == '\t'){ //horizontal tab
                         char *newtoken = malloc(sizeof(word) + 4);
                         strncpy(newtoken, word, counter);
                         strcat(newtoken, "[0x09]");
                         curr++;
                         strcat(newtoken, curr);
                         return newtoken;
                }
                else if(*first == '\v'){ //vertical tab
                         char *newtoken = malloc(sizeof(word) + 4);
                         strncpy(newtoken, word, counter);
                         strcat(newtoken, "[0x0b]");
                         curr++;
                         strcat(newtoken, curr);
                         return newtoken;
               }
               else if(*first == '\b'){ //backspace
                         char *newtoken = malloc(sizeof(word) + 4);
                         strncpy(newtoken, word, counter);
                         strcat(newtoken, "[0x08]");
                         curr++;
                         strcat(newtoken, curr);
                         return newtoken;
                }
               else if(*first == '\r'){ //carriage return
                         char *newtoken = malloc(sizeof(word) + 4);
                         strncpy(newtoken, word, counter);
                         strcat(newtoken, "[0x0d]");
                         curr++;
                         strcat(newtoken, curr);
                         return newtoken;
               }
               else if(*first == '\f'){ //form feed
                         char *newtoken = malloc(sizeof(word) + 4);
                         strncpy(newtoken, word, counter);
                         strcat(newtoken, "[0x0c]");
                         curr++;
                         strcat(newtoken, curr);
                         return newtoken;
                }
                else if(*first == '\a'){ //audible alert
                         char *newtoken = malloc(sizeof(word) + 4);
                         strncpy(newtoken, word, counter);
                         strcat(newtoken, "[0x07]");
                         curr++;
                         strcat(newtoken, curr);
                         return newtoken;
                }
                else if(*first == '\"'){ //double quote
                         char *newtoken = malloc(sizeof(word) + 4);
                         strncpy(newtoken, word, counter);
                         strcat(newtoken, "[0x22]");
                         curr++;
                         strcat(newtoken, curr);
                         return newtoken;
                }
                else if(*first == '\\') //backslash by itself
                {              //check for two character esc chars
                            if(*second == 'n'){ //newline
                                 char *newtoken = malloc(sizeof(word) + 5);
                                 strncpy(newtoken, word, counter - 1);
                                 strcat(newtoken, "[0x0a]");
                                 curr = curr + 2;
                                 strcat(newtoken, curr);
                                 return newtoken;
                            }
                            else if(*second == 't'){ //horizontal tab
                                 char *newtoken = malloc(sizeof(word) + 5);
                                 strncpy(newtoken, word, counter - 1);
                                 strcat(newtoken, "[0x09]");
                                 curr = curr + 2;
                                 strcat(newtoken, curr);
                                 return newtoken;
                            }
                            else if(*second == 'v'){ //vertical tab
                                 char *newtoken = malloc(sizeof(word) + 5);
                                 strncpy(newtoken, word, counter - 1);
                                 strcat(newtoken, "[0x0b]");
                                 curr = curr + 2;
                                 strcat(newtoken, curr);
                                 return newtoken;
                            }
                            else if(*second == 'b'){ //backspace
                                 char *newtoken = malloc(sizeof(word) + 5);
                                 strncpy(newtoken, word, counter - 1);
                                 strcat(newtoken, "[0x08]");
                                 curr = curr + 2;
                                 strcat(newtoken, curr);
                                 return newtoken;
                            }
                            else if(*second == 'r'){ //carrige return
                                 char *newtoken = malloc(sizeof(word) + 5);
                                 strncpy(newtoken, word, counter - 1);
                                 strcat(newtoken, "[0x0d]");
                                 curr = curr + 2;
                                 strcat(newtoken, curr);
                                 return newtoken;
                            }
                            else if(*second == 'f'){ //form feed
                                 char *newtoken = malloc(sizeof(word) + 5);
                                 strncpy(newtoken, word, counter - 1);
                                 strcat(newtoken, "[0x0c]");
                                 curr = curr + 2;
                                 strcat(newtoken, curr);
                                 return newtoken;
                            }
                            else if(*second == 'a'){ //audible alert
                                 char *newtoken = malloc(sizeof(word) + 5);
                                 strncpy(newtoken, word, counter - 1);
                                 strcat(newtoken, "[0x07]");
                                 curr = curr + 2;
                                 strcat(newtoken, curr);
                                 return newtoken;
                            }
                            else if(*second == '\"'){ //double quote
                                 char *newtoken = malloc(sizeof(word) + 5);
                                 strncpy(newtoken, word, counter - 1);
                                 strcat(newtoken, "[0x22]");
                                 curr = curr + 2;
                                 strcat(newtoken, curr);
                                 return newtoken;
                            }
                            else{ //just a backslash, followed by some other char
                                 char *newtoken = malloc(sizeof(word) + 5);
                                 strncpy(newtoken, word, counter - 1);
                                 curr++;
                                 strcat(newtoken, curr);
                                 return newtoken;   
                            }
                }
     }
     //no escape chars found, return the original
     return word;
}


/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk) {
     
     //check for end of input
     if(tk->start == NULL){
                  return NULL;
     }
     
     char *temp = tk->start;
	 char *end = FindTheSymbol(tk->start, tk->syms);
     //if there are no separators
     if(end==NULL){ 
                    char *word = malloc(strlen(tk->start) +1); //create "word" to store the 1 token
                    strcpy(word, tk->start); //move the 1 token to "word"
                    tk->start = NULL; //set start to null since there are no more tokens
                    return(word); //return the single token
     }
     
     //now if there are separators...
     char *word = malloc(strlen(tk->start));
     int len = ((int)(end-temp)); //length of the token
     strncpy(word, tk->start, len); //will copy up to the location of the separator
     word[len] = '\0'; //last character in output is '\0' per specs
     tk->start = (temp+len+1); //increment tk->start by the length of the token +1S
    
  return word;
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
    
    //check number of input arguments is correct:
    if(argc > 3){
          printf("Error: Too many arguments passed.\n");
          return 0;
    }
    else if(argc < 3){
         printf("Error: Not enough arguments passed.\n");
         return 0;
    }
    
    //initialize everything
    char *sepchars = argv[1];
    char *tokenz = argv[2];
    char *temp;
	
	//create the tokenizer
    TokenizerT *full = TKCreate(sepchars, tokenz);
	
	//output:
	temp=(TKGetNextToken(full)); //use TKGetNextToken to get the next available word
    while(temp != NULL){ //while we haven't hit the end of the input string
        temp = EscChk(temp); //check for escape characters
        if(strlen(temp) != 0) //check for blank chars
		        printf("%s\n", temp); //output the word
		temp=(TKGetNextToken(full)); //grab the next word 
    }

    TKDestroy(full); //destroy the TokenizerT object now that we are done

  return 0;
}
