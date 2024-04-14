#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static void printChar(char *);

int main()
{
    int pid;
    if ((pid = fork()) < 0)
        printf("fork error\n");
    else if (pid > 0) // PARENT
        printChar("Now he has\n");
    else // CHILD
        printChar("two problems.\n");
    _exit(0);
}

static void printChar(char *str)
{
    char *ptr;
    int c;
    setbuf(stdout, NULL);
    for (ptr = str; (c = *ptr++) != 0;)
        putc(c, stdout);
}
