/*
    SOLUCION TEORICA 4

    SEMAFOROS:
     - lugar_en_deposito: contador (inicializado en 50) => disponibilidad actual en kg del deposito, a escala 1:100
     - mutex: mutex => para colocar carbon en el deposito o transportarlo cuando esté lleno

    VARIABLES GLOBALES:
     - count: int (cuenta la cantidad de kg de carbon colocados en el deposito, a escala 1:100)

    HILOS:
     - mineros (n)
     - camion (1)

    PROTOCOLO DE SINCRONIZACION:

     - Minero:

        > WAIT(lugar_en_deposito)
        > colocar_carbon_en_el_deposito()  ---> sleep(10)
        > WAIT(mutex)
        > while(count == 50)
            > bloquearse y esperar recibir la signal 'cond_vacio' y liberar(mutex)
            > cuando reciba dicha signal, se le vuelve a otorgar el mutex

        > count++
        > SIGNAL(lugar_en_deposito)
        > if (count == 50)
            > enviar signal de 'cond_lleno'

        > SIGNAL(mutex)

     - Camion:

        > WAIT(mutex)
        > while(count < 50)
            > bloquearse y esperar recibir la signal 'cond_lleno' y liberar(mutex)
            > cuando reciba dicha signal, se le vuelve a otorgar el mutex

        > transportar_deposito_fuera_de_la_mina()  ---> sleep(5) + sleep(2)
        > count = 0
        > enviar signal de 'cond_vacio' (broadcast)
        > SIGNAL(mutex)
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>

#define CAPACITY 10

// signal para finalizar todos los hilos (Ctrl + C)
volatile sig_atomic_t running = 1;

// variable global que cuenta cuantos mineros estan colocando carbon en el deposito simultaneamente
int count = 0;

// semaforo contador (decreciente), inicializado en la cantidad maxima de mineros admitidos para colocar carbon en el deposito simultaneamente
sem_t *LUGAR_EN_DEPOSITO;

/* 
    Condicion de deposito vacio o "deposito aun no lleno",
    
    El thread que ejecuta la instruccion pthread_cond_wait(cond_vacio...) se bloquea a la espera de la signal 'cond_vacio'. Al momento
    de bloquearse, libera el mutex involucrado en el llamado a pthread_cond_wait.
*/
pthread_cond_t cond_vacio;

/* 
    Condicion de deposito lleno o "deposito aun no vacio",
    
    El thread que ejecuta la instruccion pthread_cond_wait(cond_lleno...) se bloquea a la espera de la signal 'cond_lleno'. Al momento
    de bloquearse, libera el mutex involucrado en el llamado a pthread_cond_wait.
*/
pthread_cond_t cond_lleno;

// semaforo mutex para sincronizar los threads mineros (entre si) y el thread camion
pthread_mutex_t mutex;

int open_semaphore(sem_t **semaphore, char semName[], int initialValue);
void signal_handler(int signal);
void colocar_carbon_en_deposito(int *minero_id);
void *transportar_deposito_fuera_de_la_mina();

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s ./a.out ${cant_mineros}\n", argv[0]);
        return EXIT_FAILURE;
    }

    int mineros;
    if (sscanf(argv[1], "%i", &mineros) != 1)
    {
        fprintf(stderr, "ERROR - el argumento 'mineros' debe ser un numero\n");
    }

    int lugar_en_deposito = open_semaphore(&LUGAR_EN_DEPOSITO, "lugar_en_deposito", CAPACITY);
    if (lugar_en_deposito == -1)
    {
        puts("Error al crear semaforo contadore: LUGAR_EN_DEPOSITO");
        return EXIT_FAILURE;
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_vacio, NULL);
    pthread_cond_init(&cond_lleno, NULL);
    signal(SIGINT, signal_handler);

    pthread_t t_mineros[mineros][2];
    for (int i = 0; i < mineros; i++)
    {
        t_mineros[i][1] = i + 1;
        pthread_create(&t_mineros[i][0], NULL, (void *)colocar_carbon_en_deposito, &t_mineros[i][1]);
    }

    pthread_t t_camion;
    pthread_create(&t_camion, NULL, transportar_deposito_fuera_de_la_mina, NULL);

    for (int i = 0; i < mineros; i++)
    {
        pthread_join(t_mineros[i][0], NULL);
    }

    pthread_join(t_camion, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_vacio);
    pthread_cond_destroy(&cond_lleno);

    return EXIT_SUCCESS;
}

void colocar_carbon_en_deposito(int *minero_id)
{
    while (running)
    {
        sem_wait(LUGAR_EN_DEPOSITO);
        printf("Minero %i COLOCANDO 100 kg en el deposito...\n", *minero_id);
        sleep(10);

        pthread_mutex_lock(&mutex);
        
        while (count == CAPACITY) {
            pthread_cond_wait(&cond_vacio, &mutex);
        }
        count++;
        printf("Minero %i HA COLOCADO 100 kg en el deposito. Nivel del deposito: %d kg\n", *minero_id, count * 100);
        sem_post(LUGAR_EN_DEPOSITO);

        if (count == CAPACITY) {
            printf("El deposito HA SIDO COMPLETADO por el minero %i, listo para ser transportado por el camion...\n", *minero_id);
            pthread_cond_signal(&cond_lleno);
        }

        pthread_mutex_unlock(&mutex);
        sleep(5);
    }

    printf("Minero %i terminando.\n", *minero_id);
}

void *transportar_deposito_fuera_de_la_mina()
{
    while (running)
    {
        pthread_mutex_lock(&mutex);

        while (count < CAPACITY) {
            puts("Camion LISTO para transportar deposito fuera de la mina... debe esperar a que el deposito esté lleno (mineros aun trabajando)\n");
            pthread_cond_wait(&cond_lleno, &mutex);
        }

        puts("Camion TRANSPORTANDO deposito fuera de la mina...\n");
        sleep(7);

        count = 0;
        pthread_cond_broadcast(&cond_vacio);
        
        pthread_mutex_unlock(&mutex);
        puts("Deposito transportado\n");
    }

    puts("Camion terminando.");
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

void signal_handler(int signal)
{
    running = 0;
}