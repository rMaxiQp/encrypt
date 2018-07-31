#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define INT(str) (strtol(str, (char**) NULL, 10))
#define VALIDATE(cand, sub) ((cand < 0) ? (cand = sub) : (cand = cand))

void print_usage() {
    fprintf(stderr, "\npassGenerator [length] [# digit] [# CAPS] [# lower]\n\n");
    exit(1);
}

const char digit[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
const char cap[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
const char lower[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
const char special[] = {' ', '!', '\"', '#', '$', '%', '&', '(', ')', '*', '+', ',', '-', '.','/', ':',';','<','=','>','\?','@','[','\\',']','^','_','`','{','|','}','~', '\''};


char * randomize(const char origin[], int length, int iteration) {
    char * retval = calloc(1, (1 + iteration) * sizeof(char));
    time_t t;
    srand((unsigned ) time(&t));
    for (int i = 0; i < iteration; i++) {
        retval[i] = origin[rand() % length];
    }
    return retval;
}

void shuffle(char* array, int length) {
    time_t t;
    srand((unsigned) time(&t));
    for (int i = length - 1; i > 0; i--) {
        char tmp = array[i];
        int random = rand() % i;
        array[i] = array[random];
        array[random] = tmp;
    }
}

int main(int argc, char *argv[]) {
    if(argc > 5) { print_usage();}

    //assign values
    int length, num_digit, num_cap, num_lower, num_special;
    length = (argc > 1) ? INT(argv[1]): 10;
    if(length > 256) {
        fprintf(stderr, "\nTHE PASSWORD IS TOO LONG (MAX 256)\n\n");
        exit(1);
    }
    num_digit = (argc > 2) ? INT(argv[2]) : 3;
    num_cap = (argc > 3) ? INT(argv[3]) : 3;
    num_lower = (argc > 4) ? INT(argv[4]) : 3;
    VALIDATE(length, 10);
    VALIDATE(num_digit, 3);
    VALIDATE(num_cap, 3);
    VALIDATE(num_lower, 3);
    int remain = length - num_digit - num_cap - num_lower;
    if ( remain < 0) {
        length -= remain;
        remain = 0;
    }
    num_special = remain;

    //generate individual password
    char *ret_digit = randomize(digit, 10, num_digit);
    char *ret_cap = randomize(cap, 26, num_cap);
    char *ret_lower = randomize(lower, 26, num_lower);
    char *ret_special = randomize(special, 33, num_special);
    
    char *result = calloc(1, (length + 1) * sizeof(char));
    int cur = 0;
    for (int i = 0; i < num_digit; i++) { result[cur++] = ret_digit[i]; }
    for (int i = 0; i < num_cap; i++) { result[cur++] = ret_cap[i]; }
    for (int i = 0; i < num_lower; i++) { result[cur++] = ret_lower[i]; }
    for (int i = 0; i < num_special; i++) {result[cur++] = ret_special[i]; }

    shuffle(result, length);

    printf("\n%s\n\n", result);

    //free
    free(ret_digit);
    free(ret_cap);
    free(ret_lower);
    free(ret_special);
    free(result);
    return 0;
}
