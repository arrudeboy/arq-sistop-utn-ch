#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int CHILDREN = 2;
int THREADS = 2;

void *increment();
void *decrement();

int counter = 0;

int main(int argc, char **argv)
{
    pid_t pid;
    pthread_t thread1, thread2;

    for (int i = 0; i < CHILDREN; i++) 
    {
        int pid = fork();
        if (pid == 0) // CHILDREN
        {
            increment();
            exit(EXIT_SUCCESS);
        }
    }

    // PARENT
    pthread_create(&thread1, NULL, decrement, NULL);
    pthread_create(&thread2, NULL, decrement, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return EXIT_SUCCESS;
}

void *increment() {
    pid_t pid, ppid;

    pid = getpid();
    ppid = getppid();
    counter++;

    printf("I'm a process...\n");
    printf("PPID: %d\nPID: %d\nIncrementing 'counter'... counter = %d\n\n", ppid, pid, counter);

    return EXIT_SUCCESS;
}

void *decrement() {
    pid_t pid, ppid, tid;

    pid = getpid();
    ppid = getppid();
    tid = syscall(SYS_gettid);
    counter--;

    printf("I'm a thread...\n");
    printf("PPID: %d\nPID: %d\nTID: %d\nDecrementing 'counter'... counter = %d\n\n", ppid, pid, tid, counter);

    sleep(20); // add sleep here in order to catch the threads with watch/pstree command
    return EXIT_SUCCESS;
}
