
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void check(char *temp) {
    // strcpy(temp, "BYE") ;
    printf("%x\n", &temp) ;
    temp = "BYE" ;
    printf("%x\n", &temp) ;

}

int main() {

    char *str = malloc(20) ;
    str = "hello" ;
    // char new[] = str ; 
    printf("%s\n", str) ;
    check(str) ;
    printf("%s\n", str) ;

}