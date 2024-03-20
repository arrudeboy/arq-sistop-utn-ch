# Syscall Wrappers

En este ejemplo vemos en practica diferentes formas de invocar a una syscall:
1. A traves de una funcion Wrapper
2. Invocando directamente a la rutina de Syscall

El programa realiza una llamada a una función de la [libreria estandar de C](https://es.wikipedia.org/wiki/Biblioteca_est%C3%A1ndar_de_C) para imprimir un mensaje por pantalla.
Dado que esta funcion no es propiamente una syscall, sino un wrapper, internamente deberia en algun momento invocar a la rutina del syscall que permite hacer efectivamente la impresion por pantalla (es una operacion de E/S).

Para observar que llamadas a syscalls realiza la funcion de C (el wrapper) vamos a utiizar el comando `strace`.

#### Para compilar
```bash
gcc -g hello_world.c
```

#### Para ejecutar
```bash
./a.out
```

#### Analisis
* Analizar el comportamiento del programa. Describa lo que ocurrio.
* ¿Cuantas syscalls cree que termino haciendo este programa?
* Investigar brevemente la funcionalidad del comando "strace": `man strace`
* Ejecutar nuevamente el programa a traves del comando strace:
    ```bash
    strace ./a.out
    ```
    > Lo que vera ahora es el resultado correspondiente a todas las syscalls realizadas por dicho programa.
* Encontrar la syscall que se corresponde con la llamada a la funcion `printf()`
* Investigar brevemente que hace esa syscall que encontro, usando: `man nombre_syscall_encontrada`
* Modificar el programa para que llame directamente a la syscall en lugar de `printf()`.
* Compilar nuevamente y ejecutar el programa para chequear si se obtiene el mismo resultado (el comportamiento deberia ser el mismo)
* Ejecutarlo con strace y analizar la salida (deberia ser igual a la original)

#### Preguntas
* ¿Encuentra algo inesperado en relacion al codigo del programa y la cantidad de syscalls efectivamente ocurridas?
* ¿Que ventajas/desventajas tendria usar `printf()` versus usar directamente la syscall de Linux?
* Seguramente encuentre al menos un llamado a la syscall `brk()`. Investigue su utilidad.
* Analize la ultima llamada al sistema realizada por el programa (probablemente sea `exit_group()`).
