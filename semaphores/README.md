# Sempahores :vertical_traffic_light:

#### Objetivo
Comprender el uso de las primitivas que ofrece un semaforo:
 - __Wait__
 - __Signal__
 - Otras: __Init__, Destroy?, etc

#### Programa
Programa en C que utiliza la implementacion de semaforos de Linux. 
Basicamente el programa permite ejecutar una accion sobre un semaforo. Las acciones disponibles son:
 - __"value"__ :arrow_right: permite observar el valor actual del semaforo
 - __"wait"__ :arrow_right: solicita acceder al recurso (region critica). Si existe disponibilidad del recurso, entonces lo obtiene y finaliza, caso contrario se bloquea.
 - __"signal"__ :arrow_right: libera un recurso previamente obtenido mediante "wait" (sale de la region critica).

 #### Compilar
 ```bash
gcc -g -o semaphores.out semaphores.c -l pthread

```

#### Uso (ejemplos)
- Solicito ver el valor del semaforo "sem1":
    ```bash
    ./semaphores.out sem1 value
    ```
    Output:
    > Getting current value...
    > Current value: 0

- Solicito acceder al recurso que "sem1" protege:
    ```bash
    ./semaphores.out sem1 wait
    ```
    Output:
    > Doing wait...
    > Wait done

- Libero recurso protegido por "sem1":
    ```bash
    ./semaphores.out sem1 signal
    ```
    Output:
    > Doing signal...
    > Signal done

#### Playground
1. Ejecutar `./semaphores.out semaforo1 value` (deberia crear un semaforo y leer su valor, inicialmente en 1).
2. Ejecutar el programa pero esta vez enviando la operacion "wait". Verificar lo que sucede. Repetir la operacion una vez mas. Que sucedio?
3. Ejecutar desde otra terminal el mismo programa pero ahora con la operacion "signal". Verificar que sucedio con el programa anterior.
4. En una tercer terminal, ejecute el comando `watch ./semaphores.out semaforo1 value` (esto ejecutara el comando permanentemente). Repita todos los pasos previos y vaya validando como cambia el valor del semaforo.
5. Que sucedio con el valor mostrado por la operacion "value" cuando al menos un proceso se encontraba bloqueado? Investigue una forma de averiguar cuantos procesos se encuentran bloqueados en Linux por un semaforo.
    > _tip_: recuerde que los semaforos en Linux son archivos
4. En Linux existen los "named semaphores" y los "unnamed sepahores". De cual tipo son los usados en el ejemplo? Cuando convendria usar un tipo o el otro?

#### Challenge
- Permita que el programa pueda ahora procesar una nueva operacion llamada __"destroy"__, la cual debe eliminar al semaforo del sistema operativo 
    >_tip_: investigue si existe alguna funcion de la libreria `<semaphore.h>` para hacer esto; similar a las vistas: sem_wait, sem_post, sem_init, etc.

- El codigo del programa presenta algunas malas practicas de programacion, las cuales lo hacen poco legible, poco mantenible, poco escalable e ineficiente. Piense de que forma puede refactorizar el codigo actual para lograr mitigar esas deficiencias mencionadas.
