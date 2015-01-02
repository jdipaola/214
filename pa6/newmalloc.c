/* newmalloc.c
 * Assignment 6
 * Jeff DiPaola
 * Dec 14, 2014
 */

#include "newmalloc.h"

//Global Variables:
currentmem all[50];
bool firstrun = true;
int count = 0;
bool callocing = false;



//My implementation of the malloc function
void *mymalloc(int size, char *file, int line){
    
    //If this is the first run this cleans up our memory structure
    if(firstrun){
        int i;
        for(i=0; i<50; i++){
            all[i].location = NULL;
            all[i].next = 99;
        }
        firstrun = false;
        callocing = false;
        count = 0;
    }
    
    //Accounts for any size (i.e. size of int, etc)
    size = ceil(size/sizeof(char));
    
    //Check that it will fit in the entire array first
    if(size > 5000){
        printf("Not enough memory available to perform malloc in file %s at line %i\n", file, line);
        if(callocing){
            callocing = false;
        }
        return NULL;
    }
    
    //If this is the first item
    if(all[count].location == NULL){
        all[count].size = size;
        all[count].location = &myblock[0];
        all[count].arrayloc = 0;
        all[count].next = 99;
        if(callocing){ //Was this invoked by the calloc function?
            callocing = false;
            int i;
            for(i = all[count].arrayloc; i < (all[count].arrayloc + all[count].size); i++){
                myblock[i] = '\0';
            }
        }
        return all[count].location;
    }
    
    //If this is not the first time
    //Begin by finding first empty space
    int currloc;
    int prevloc;
    currloc = count;
    prevloc = 99;
    
    while(true){
        
        //Last element
        if(all[currloc].location == NULL){
            
            //Checking that there is room available
            int max;
            max = (all[prevloc].size) + (all[prevloc].arrayloc); //the end of prevloc
            if(size > (5000-max)){
                printf("Not enough memory available to perform malloc in file %s at line %i\n", file, line);
                if(callocing){
                    callocing = false;
                }
                return NULL;
            }
            
            all[currloc].arrayloc = all[prevloc].arrayloc + all[prevloc].size;
            all[currloc].location = &myblock[all[currloc].arrayloc];;
            all[currloc].size = size;
            all[currloc].next = 99;
            all[prevloc].next = currloc;
            if(callocing){ //Was ths invoked by the calloc function?
                callocing = false;
                int i;
                for(i = all[currloc].arrayloc; i < (all[currloc].arrayloc + all[currloc].size); i++){
                    myblock[i] = '\0';
                }
            }
            return all[currloc].location;
        }
        //Second element
        else if(all[currloc].next == 99 && prevloc == 99){
            
            //Checking that there is room available
            int max;
            max = (all[currloc].size) + (all[currloc].arrayloc); //the end of currloc
            if(size > (5000-max)){
                printf("Not enough memory available to perform malloc in file %s at line %i\n", file, line);
                if(callocing){ //Was this invoked by the calloc function?
                    callocing = false;
                }
                return NULL;
            }
            
            all[currloc + 1].arrayloc = all[currloc].arrayloc + all[currloc].size;
            all[currloc + 1].location = &myblock[all[currloc + 1].arrayloc];
            all[currloc + 1].size = size;
            all[currloc + 1].next = 99;
            all[currloc].next = currloc + 1;
            if(callocing){ //Was this invoked by the calloc function?
                callocing = false;
                int i;
                for(i = all[currloc + 1].arrayloc; i < (all[currloc + 1].arrayloc + all[currloc + 1].size); i++){
                    myblock[i] = '\0';
                }
            }
            return all[currloc + 1].location;            
        }
        //Fits somewhere in the middle
        else if((prevloc != 99) && (size <= (all[currloc].arrayloc - (all[prevloc].arrayloc + all[prevloc].size)))){
            all[currloc + 1].arrayloc = all[prevloc].arrayloc + all[prevloc].size;
            all[currloc + 1].location = &myblock[all[currloc + 1].arrayloc];
            all[currloc + 1].size = size;
            all[currloc + 1].next = currloc;
            all[prevloc].next = currloc + 1;
            if(callocing){ //Was this invoked by the calloc function?
                callocing = false;
                int i;
                for(i = all[currloc + 1].arrayloc; i < (all[currloc + 1].arrayloc + all[currloc + 1].size); i++){
                    myblock[i] = '\0';
                }
            }
            return all[currloc + 1].location;
        }
        else{
            if(all[currloc].next == 99){ //if the "next" is "null (99)" just add to the end
                prevloc = currloc;
                currloc++;
            }
            else{ //otherwise let's check the next element
                prevloc = currloc;
                currloc = all[currloc].next;
            }
        }  
    }
}




//My implementation of the free function
void myfree(void *ptr, char *file, int line){
    
    if(firstrun){
        int i;
        for(i=0; i<50; i++){
            all[i].location = NULL;
            all[i].next = 99;
        }
        count = 0;
        firstrun = false;
    }
    
    if(all[count].location == NULL || ptr == NULL){ 
        printf("ERROR: Free called on ptr that has not had memory allocated for it in file %s at line %i\n", file, line);
        return;
    }
    
    int curr;
    int  prev = 99;
    curr = count;
    
    //Search through for each element
    while(all[curr].location != NULL){

        if( (all[curr].location) == ptr){
            //found and free
            
            if(prev == 99){ //first item in the list
                if(all[curr].next == 99){
                    all[curr].location = NULL;
                    firstrun = true;
                }
                else{
                    count = all[curr].next;
                    all[curr].location = NULL;
                }            
            }
            else if(all[curr].next == 99){ //Last element
                all[prev].next = 99;
                all[curr].location = NULL;
            }
            else{ //Element in the middle
                all[prev].next = all[curr].next;
            }
            
            //These actions are completed as long as mem is found - Clearing the array out
            int i;
            for(i = all[curr].arrayloc; i < (all[curr].arrayloc + all[curr].size); i++){
                myblock[i] = '\0';
            }
            return;
        }
        else{ //Increment if not found
            prev = curr;
            curr = all[curr].next;
        }
    }
    
    //Back out here, wasn't found
    printf("ERROR: Free called on ptr that has not had memory allocated for it in file %s at line %i\n", file, line);
    return;

}




//My implementation of the calloc function
void *mycalloc(int size, char *file, int line){
    callocing = true;
    return(mymalloc(size, file, line));
}




