# Zombie process

El programa crea un proceso hijo, y luego ambos esperan un tiempo y finalizan (donde el proceso hijo finaliza antes que el proceso padre).

#### Para compilar
```bash
gcc -g -o zombie_process.out zombie_process.c
```

#### Pasos a seguir
1. Ejecutar `watch ps -efo pid,ppid,stat,pmem,rss,cmd`
2. Ejecutar el programa en otra terminal: `./zombie_process.out`
3. Revisar el estado de los procesos

#### Preguntas
* ¿Como podemos definir entonces a un proceso zombie?
* ¿Ocupa memoria en el sistema? ¿Cuanta?
* ¿Que sucede si se intenta matar al proceso usando el comando `kill`?
* Suponga que el proceso hijo quiere indicarle al padre si pudo finalizar bien o mal: ¿Existe alguna forma de lograrlo? (tip: revise la llamada al sistema _waitpid()_)