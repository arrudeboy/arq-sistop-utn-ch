/*
 * processes_and_threads.c
 *
 * Fork a process and launch two threads. Then print PID, PPID and TID for each process either thread
 *
 * Compile using 'pthread' library: gcc -o processes_and_threads processes_and_threads.c -l pthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void *printData();
void *printDataAndWait();
int global = 0;

int main(int argc, char **argv)
{

    int pid = fork();
    pthread_t thread1, thread2;

    if (pid == 0)
    { // CHILDREN

        printData();
        pthread_create(&thread1, NULL, printDataAndWait, NULL);
        pthread_create(&thread2, NULL, printDataAndWait, NULL);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
    }
    else if (pid > 0)
    { // PARENT

        printData();
        waitpid(pid, NULL, 0);
    }
    else
    { // Error

        perror("Error forking process...");
    }

    return EXIT_SUCCESS;
}

void *printDataAndWait()
{

    printData();
    sleep(10);

    return EXIT_SUCCESS;
}

void *printData()
{

    pid_t pid, ppid, tid;

    pid = getpid();
    ppid = getppid();
    tid = syscall(SYS_gettid);
    global++;

    printf("PPID: %d\nPID: %d\nTID: %d\nGlobal: %d\n\n", ppid, pid, tid, global);

    return EXIT_SUCCESS;
}
