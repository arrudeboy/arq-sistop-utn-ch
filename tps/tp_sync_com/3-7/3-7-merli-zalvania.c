#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CAPACIDAD 5000  
#define CARGA_MINERO 100 
#define TIEMPO_MINERO 10  
#define DESCANSO_MINERO 5  
#define TIEMPO_TRANSPORTE 5 
#define TIEMPO_RETORNO 2   

int carga_actual = 0; 
sem_t lleno;          
sem_t vacio;         
pthread_mutex_t mutex; 

void* minero(void* arg) {
    while (1) {
        sleep(TIEMPO_MINERO);
        pthread_mutex_lock(&mutex);
        if (carga_actual + CARGA_MINERO <= CAPACIDAD) {
            carga_actual += CARGA_MINERO;
            printf("Minero %ld colocó 100 kg. Carga actual: %d kg\n", (long)arg, carga_actual);
        }
        if (carga_actual == CAPACIDAD) {
            sem_post(&lleno);
        }
        pthread_mutex_unlock(&mutex);
        sleep(DESCANSO_MINERO);
    }
}

void* camion(void* arg) {
    while (1) {
        sem_wait(&lleno);
        sleep(TIEMPO_TRANSPORTE);
        printf("Camión está transportando el depósito lleno fuera de la mina.\n");
        sleep(TIEMPO_RETORNO);
        printf("Camión regresó con el depósito vacío.\n");
        pthread_mutex_lock(&mutex);
        carga_actual = 0;
        sem_post(&vacio);
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    pthread_t mineros[5], camion_thread;
    sem_init(&lleno, 0, 0);
    sem_init(&vacio, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    for (long i = 0; i < 5; i++) {
        pthread_create(&mineros[i], NULL, minero, (void*)i);
    }
    pthread_create(&camion_thread, NULL, camion, NULL);

    for (int i = 0; i < 5; i++) {
        pthread_join(mineros[i], NULL);
    }
    pthread_join(camion_thread, NULL);

    sem_destroy(&lleno);
    sem_destroy(&vacio);
    pthread_mutex_destroy(&mutex);

    return 0;
}