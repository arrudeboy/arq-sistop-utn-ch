#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

int openSemaphore(sem_t **semaphore, char *sem_name);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <value|wait|signal>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *sem_name = argv[1];
    char *operation = argv[2];

    sem_t *semaphore;
    int failed = openSemaphore(&semaphore, sem_name);

    if (failed)
    {
        return EXIT_FAILURE;
    }

    if (!strcmp(operation, "wait"))
    {
        puts("> Doing wait...");
        sem_wait(semaphore);
        puts("> Wait done");
    }
    if (!strcmp(operation, "signal"))
    {
        puts("> Doing signal...");
        sem_post(semaphore);
        puts("> Signal done");
    }

    if (!strcmp(operation, "value"))
    {
        puts("> Getting current value...");
        int current_value;
        sem_getvalue(semaphore, &current_value);
        printf("> Current value: %d\n", current_value);
    }

    return EXIT_SUCCESS;
}

int openSemaphore(sem_t **semaphore, char *sem_name)
{
    // O_CREAT significa "Crear el semaforo si no existe, y si existe abrirlo"
    // S_IRWU significa "Si se crea el semaforo, hacerlo con permisos Read|Write|eXecute para el propietario del semaforo"
    int initialValue = 1;
    sem_t *opennedSemaphore = sem_open(sem_name, O_CREAT, S_IRWXU, initialValue);

    if (opennedSemaphore == SEM_FAILED)
    {
        perror("sem_open");
        return -1;
    }
    else
    {
        *semaphore = opennedSemaphore;
        return 0;
    }
}
