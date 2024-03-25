# Process states 

Verificación de los estados que usa Linux para los procesos.

El programa __process_states.c__ alterna entre 3 tareas de forma secuencial:
 1. Solicita operacion de E/S
 2. Solicita tarea de espera activa (_sleep_) 
 3. Solicita uso de CPU

> Esto lo repite en un bucle infinito (`while(1)`) hasta que se finaliza o mata a dicho proceso enviandole una _signal_ (syscall) desde afuera del programa.

La idea es ejecutar el programa e ir revisando los diferentes estados por los cuales va pasando el proceso del programa.

#### Para compilar
```bash
gcc -g -o process_states.out process_states.c
```

#### Para ejecutar
```bash
./process_states.out
```

#### Pasos a seguir
1. Compilar y ejecutar el programa
2. Abrir otra terminal, ejecutar: `ps f` y observar la columna _STAT_ (para ejecutar el mismo comando repetidas veces de manera automatica, ejecutar: `watch ps f`)
3. Investigar el significado de los estados revisando la pagina del manual (`man ps`, seccion _PROCESS STATE CODES_)
4. Observar como el proceso del programa va cambiando de estados segun la tarea que esta ejecutando (E/S, sleep, CPU)
5. "Frenar" el programa a traves del comando `kill -SIGSTOP $PID` (donde `$PID` es el PROCESS ID del programa en ejecucion)
6. "Continuar" la ejecuion del proceso frenado utilizando nuevamente el comando `kill` y la signal que cumpla con dicho proposito (Investigar como!)
7. "Matar" al proceso utilizando nuevamente comando `kill` y la signal que cumpla con dicho proposito (Investigar como!)

#### Preguntas
- ¿Que significa la `s` (minúscula) y el `+` al lado del estado del proceso, en la columna _STAT_?
- ¿Cual estado de Linux se corresponde con "Bloqueado"?
- ¿Cual estado de Linux se corresponde con "Listo"?

