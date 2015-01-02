/* books.c 
 * Assignment 5
 * Jeff DiPaola
 * Nov 26, 2014
 *
 * Needs books.h to run
*/


#include "books.h"


/*
 * FileOrDir - Check if the passed filename is a file, directory, or neither
 * Returns 1 for file, 0 & error otherwise
*/
int FileOrDir(void *filename){
    struct stat buf;
    
    if( stat(filename, &buf) == 0 && (buf.st_mode & S_IFREG)){
        return 1;
    }
    
    //Not a file - Error Case
    return 0;

}



/*
 * Producer - Imports book orders
 * Takes Argument: FileName
 * Checks & Opens files and imports into structure for consumers to read
 *
 * ToDo: Move to one buffer per consumer methodology
*/
void *Producer(void* filename){
    
    //Declarations
    char *path = (char *)filename;
     FILE *file;
     char line[1000];
     orders  = (bookorder **)malloc(sizeof(bookorder *) * 3);
     orders[0] = NULL;
     orders[1] = NULL;
     orders[2] = NULL;
     
     //Open file if possible, error if not
     file = fopen(path, "r");
     if(file == NULL){
          printf("Error opening the book order input file: %s\n", filename);
          exit(0);
     }
    
    //Check if the file is empty
    if(FileEmptyCheck(path) == 0){
        printf("The book order input file %s is empty\n", path);
        fclose(file);
        exit(0);
    }
    
    //Begin parsing the file, line by line (order by order)
    while(fgets(line, 1000, file) != NULL){
        bookorder *neworder = malloc(sizeof(bookorder)); 
        char *temp;
        temp = strtok(line, "\""); //removing quotations
        neworder->title = malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(neworder->title, temp); //title
        temp = strtok(NULL, "|");
        neworder->price = atof(temp); //price
        temp = strtok(NULL, "|");
        neworder->custid = atoi(temp); //customer id
        temp = strtok(NULL, "\n");
        neworder->category = malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(neworder->category, temp); //category
        int i;
        for(i = 0; i < 3; i++){
            if(strcmp(categories[i], neworder->category) == 0){
                //Check that the queue isn't full
                bool chk = true;
                while(count[i] == 5){
                    if(chk)
                        printf("Producer is waiting due to a full queue for category: %s\n\n", categories[i]);
                    chk = false;
                }
                if(!chk)
                    printf("Space is available in the queue for %s, producer resuming\n\n", categories[i]); 
                //if/when it's not add the new item to the queue
                pthread_mutex_lock(&orderlock);
                neworder->next = orders[i];
                orders[i] = neworder;
                count[i]++;
                pthread_mutex_unlock(&orderlock);
                i = 5;
            }
        }
        if(i == 3){
             printf("Order for %s neglected. Dropping order.\n Reason: Category %s not found.\n\n", neworder->title, neworder->category);
        }
    }
    
    pthread_mutex_lock(&runchk);
    running = false;
    pthread_mutex_unlock(&runchk);


}



/* Consumer - One will be created per category
 * Processes orders in it's own queue 
 * Only ends once
    (1) The processor is no longer running
    (2) No more orders in its queue to process
*
* Takes argument - categoryname - Determines which category each consumer handles
*/
void *Consumer(void* categoryname){

    char* category = (char*)categoryname;
    int x;
    for(x = 0; x < 3; x++){
        if(strcmp(categories[x],category) == 0){
            break;
        }
    }
    
    pthread_mutex_lock(&runchk);
    
    while(orders[x] != NULL || running){
        
        //Check the queue:
        bool chk = true;
        while(count[x] == 0 && running && orders[x] != NULL){
            if(chk)
                printf("Consumer for category %s is waiting because queue is empty\n\n", categories[x]);
            chk = false;    
        }
        if(!chk)
            printf("There is something in the %s queue, consumer is resuming\n\n", categories[x]);
        
        
        bool todo = false;
        pthread_mutex_lock(&orderlock); //starting to play with order structure
        pthread_mutex_unlock(&runchk);
        
        bookorder *conorder = orders[x];
        bookorder *last = orders[x];

        //Search for book orders that this consumer should process
        while(conorder != NULL){
            
            //If the current order belongs in this consumer's category
            if(strcmp(conorder->category, category) == 0){
                if(last == conorder){ //if this is the first order
                    conorder = last;
                    orders[x] = last->next; //remove this from the main "orders" list
                }
                last->next = conorder->next;
                todo = true;
                break;
            }
            else{
                last = conorder;
                conorder = conorder->next;
            }
        } //end while(conorder->next)
        
        //First and only order for this consumer to process?
        if(conorder != NULL && conorder->next == NULL){
            if(strcmp(conorder->category, category) == 0){
                orders[x] = conorder->next; //remove this one from the main "orders" list
                todo = true;
            }
        }
        
        pthread_mutex_unlock(&orderlock); //done playing with order structure
        
        //If there's something for this consumer to do
        if(todo){
            //Find the customer in our structure
            List *currcust = &custdb;
            
            while(currcust != NULL){ //while we haven't exhausted the list of customers
                
                if(currcust->IDNum == conorder->custid){ //found customer
                    
                    if(currcust->budget >= conorder->price){ //can afford
                        currcust->budget = currcust->budget - conorder->price;
                        revenue = revenue + conorder->price;
                        purchase *success = malloc(sizeof(purchase));
                        success->title = malloc(sizeof(char) * (strlen(conorder->title) + 1));
                        strcpy(success->title, conorder->title);
                        success->price = conorder->price;
                        success->credit = currcust->budget;
                        success->next = currcust->good;
                        currcust->good = success;
                        count[x]--;
                        //Output an order confirmation:
                        printf("ORDER CONFIRMATION:\n Book: %s\n Price: %f\n", conorder->title, conorder->price);
                        printf("Shipping to: %s\n %s, %s, %s\n\n", currcust->name, currcust->street, currcust->state, currcust->zip);
                    }
                    else{ //cant afford
                        rejects *failure = malloc(sizeof(rejects));
                        failure->title = malloc(sizeof(char) * (strlen(conorder->title) + 1));
                        strcpy(failure->title, conorder->title);
                        failure->price = conorder->price;
                        failure->next = currcust->bad;
                        currcust->bad = failure;
                        count[x]--;
                        //Output order rejection:
                        printf("ORDER REJECTION:\n Customer: %s has budget %f\n", currcust->name, currcust->budget);
                        printf("Book: %s\n Price: %f\n\n", conorder->title, conorder->price);
                    }
                    break;
                }
                else{
                    currcust = currcust->next;
                }
            } //end while(currcust)
            if(currcust == NULL){ //if we didn't find a customer with a matching ID
                printf("Order for %s neglected. Dropping order.\n Reason: UserID %i not found.\n\n", conorder->title, conorder->custid);
                count[x]--;
            }
            //conorder = NULL;
        } //end if(todo)
        
    }

return;
}


/* FinalOut - Produces our final out, per the program specs
 * Takes no arguments, uses globally delared struct
 * Uses while loops to travers through:
        All Users
            All Successful orders
            All unsuccessful Orders
*/
void FinalOut(){
    
    List *customer = &custdb;
    if(customer == NULL){
        printf("No orders were placed\n");
        exit(0);
    }
    while(customer != NULL){ //For each customer
        printf("=== BEGIN CUSTOMER INFO ===\n");
        printf("Customer name: %s\nCustomer ID number: %i\n", customer->name, customer->IDNum);
        printf("Remaining credit balance after all purchases (a dollar amount): %f\n", customer->budget);
        printf("### SUCCESSFUL ORDERS ###\n");
        
        //Output all successful orders
        while(customer->good != NULL){
            printf("\"%s\"|%f|%f\n", customer->good->title, customer->good->price, customer->good->credit);
            customer->good = customer->good->next;
        }
        
        //Output all unsuccessful orders
        printf("### REJECTED ORDERS ###\n");
        while(customer->bad != NULL){
            printf("\"%s\"|%f\n", customer->bad->title, customer->bad->price);
            customer->bad = customer->bad->next;
        }
        printf("=== END CUSTOMER INFO ===\n\n");
        customer = customer->next;
    } 
}



/* Main function
 * Checks input arguments for error conditions
 * Calls for creation of category struct
 * Calls for creation of customer database
 * Creates producer & consumer threads
 * Producer & consumer return here upon completion
 * Calls output function
*/
int main(int argc, char *argv[]){
    
    //Check Proper input format
    if(argc != 4){
        printf("ERROR: Input was not of the correct format.\n");
        printf("Input should be of the form: \n");
        printf("./books <database file> <book order file> <category file>\n");
        return 0;
    }
    
    //Check all input files are actual files, not anything else (like a directory)
    if(FileOrDir(argv[1]) == 0){ //Name of database input file
        printf("Customer Database file: %s was not of the correct type\n", argv[1]);
        return 0;
    }
    else if(FileOrDir(argv[2]) == 0){ //Name of book order input file
        printf("Book Order file: %s was not of the correct type\n", argv[2]);
        return 0;
    }
    else if(FileOrDir(argv[3]) == 0){ //Name of category name file
        printf("Book Category file: %s was not of the correct type\n", argv[3]);
        return 0;
    }
    
    //Read in the categories:
    categories  = (char **)malloc(sizeof(char *) * 3);
    categories[0] = (char *) malloc(sizeof(char) * 20);
    categories[1] = (char *) malloc(sizeof(char) * 20);
    categories[2] = (char *) malloc(sizeof(char) * 20);
    ReadInCats(argv[3], categories);
    
    //Create Customer database
    custdb = ReadIn(argv[1]);
    
    //Invoke the producer & the order structure mutex
    pthread_mutex_init(&orderlock, 0);
    pthread_t producer_thread;
    if(pthread_create(&producer_thread, 0, Producer, (void *)argv[2]) != 0){
        printf("Error Creating the Producer\n");
        exit(0);
    }
    
    /* Delay!!
     * The program was sometimes (rarely) running the producer before the categories
     * had finished being read. The delay below should stop that.
     */
    int eattime;
    for(eattime = 0; eattime < 10000; eattime++){
        eattime = eattime;
    }
    
    //Prepare the consumer conuters:
    count[0] = 0;
    count[1] = 0;
    count[2] = 0;    
    
    //Invoke the consumers:
    pthread_t consumer_thread[3];
    int i;
    for(i = 0; i < 3; i++){
        if(pthread_create(&consumer_thread[i], 0, Consumer, (void *) categories[i]) != 0){
            printf("Error creating consumer %i\n", i);
            exit(0);
        }
    }
    
    //pthread joins
    pthread_join(producer_thread, NULL);
    for (i = 0; i < 3; i++){
        pthread_join(consumer_thread[i], NULL);
    }
    
    //Program is over - Don't need the lock anymore
    pthread_mutex_destroy(&orderlock);
    
    //Generate Final Output
    printf("\n\n\n"); //formatting
    printf("Total Revenue Gained: %f\n\n", revenue);
    FinalOut();
    
}
