#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

static void printChar(char *);

// Semaforo de mutua exclusión para acceder a la RC: funcion "putc"
pthread_mutex_t PRINT_CHAR_MUTEX;

int main(void)
{
    pthread_t thread1;
    pthread_t thread2;

    // Inicializo el semaforo
    pthread_mutex_init(&PRINT_CHAR_MUTEX, NULL);

    pthread_create(&thread1, NULL, (void *)printChar, "Now he has\n");
    pthread_create(&thread2, NULL, (void *)printChar, "two problems.\n");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Elimino el semaforo para liberar recursos
    pthread_mutex_destroy(&PRINT_CHAR_MUTEX);

    return EXIT_SUCCESS;
}

static void printChar(char *str)
{
    char *ptr;
    int c;
    setbuf(stdout, NULL);

    pthread_mutex_lock(&PRINT_CHAR_MUTEX);      // Ingreso a la RC
    for (ptr = str; (c = *ptr++) != 0;)
        putc(c, stdout);                        // Uso de la RC

    pthread_mutex_unlock(&PRINT_CHAR_MUTEX);    // Salida de la RC
}
