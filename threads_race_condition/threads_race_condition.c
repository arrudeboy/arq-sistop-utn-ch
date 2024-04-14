#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

static void printChar(char *);

int main(void) {

	pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, (void*) printChar, "Now he has\n");
    pthread_create(&thread2, NULL, (void*) printChar, "two problems.\n");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

	return EXIT_SUCCESS;
}

static void printChar(char *str)
{
    char *ptr;
    int c;
    setbuf(stdout, NULL);
    for (ptr = str; (c = *ptr++) != 0;)
        putc(c, stdout);
}
