/*
    SOLUCION TEORICA

    SEMAFOROS:
     - cochera: "contador" inicializado en 10
     - primer_subsuelo: "contador" inicializado en 2
     - montacarga1: "mutex" (usado para bajar y subir autos entre pb <-> 1er subs.)
     - montacarga2: "mutex" (usado para bajar y subir autos entre 1er subs. <-> 2do subs.)

    1) ENTRAR

    WAIT(primer_subsuelo)
    WAIT(montacarga1)
    bajar_al_primer_subsuelo()
    entrar_al_primer_subsuelo()
    SIGNAL(montacarga1)
    
    WAIT(cochera)
    WAIT(montacarga2)
    bajar_al_segundo_subsuelo()
    entrar_a_la_cochera()
    SIGNAL(montacarga2)
    SIGNAL(primer_subsuelo)

    2) SALIR

    WAIT(primer_subsuelo)
    WAIT(montacarga2)
    subir_al_primer_subsuelo()
    entrar_al_primer_subsuelo()
    SIGNAL(montacargas2)
    SIGNAL(cochera)

    WAIT(montacarga1)
    subir_a_planta_alta()
    SIGNAL(montacarga1)
    SIGNAL(primer_subsuelo)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

// semaforo contador para controlar la capacidad (10) de la cochera
sem_t *COCHERA;

// semaforo contador para controlar la capacidad (2) del primer subsuelo
sem_t *PRIMER_SUBSUELO;

// semaforo mutex para controlar los montacargas (MONTACARGA1 = PB <-> PRIMER_SUBSUELO; MONTACARGA2 = PRIMER_SUBSUELO <-> COCHERA)
pthread_mutex_t MONTACARGA1;
pthread_mutex_t MONTACARGA2;

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

    // inicializo los semaforos que controlan la capacidad de la cochera (10) y la capacidad del primer subsuelo (2)
    int failedCochera = open_semaphore(&COCHERA, "cochera", 10);
    int failedPrimerPiso = open_semaphore(&PRIMER_SUBSUELO, "primer_subsuelo", 2);
    if (failedCochera == -1 || failedCochera == -1) {
        puts("Error al crear los semaforos contadores");
        return EXIT_FAILURE;
    }

    // inicializo los semaforos mutex que controlan el acceso a los montacargas (ocupado/libre). Por defecto un mutex se inicializa en 1 (libre)
    pthread_mutex_init(&MONTACARGA1, NULL);
    pthread_mutex_init(&MONTACARGA2, NULL);

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
    sem_unlink("primer_subsuelo");
    pthread_mutex_destroy(&MONTACARGA1);
    pthread_mutex_destroy(&MONTACARGA2);

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

    sleep(1); // permanece 10 segundos en la cochera

    printf("Auto %i intentando SALIR a la cochera...\n", *thread_number);
    salir_de_la_cochera(thread_number);
    puts("");
    puts("");
}

static void entrar_a_la_cochera(int *thread_number)
{
    /*
    WAIT(primer_subsuelo)
    WAIT(montacarga1)
    bajar_al_primer_subsuelo()
    entrar_al_primer_subsuelo()
    SIGNAL(montacarga1)
    ---------------------------
    WAIT(cochera)
    WAIT(montacarga2)
    bajar_al_segundo_subsuelo()
    entrar_a_la_cochera()
    SIGNAL(montacarga2)
    SIGNAL(primer_subsuelo)
    */

    // Solicito acceso al primer subsuelo
    sem_wait(PRIMER_SUBSUELO);
    // Solicito utilizar el montacarga1 (que opera entre la planta baja y el primer subsuelo)
    pthread_mutex_lock(&MONTACARGA1);

    printf("Auto %i ENTRO al primer subsuelo...\n", *thread_number);

    // Libero montacarga1
    pthread_mutex_unlock(&MONTACARGA1);

    //--------------------------------------------------------------------------------------

    // Solicito acceso a la cochera
    sem_wait(COCHERA);

    // Solicito utilizar el montacarga2 (que opera entre el primer subsuelo y la cochera)
    pthread_mutex_lock(&MONTACARGA2);

    printf("Auto %i ENTRO a la cochera...\n", *thread_number);

    // Libero montacarga2
    pthread_mutex_unlock(&MONTACARGA2);

    // Como ya estoy en la cochera, libero el lugar que habia ocupado antes en el primer subsuelo
    sem_post(PRIMER_SUBSUELO);
}

static void salir_de_la_cochera(int *thread_number)
{
    /*
    WAIT(primer_subsuelo)
    WAIT(montacarga2)
    subir_al_primer_subsuelo()
    entrar_al_primer_subsuelo()
    SIGNAL(montacargas2)
    SIGNAL(cochera)
    --------------------------
    WAIT(montacarga1)
    subir_a_planta_alta()
    SIGNAL(montacarga1)
    SIGNAL(primer_subsuelo)
    */

    // Consulto si hay lugar en el primer subsuelo, sino espero
    sem_wait(PRIMER_SUBSUELO);

    // Solicito utilizar el montacarga2 (para subir al primer subsuelo)
    pthread_mutex_lock(&MONTACARGA2);

    printf("Auto %i SALIO de la cochera...\n", *thread_number);

    // Libero montacarga2
    pthread_mutex_unlock(&MONTACARGA2);

    // Libero un lugar en la cochera
    sem_post(COCHERA);

    //----------------------------------------------------------------

    // Solicito utilizar el montacarga1 (para subir a la planta baja)
    pthread_mutex_lock(&MONTACARGA1);

    printf("Auto %i SALIO del primer subsuelo...\n", *thread_number);

    pthread_mutex_unlock(&MONTACARGA1);

    // Libero el lugar del primer subsuelo... me re fui, chau
    sem_post(PRIMER_SUBSUELO);
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