/*sorted-list.c
* Jeff DiPaola
* CS 214 - Assignment 2
*/

#include <stdlib.h>
#include <stdint.h>
#include "sorted-list.h"

int changes=0; //for iterator


/*
 * SLCreate
 * Given a compare and destruct function
 * create the new element
 */
SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df){              
              //make sure cf and df exist:
              if(cf && df){
                    SortedListPtr newguy = malloc(sizeof(SortedListPtr)); //create the new element
                    newguy->cf = cf; //add cf to it
                    newguy->df = df; //add df to it
                    newguy->start = malloc(sizeof(Node));
                    newguy->start = NULL;
                    return newguy; //return it
                    }
                    
              //if we don't end up in the if statement above
              //either cf or df doesn't exist so return NULL
              //will not reach this as long as cf and df exist
              return NULL;
}
              

//Erradicate list from memory
void SLDestroy(SortedListPtr input){
     //Keep track of current position
     Node current = input->start;
              
     //while current exists
     while(current){
                    Node other = current; //gonna delete other
                    current = current->next; //increment current to the next object
                    (*input).df(other->obj); //delete the "current" object (other)
                    free(other); //free the object in its entirety
                    }
                    
     free(current); //free our marker
     free(input); //free the list in general
     
     return;
}
     

/*
 *Inserting a new element in the proper sopt in our list
 */
int SLInsert(SortedListPtr input, void *newbie){
    
    /*check for error classes
     * If the new element doesn't exist we can't add anything
     * If the list doesn't exist we are going to take it as the same condition
     * This was not specified in the project description
     * But being that we don't have newbie's df and cf we can't call SLCreate
     */
    if(!newbie || !input)
              return 0;
              
    //Initalizations:
    Node ptr1 = malloc(sizeof(Node));
    Node ptr2 = malloc(sizeof(Node));
    Node put = malloc(sizeof(Node));
    put->obj = malloc(sizeof(newbie));
    put->next = malloc(sizeof(Node));
    put->obj = newbie;
    ptr1 = input->start;
    
    //If there's nothing in the list yet
    if(!ptr1){
              input->start = put;
              put->next = NULL;
    }
    
    //While we're not at the end of the loop:
    while(ptr1){
                if((*input).cf(newbie, ptr1->obj) > 0 ){ //if newbie>current
                                     put->next = ptr1; // ptr1 will be after put
                                     if(&(input->start->obj) == &(ptr1->obj)){ //if we're at the first node
                                                        input->start = put; //new node is the first node
                                                        }
                                     else //if not
                                         ptr2 -> next = put; //inserting after ptr2
                                     //return 1;
                                     return 1; //and this is our new insert
                }
				else if((*input).cf(newbie, ptr1->obj) == 0){ //Given by announcement on 9/28/14
					printf("ERROR: Cannot Insert Duplicate Values\n");
					return 0;
				}
                //increment if we haven't found the right spot in the list
                ptr2 = ptr1; 
                ptr1 = ptr1->next;
    }
    
    
    //If it's the last element ptr1 will be null
    //So we'll end up out here:
    ptr2->next = put;
    put->next = NULL;
    //return 1;
    return 1;
    
    }
    
    
    
/*
 * Find and remove given data from the list
 */
int SLRemove(SortedListPtr list, void *newObj){
    
    //if bad arguments are passed, do nothing
    if(!newObj|| !list)
                   return 0;
    
    //Initalizations:
    Node ptr1 = malloc(sizeof(Node));
    Node ptr2 = malloc(sizeof(Node));
    ptr1 = list->start;
    
    //while we haven't hit the end of the end of the list
    while(ptr1){                
                if((*list).cf(newObj, ptr1->obj) == 0){ //if they're the same                      
                        if(!ptr2) //if it's the first element
                                       list->start = ptr1->next; //replace the start
                        else if(ptr1->next == NULL) //if it is the last element
                             ptr2->next = NULL; //chop off the last element (ptr1)
                        else
                             ptr2->next = ptr1->next; //make ptr2 (before ptr1) point to what's after ptr1
                        //These need to be done regardless if we're inside the loop:
                        (*list).df(ptr1->obj); //delete the "current" object (ptr1)
                        free(ptr1); //free the object in its entirety
                        changes++; //note that a change was made for iterrator
                        return 1; //mission success          
                }
                //increment if we haven't found the match yet
                ptr2 = ptr1;
                ptr1 = ptr1->next;
    }
//if we don't find a match we end up out here
return 0;    
}
    
    
//Create Iterator object
SortedListIteratorPtr SLCreateIterator(SortedListPtr input){
        fflush(0);
        if(!input) //check for null input
                      return NULL;
        
        //Create and intialize object:
        SortedListIteratorPtr iter = malloc(sizeof(SortedListIteratorPtr));
        iter->list = malloc(sizeof(SortedListPtr));
        iter->list = input;
        iter->ptr = malloc(sizeof(Node));
        iter->ptr = input->start;
        iter->count=changes; //so we can tell if a change is made later
        iter->lastobj = NULL; //goes along with the above, last object for nextitem
        
        return iter;
}


//Destory the iterator object
void SLDestroyIterator(SortedListIteratorPtr input){
     free(input);
     return;
}

//Per the instrucftions returns the object of the current spot, 0 if no pointer
void *SLGetItem(SortedListIteratorPtr iter){
     if(iter->ptr == NULL)
                  return 0;
     
     return iter->ptr->obj;     
}
     
//Gather the next item
void *SLNextItem(SortedListIteratorPtr iter){
          
     if(changes != iter->count){ //if something has been changed
                iter->ptr = iter->list->start; //reset the pointer to the begining of the list
                iter->count = 0;
                
                if(iter->lastobj){ //if we aren't at the first object

                                   while(iter->list->cf(iter->lastobj, SLGetItem(iter)) < 0){ //if it wasn't the start of the list
                                            iter->count++;
                                            iter->ptr = iter->ptr->next; //increment                                                        
                                   }
                }
     //reset change detector
     changes = iter->count;
     }
     
     else{ //nothing changed, move to next object
          if(iter->ptr == NULL)
                       return NULL;
          iter->lastobj = SLGetItem(iter);
          iter->ptr = iter->ptr->next;
          iter->count++;
          changes = iter->count;
          return iter->lastobj;
          }
          
     return SLGetItem(iter);
}
