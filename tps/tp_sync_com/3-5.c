#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

struct params
{
    int numbers_size;
    int *numbers;
    char operation[11];
};

int restar(int array[], int n)
{
    int sub = array[0];
    int index = 1;

    // Utilizo un <label> al cual llame "repeat" para realizar la resta, como relacionan este conceto con lo visto en assembler sobre conditional/unconditional jumps?
    // https://www.geeksforgeeks.org/sum-of-n-elements-without-loops-and-recursion/
    repeat:
        sub -= array[index++];
        if (index < n)
        {
            goto repeat;
        }

        return sub;
}

int sumar(int array[], int n)
{
    // En este caso utilizo otra estrategia diferente a la anterior para realizar la suma: llamar recursivamente a la misma funcion (la funcion "restar" se llama a si misma)
    // con diferentes pametros. Es un concepto de programacion un poco mas complejo, los invito a explorarlo. https://www.geeksforgeeks.org/program-find-sum-elements-given-array/
    if (n == 0)
    {
        return 0;
    }
    else
    {
        return array[0] + sumar(array + 1, n - 1);
    }
}

double dividir(int array[], int n)
{
    double resultado = (double) array[0];
    for (int i = 1; i < n; i++)
    {
        double numb = (double) array[i];
        if (numb != 0.0)
        {
            resultado /= numb;
        }
        else
        {
            puts("Se ha intentado hacer division por cero, abortando el programa\n");
            return 0.0;
        }
    }

    return resultado;
}

int multiplicar(int array[], int n)
{
    int resultado = 0;
    for (int i = 0; i < n; i++)
    {
        resultado *= array[i];
    }

    return resultado;
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Usage: %s $number1, $number2 ... $numberN, $operation ('sumar', 'restar', 'multiplicar', 'dividir')\n", argv[0]);
        return EXIT_FAILURE;
    }

    // declaro mi variable de tipo struct 'params' donde voy a poner los args que mando el usuario (numeros y operacion)
    struct params input;

    // calculo la cantidad de numeros que paso el usuario por argumentos
    input.numbers_size = argc - 2;

    // solicito un puntero a una secuencia de bytes (arrays) donde voy a poner los numeros
    input.numbers = (int *)malloc(input.numbers_size * sizeof(int));
    for (int i = 0; i < input.numbers_size; i++)
    {
        input.numbers[i] = atoi(argv[i + 1]);
    }

    // guardo en mi variable struct 'params' la operacion indicada
    strncpy(input.operation, argv[argc - 1], sizeof(input.operation) - 1);
    input.operation[sizeof(input.operation) - 1] = '\0';

    // declaro el pipe, estructuralmente es un array (de 2 posiciones) para almacenar los descriptores de archivo del pipe (uno para leer y otro para escribir)
    int the_pipe[2];

    // inicializo el pipe y controlo si fallo su creacion
    if (pipe(the_pipe) == -1)
    {
        perror("Error al crear el pipe");
        exit(EXIT_FAILURE);
    }

    // Crear otro proceso (que va a ser el encargado de leer los datos de entrada y llevar a cabo la operacion)
    pid_t pid;
    pid = fork();
    if (pid == -1)
    {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        printf("Soy el proceso 2 y voy a leer por un pipe tus numeros para realizar la operacion '%s'\n", input.operation);
        // cierro el "canal" de escritura del pipe, para asegurar que el otro proceso no mande nada mientras estoy leyendo en el pipe (mutua exclusion)
        close(the_pipe[1]);

        // en el proceso nuevo leo los inputs escritos por el proceso padre en el pipe
        struct params input_leido;
        read(the_pipe[0], &input_leido.numbers_size, sizeof(input_leido.numbers_size));
        input_leido.numbers = (int *)malloc(input_leido.numbers_size * sizeof(int));
        read(the_pipe[0], input_leido.numbers, input_leido.numbers_size * sizeof(int));
        read(the_pipe[0], input_leido.operation, sizeof(input_leido.operation));

        close(the_pipe[0]); // cierro el extremo de lectura del pipe

        double result = 0.0;
        switch (input_leido.operation[0])
        {
        case 's':
            result = sumar(input_leido.numbers, input_leido.numbers_size);
            break;
        case 'r':
            result = restar(input_leido.numbers, input_leido.numbers_size);
            break;
        case 'm':
            result = multiplicar(input_leido.numbers, input_leido.numbers_size);
            break;
        case 'd':
            result = dividir(input_leido.numbers, input_leido.numbers_size);
            break;
        default:
            puts("Operacion no reconocida, debes elegir alguna de estas: 'sumar', 'restar', 'multiplicar' o 'dividor'\n");
            break;
        }

        printf("Resultado: %.2f\n", result);
        free(input_leido.numbers);
    }
    else
    {
        printf("Soy el proceso 1 y voy a enviar por un pipe tus numeros para realizar la operacion '%s'\n", input.operation);
        close(the_pipe[0]);

        write(the_pipe[1], &input.numbers_size, sizeof(input.numbers_size));
        write(the_pipe[1], input.numbers, input.numbers_size * sizeof(int));
        write(the_pipe[1], input.operation, sizeof(input.operation));

        close(the_pipe[1]);
        wait(NULL); // Esperar a que el proceso hijo termine
    }

    free(input.numbers);
    return 0;
}
