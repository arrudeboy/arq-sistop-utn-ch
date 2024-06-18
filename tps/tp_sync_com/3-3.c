/*
    SOLUCION TEORICA

    SEMAFOROS:
     - impresora: mutex

    PROTOCOLO DE SYNC:
     - WAIT(impresora)
     - imprimir_trabajos($cant_trabajos)
     - SIGNAL(impresora)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

pthread_mutex_t IMPRESORA;

static void imprimir_trabajos(void *params);

// estructura de datos que voy a pasarle como arg a la funcion imprimir trabajos
// esta estructura tiene dos campos: el thread id (o numero de usuario) y la cantidad de trabajos a imprimir para ese usuario
struct params
{
    int thread_id;
    int cant_trabajos;
};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s $user1_jobs, $user2_jobs, ... $userN_jobs \nDonde $user(K)_jobs representa la cant. de trabajos a imprimir por ese usuario\n", argv[0]);
        return EXIT_FAILURE;
    }

    pthread_mutex_init(&IMPRESORA, NULL);

    // chequeo que los argumentos sean de tipo numerico (son cant. de trabajos que imprimira cada usuario)
    for (int i = 1; i <= argc - 1; i++)
    {
        if (sscanf(argv[i], "%i", &i) != 1)
        {
            fprintf(stderr, "ERROR - los argumentos 'user(K)_jobs' deben ser de tipo numerico\n");
            return EXIT_FAILURE;
        }
    }

    // inicializo los usuarios (threads / usuairos) que van mandar sus trabajos a la impresora
    int threads = argc - 1;
    pthread_t tid[threads];
    struct params t_params[threads];
    for (int i = 0; i < threads; i++)
    {
        t_params[i].thread_id = i + 1;
        t_params[i].cant_trabajos = atoi(argv[i + 1]);
        pthread_create(&tid[i], NULL, (void *)imprimir_trabajos, &t_params[i]);
    }

    for (int i = 0; i < threads; i++)
    {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&IMPRESORA);

    return EXIT_SUCCESS;
}

static void imprimir_trabajos(void *t_params)
{

    struct params *t_params_reader = t_params;
    int cant_trabajos = t_params_reader->cant_trabajos;
    int thread_id = t_params_reader->thread_id;

    printf("Solicitando imprimir trabajos del usuario %i...\n", thread_id);
    pthread_mutex_lock(&IMPRESORA);

    printf("Imprimiendo %i trabajos del usuario %i...\n", cant_trabajos, thread_id);
    sleep(cant_trabajos);

    pthread_mutex_unlock(&IMPRESORA);
}