/* test.c
 * Assignment 6
 * Jeff DiPaola
 * Dec 14, 2014
 *
 * test.c is used to run the test cases for the newmalloc program
 */
 
 #include "newmalloc.c"
 
 
 int main(){
       
    printf("Test Cases:\n");
    printf("\t(0) Exit\n");
    printf("\t(1) malloc & free of a char*\n");
    printf("\t(2) malloc & free of an int*\n");
    printf("\t(3) free of a pointer that was not malloc'd after mallocing a different pointer\n");
    printf("\t(4) free of a pointer that wasn't malloc'd without any other malloc\n");
    printf("\t(5) free of a pointer that was already freed\n");
    printf("\t(6) malloc ptr, free ptr, malloc same ptr, free same ptr\n");
    printf("\t(7) freeing pointers to dynamic mem that weren't returned from malloc\n");
    printf("\t(8) malloc 3 ptrs, free middle ptr, then first, then last\n");
    printf("\t(9) malloc 3 ptrs, free first, last, middle\n");
    printf("\t(10) malloc 3 ptrs, free last, first, middle\n");
    printf("\t(11) malloc 3 ptrs, free first, middle, last\n");
    printf("\t(12) malloc & free of entire available area\n");
    printf("\t(13) malloc of more than available area\n");
    printf("\t(14) malloc of entire area, free part, malloc that part\n");
    printf("\t(15) malloc of entire area, try to malloc more\n");
    printf("\t(16) malloc most of the area, try to malloc more than what's left\n\n");
    printf("Extra Credit Test Cases:\n");
    printf("\t(17) test of calloc\n");
    printf("\t(18) Free a ptr not previously returned by malloc/calloc\n");
    printf("\t(19) Free a ptr not in the heap\n");
    printf("\n");
    
    int input = 100;
    while(input != 0){
        printf("Enter 1-19 to select a test case or 0 to exit\n\n");
        scanf("%d", &input);
        
        switch(input){
            case 0:
                break;
            case 1: //malloc & free of char*
                {
                printf("Test Case 1: \n\tchar *ctemp = malloc(6*sizeof(char))\n");
                printf("\tfree(ctemp)\n");
                char *ctemp = malloc(6 * sizeof(char));
                free(ctemp);
                printf("Test Case 1 Complete.\n\n");
                break;
                }
            case 2: //malloc & free of int*
                {
                printf("Test Case 2: \n\tint *itemp = malloc(10*sizeof(int))\n");
                printf("\tfree(itemp)\n");
                int *itemp = malloc(10 * sizeof(int));
                free(itemp);
                printf("Test Case 2 Complete.\n\n");
                break;
                }
            case 3: //malloc ptr a, free ptr b (never malloc'd)
                {
                printf("Test Case 3: \n\tint *temp3 = mymalloc(sizeof(int))\n");
                printf("\tchar x\n\tmyfree(&x)\n");
                int *temp3 = malloc(sizeof(int));
                char x;
                printf("Freeing &x\n");
                free(&x);
                printf("Freeing temp3\n");
                free(temp3);
                printf("Test Case 3 Complete.\n\n");
                break;
                }
            case 4: //free ptr without mallocing anything
                {
                printf("Test Case 4: \n\tint temp4\n\tfree(&temp4)\n");
                int temp4;
                free(&temp4);
                printf("Test Case 4 Complete.\n\n");
                break;
                }
            case 5: //free ptr that was already freed
                {
                printf("Test Case 5: \n\tchar *temp5 = malloc(1000*sizeof(char))\n");
                printf("\tfree(temp5)\n\tfree(temp5)\n");
                char *temp5 = malloc(1000*sizeof(char));
                printf("First free\n");
                free(temp5);
                printf("Second free\n");
                free(temp5);
                printf("Test Case 5 Complete.\n\n");
                break;
                }
            case 6: //malloc ptr, free ptr, malloc same ptr, free same ptr
                {
                printf("Test Case 6: \n\tchar*temp6 = malloc(4000*sizeof(char))\n\tfree(temp6)\n");
                printf("\ttemp6 = malloc(2000*sizeof(char))\n\tfree(temp6)\n");
                char *temp6 = malloc(4000*sizeof(char))
                printf("First free\n");
                free(temp6);
                temp6 = malloc(2000*sizeof(char));
                printf("Second free\n");
                free(temp6);
                printf("Test Case 6 Complete.\n\n");
                break;
                }
            case 7: //freeing pointers to dynamic mem that weren't returned from malloc
                { 
                printf("Test Case 7: \n\tchar *p = (char*)malloc(200)\n\tfree(p + 10)\n");
                printf("\tfree(p)\n");
                char *p = (char*)malloc(200);
                printf("free(p + 10)\n");
                free(p + 10);
                free(p);
                printf("Test Case 7 Complete.\n\n");
                break;
                }
            case 8: //malloc 3 ptrs, free middle ptr, then first, then last
                {
                printf("Test Case 8: \n\tint *temp8a = malloc(600*sizeof(int))\n");
                printf("\tchar *temp8b = malloc(1000*sizeof(char))\n\tint *temp8c = malloc(100*sizeof(int))\n");
                printf("\tfree(temp8b)\n\tfree(temp8a)\n\tfree(temp8c)\n");
                int *temp8a = malloc(600*sizeof(int));
                char *temp8b = malloc(1000*sizeof(char));
                char *temp8c = malloc(100*sizeof(int));
                printf("Freeing middle ptr\n");
                free(temp8b);
                printf("Freeing first ptr\n");
                free(temp8a);
                printf("Freeing last ptr\n");
                free(temp8c);
                printf("Test Case 8 Complete.\n\n");
                break;
                }
            case 9: //malloc 3 ptrs, free first, last, middle
                {
                printf("Test Case 9: \n\tchar *temp9a = malloc(800*sizeof(char))\n");
                printf("\tchar *temp9b = malloc(200*sizeof(char))\n\tchar *temp9c = malloc(100*sizeof(char))\n");
                printf("\tfree(temp9a)\n\tfree(temp9c)\n\tfree(temp9b)\n");
                char *temp9a = malloc(800*sizeof(char));
                char *temp9b = malloc(200*sizeof(char));
                char *temp9c = malloc(100*sizeof(char));
                printf("Freeing first\n");
                free(temp9a);
                printf("Freeing last\n");
                free(temp9c);
                printf("Freeing middle\n");
                free(temp9b);
                printf("Test Case 9 Complete.\n\n");
                break;
                }
            case 10: //malloc 3 ptrs, free last, first, middle
                {
                printf("Test Case 10: \n\tchar*temp10a = malloc(800*sizeof(char))\n");
                printf("\tchar *temp10b = malloc(200*sizeof(char))\n\tchar *temp10c = malloc(100*sizeof(char))\n");
                printf("\tfree(temp10c)\n\tfree(temp10a)\n\tfree(temp10b)\n");
                char *temp10a = malloc(800*sizeof(char));
                char *temp10b = malloc(200*sizeof(char));
                char *temp10c = malloc(100*sizeof(char));
                printf("Freeing last\n");
                free(temp10c);
                printf("Freeing first\n");
                free(temp10a);
                printf("Freeing middle\n");
                free(temp10b);
                printf("Test Case 10 Complete.\n\n");
                break;
                }
            case 11: //malloc 3 ptrs, free first, middle, last
                {
                printf("Test Case 11: \n\tchar *temp11a = malloc(800*sizeof(char))\n");
                printf("\tchar *temp11b = malloc(200*sizeof(char))\n\tchar *temp11c = malloc(100*sizeof(char))\n");
                printf("\tfree(temp11a)\n\tfree(temp11b)\n\tfree(temp11c)\n");
                char *temp11a = malloc(800*sizeof(char));
                char *temp11b = malloc(200*sizeof(char));
                char *temp11c = malloc(100*sizeof(char));
                printf("Freeing first\n");
                free(temp11a);
                printf("Freeing middle\n");
                free(temp11b);
                printf("Freeing last\n");
                free(temp11c);
                printf("Test Case 11 Complete.\n\n");
                break;
                }
            case 12: //malloc of entire available area
                {
                printf("Test Case 12: \n\tchar *temp12 = malloc(5000*sizeof(char))\n");
                printf("\tfree(temp12)\n");
                char *temp12 = malloc(5000*sizeof(char));
                free(temp12);
                printf("Test Case 12 Complete.\n\n");
                break;
                }
            case 13: //malloc of more than available area
                {
                printf("Test Case 13: \n\tchar *temp13 = malloc(5001*sizeof(char))\n");
                printf("\tint *temp13b = malloc(1251*sizeof(int))\n\tchar *temp13c = malloc(8000*sizeof(char))\n");
                char *temp13 = malloc(5001*sizeof(char));
                int *temp13b = malloc(1251*sizeof(int));
                char *temp13c = malloc(8000*sizeof(char));
                printf("Test Case 13 Complete.\n\n");
                break;
                }
            case 14: //malloc of entire area, free part, malloc that part
                {
                printf("Test Case 14: \n\tchar *temp14a = malloc(3000*sizeof(char))\n");
                printf("\tchar *temp14b = malloc(500*sizeof(char))\n\tchar *temp14c = malloc(1500*sizeof(char))\n");
                printf("\tfree(temp14b)\n\tchar *temp14d = malloc(450*sizeof(char))\n");
                char *temp14a = malloc(3000*sizeof(char));
                char *temp14b = malloc(500*sizeof(char));
                char *temp14c = malloc(1500*sizeof(char));
                printf("free(temp14b)\n");
                free(temp14b);
                printf("malloc temp14d\n");
                char *temp14d = malloc(450*sizeof(char));
                free(temp14a);
                free(temp14c);
                free(temp14d);
                printf("Test Case 14 Complete.\n\n");
                break;
                }
            case 15: //malloc of entire area, try to malloc more
                {
                printf("Test Case 15: \n\tchar *temp15a = malloc(5000*sizeof(char))\n");
                printf("\tchar *temp15b = malloc(sizeof(char))\n");
                char *temp15a = malloc(5000*sizeof(char));
                printf("Mallocing too much mem\n");
                char *temp15b = malloc(sizeof(char));
                free(temp15a);
                printf("Test Case 15 Complete.\n\n");
                break;
                }
            case 16: //malloc most of the area, try to malloc more than what's left
                {
                printf("Test Case 16: \n\tchar *temp16a = malloc(1000*sizeof(char))\n");
                printf("\tchar *temp16b = malloc(2000*sizeof(char))\n\tchar *temp16c = malloc(1580*sizeof(char))\n");
                printf("\tfree(temp16b)\n\tchar *temp16d = malloc(2200*sizeof(char))\n");
                char *temp16a = malloc(1000*sizeof(char));
                char *temp16b = malloc(2000*sizeof(char));
                char *temp16c = malloc(1580*sizeof(char));
                free(temp16b);
                printf("Mallocing temp16d\n");
                char *temp16d = malloc(2200*sizeof(char));
                free(temp16a);
                free(temp16c);
                printf("Test Case 16 Complete.\n\n");
                break;
                }
            case 17: //test of calloc
                {
                printf("Test Case 17: \n\tchar *temp17 = calloc(1000*sizeof(char))\n");
                printf("\tfree(temp17)\n");
                char *temp17 = calloc(1000*sizeof(char));
                free(temp17);
                printf("Test Case 17 Complete.\n\n");
                break;
                }
            case 18: //free a ptr not previously returned by malloc/calloc
                {
                printf("Test Case 18: \n\tint *temp18 = malloc(10*sizeof(int))\n");
                printf("\tfree(temp18 + 4)\n");
                int *temp18 = malloc(10*sizeof(int));
                free(temp18 + 4);
                free(temp18);
                printf("Test Case 18 Complete.\n\n");
                break;
                }
            case 19: //free a ptr not in the heap
                {
                printf("Test Case 19: \n\tint *temp19 = malloc(10*sizeof(int))\n");
                printf("\tfree(temp19 - 5)\n");
                int *temp19 = malloc(10*sizeof(int));
                free(temp19 - 5);
                free(temp19);
                printf("Test Case 19 Complete.\n\n");
                break;
                }
            case 99: //System information for testing
                {
                printf("size of char = %i\n", sizeof(char));
                printf("size of int = %i\n", sizeof(int));
                break;
                }
            default: //None of the above
                {
                printf("Error: Bad input.\n");
                break;
                }
        }
    
    
    }
    
    return 0;
}
