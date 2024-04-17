#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

// semaforo contador para controlar la capacidad de la cochera
sem_t *COCHERA;

// semaforo mutex para controlar el uso del montacarga
pthread_mutex_t MONTACARGA;

// declaracion de funciones del programa. El lenguaje C requiere declarar antes y por separado, la "firma" de una funcion y la implementacion de la misma
int open_semaphore(sem_t **semaphore, char semName[], int initialValue);
static void estadia_en_la_cochera(int *thread_number);
static void entrar_a_la_cochera(int *thread_number);
static void salir_de_la_cochera(int *thread_number);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s $autos\nDonde $autos es la cantidad de autos que quieres interactuen con la cochera.\n", argv[0]);
        return EXIT_FAILURE;
    }

    // inicializo el semaforo que controla la capacidad de la cochera con valor 10 (cantidad max de lugares)
    int failedCochera = open_semaphore(&COCHERA, "cochera", 10);

    // inicializo el semaforo mutex que controla el acceso al montacarga (ocupado/libre). Por defecto un mutex se inicializa en 1 (libre)
    pthread_mutex_init(&MONTACARGA, NULL);

    // cantidad de autos (threads) que van a interactuar con la cochera en esta ejecucion
    int threads;
    if (sscanf(argv[1], "%i", &threads) != 1)
    {
        fprintf(stderr, "ERROR - el argumento 'autos' debe ser un numero\n");
    }
    pthread_t tid[threads][2];

    // inicializo los autos (threads) diciendoles que llamen a la funcion 'estadia_en_la_cochera'
    for (int i = 0; i < threads; i++)
    {
        tid[i][1] = i + 1;
        pthread_create(&tid[i][0], NULL, (void *)estadia_en_la_cochera, &tid[i][1]);
    }

    // espero a la finalizacion de cada auto
    for (int i = 0; i < threads; i++)
    {
        pthread_join(tid[i][0], NULL);
    }

    // destruyo los semaforos ya que no los voy a utilizar mas
    sem_unlink("cochera");
    pthread_mutex_destroy(&MONTACARGA);

    return EXIT_SUCCESS;
}

static void estadia_en_la_cochera(int *thread_number)
{
    printf("Auto %i intentando ENTRAR a la cochera...\n", *thread_number);
    entrar_a_la_cochera(thread_number);
    puts("");
    puts("");

    int cant_lugares_libres_en_la_cochera;
    sem_getvalue(COCHERA, &cant_lugares_libres_en_la_cochera);
    printf("Cantidad de lugares disponibles en la cochera: %d\n", cant_lugares_libres_en_la_cochera);
    puts("");
    puts("");

    sleep(10); // permanece 10 segundos en la cochera

    printf("Auto %i intentando SALIR a la cochera...\n", *thread_number);
    salir_de_la_cochera(thread_number);
    puts("");
    puts("");
}

static void entrar_a_la_cochera(int *thread_number)
{

    // Solicito acceso a la cochera
    sem_wait(COCHERA);

    // Solicito utilizar el montacarga
    pthread_mutex_lock(&MONTACARGA);

    printf("Auto %i ENTRO a la cochera...\n", *thread_number);

    // Libero Montacarga
    pthread_mutex_unlock(&MONTACARGA);
}

static void salir_de_la_cochera(int *thread_number)
{

    // Solicito utilizar el montacarga
    pthread_mutex_lock(&MONTACARGA);

    // Libero un lugar en la cochera
    sem_post(COCHERA);

    printf("Auto %i SALIO de la cochera...\n", *thread_number);

    // Libero Montacarga
    pthread_mutex_unlock(&MONTACARGA);
}

int open_semaphore(sem_t **semaphore, char sem_name[], int initial_value)
{
    // O_CREAT significa "Crear el semaforo si no existe, y si existe abrirlo"
    // S_IRWU significa "Si se crea el semaforo, hacerlo con permisos Read|Write|eXecute para el propietario del semaforo"
    sem_t *opennedSemaphore = sem_open(sem_name, O_CREAT, S_IRWXU, initial_value);

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