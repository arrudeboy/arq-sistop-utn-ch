#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void) {

    char myString[] = "HOLA,COMO,ESTAS";
    
    char* valor1 = strtok(myString, ",");
    puts(valor1);
    char* valor2 = strtok(NULL, ",");
    puts(valor2);
    char* valor3 = strtok(NULL, ",");
    puts(valor3);
    char* valor4 = strtok(NULL, ",");
    puts(valor4);

    return 0;
}