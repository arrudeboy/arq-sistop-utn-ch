# Processes and Threads

#### Objetivo
Diferenciar proceso / proceso hijo / hilos

#### Programa
Al iniciar, el programa crea un nuevo proceso usando `fork()`. Paso seguido ambos procesos imprimen su PID, PPID, y TID. Luego el proceso hijo crea dos hilos, los cuales imprimen por pantalla la misma información.

#### Compilar
```bash
gcc -g -o processes_and_threads.out processes_and_threads.c -l pthread
```

#### Análisis
1. Con esa info, podes darte cuenta que proceso creó a quien? Y que proceso es dueño de cada hilo? Comandos sugeridos para analisis:
    - `htop`, podes usar F4 para filtrar por nombre de programa y F5 para mostrar en forma de arbol
    - `pstree -p pidProceso`

2. Observar los PID's y TID's. Investigar como Linux implementa los hilos a nivel kernel.
3. Revisar el valor que tiene la variable global en cada caso y compararlo con la cantidad de veces que fue modificada.
