#include <stdio.h>
#include <unistd.h>

int main(void) {

    puts("Starting program!\n");

    while (1) {
        printf("Requesting I/O operation...");
        fflush(stdout);
        int i = 0;
        for (i = 0; i < 1000000; i++) {
	    printf(".\b");
	}		
        printf("done\n");
        fflush(stdout);

	printf("Going to sleep...");
        fflush(stdout);
	sleep(5);
	printf("done\n");
        fflush(stdout);

	printf("Requesting CPU usage...");
        fflush(stdout);
	int j = 0;
	int r = 0;
	for (i = 0; i < 1000000; i++) {
	    for (j = 0;j<1000;j++) {
		r += 1 + 3 + i + j;
	    }
	}
	printf("done\n");
        fflush(stdout);

        sleep(1);
    }

    return 0;
}
