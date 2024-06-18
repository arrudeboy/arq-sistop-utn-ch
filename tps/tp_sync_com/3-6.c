#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>

double resultado = 0;

void manejar_signal(int sig) {
    if (sig == SIGUSR1) 
    {   // Señal SIGUSR1: Calcular la potencia al cuadrado
        resultado = pow(resultado, 2);
        printf("Potencia al cuadrado calculada: %.2f\n", resultado);
    } 
    else if (sig == SIGUSR2) 
    {   // Señal SIGUSR2: Calcular la raíz cuadrada
        if (resultado >= 0) 
        {
            resultado = sqrt(resultado);
            printf("Raíz cuadrada calculada: %.2f\n", resultado);
        } 
        else 
        {
            printf("No se puede calcular la raíz cuadrada de un número negativo.\n");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <numero>\n", argv[0]);
        return 1;
    }

    double numero = atof(argv[1]);
    resultado = numero;

    printf("PID para enviar señales con kill: %ld\n\n", (long) getpid());

    printf("Número inicial: %.2f\n\n", resultado);

    // Establece el manejador de señales
    signal(SIGUSR1, manejar_signal);
    signal(SIGUSR2, manejar_signal);

    printf("Envíe la señal SIGUSR1 para calcular la potencia al cuadrado.\n");
    printf("Envíe la señal SIGUSR2 para calcular la raíz cuadrada.\n");

    // Bucle infinito para mantener el programa en ejecución
    while (1) {
        // Espera activa para permitir que el programa continúe ejecutándose
    }

    return 0;
}
