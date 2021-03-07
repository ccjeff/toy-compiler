#include <stdio.h>
#include <stdlib.h>
#include "../translate.h"

int main(){
    char* testString = generate("ADD",12,"A2","");
    printf("testString is: %s \n", testString);

    return 0;
}