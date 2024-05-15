# Tarea 2 (classroom)

#### Consignas

1. Desarrollar los ejercicios 9 y 10 del Trabajo Practico: _Planificacion de Procesos_
2. _Procesos e Hilos_:
    - Escribir un programa en C que cree dos Procesos hijos. Luego, el proceso Padre debe crear dos Hilos. Tanto Procesos como Hilos modifican una variable global llamada "counter". Pero solo los Procesos incrementan esta variable en 1, mientras que los Hilos la decrementan en 1. 
    - Mostrar como queda el arbol de procesos e hilos que genera el programa mencionado anteriormente. Con cual comando puede ver en Linux el arbol de procesos incluidos los hilos? 
    - Cuantas variables "counter" genera el programa y que valores tienen estas al finalizar la ejecucion? Justifique.
3. Desarrollar los ejercicios 4.4, 4.5 y 4.6 del Trabajo Practico: _Linux (parte 2) - Usuarios, Pipelines, E/S, Utils_

#### Posibles soluciones

1. [Planificacion_procesos.xlsx](1_Planificacion_procesos.xlsx) ([ver en Google Sheet](https://docs.google.com/spreadsheets/d/1Z0qTc_vEgnJOioHjlGQl4Uw79CQwt-60UgWyu3gEHMA/edit?usp=sharing))
2. [hilos.c](hilos.c)
3. Linux Bash commands:
    - ejercicio 4.4:
        ```bash
        cat /var/log/kern.log | grep -E "error|Error|ERROR"
        ```
    - ejercicio 4.5:
        ```bash
        date '+%Y-%m-%d %H:%M:%S'>today_dates.txt && sleep 5 && date '+%Y-%m-%d %H:%M:%S'>>today_dates.txt
        ```
    - ejercicio 4.6:
        ```bash
        timedatectl list-timezones | grep Argentina
        timedatectl set-timezone America/Argentina/Buenos_Aires
        ```
