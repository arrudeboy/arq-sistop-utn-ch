# Race Condition

#### Objetivo
Determinar que parte del código representa la __Región Crítica (RC)__, es decir, donde puede darse una __Condición de Carrera__ (_Race Condition_) entre hilos o procesos.

#### Programas
1. __`procesess_race_condition.c`__: el proceso principal crea un proceso hijo y luego ambos llaman a la funcion `printChar`, la cual expone una condicion de carrera entre ambos.
2.  __`threads_race_condition.c`__: el proceso principal crea 2 threads y ambos llaman a la funcion `printChar`, la cual expone una condicion de carrera entre ambos.
3. __`threads_race_condition_with_sem.c`__: el proceso principal crea 2 threads y ambos llaman a la funcion `printChar`, la cual expone una condicion de carrera entre ambos. Pero en este caso se utiliza un semaforo mutex para garantizar que solo un único thread puede ejecutar la RC de `printChar` (la instrucción `putc`).

#### Compilar
```bash
gcc -g -o processes_race_condition.out processes_race_condition.c -l pthread
gcc -g -o threads_race_condition.out threads_race_condition.c -l pthread
gcc -g -o threads_race_condition_with_sem.out threads_race_condition_with_sem.c -l pthread
```

#### Pasos a seguir
1. Compilar los programas de threads
2. Ejecutar varias veces el programa `threads_race_condition.out` y observar que el resultado no siempre es el esperado (las frases se imprimen con distinto orden e intercalando letras).
3. Decompile el programa en instrucciones de Assembler, usando el comando `objdump -d threads_race_condition.out | less`
4. Navegue por el codigo assembler usando los cursores, y busque el codigo correspondiente a la funcion "printChar" (puede buscar texto tipeando `/printChar <enter>`)
5. Intente encontrar las instrucciones correspondientes a la manipulación de la funcion critica `putc`
6. Ejecutar varias veces el programa `threads_race_condition_with_sem.out` y observar que el resultado siempre es el esperado (se escriben las frases en orden).
7. Compare ambos códigos en C vea la diferencia
8. ¿Cual de los dos programas ejecuta más rapido?

