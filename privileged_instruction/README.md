# Privileged Instructions

En este ejemplo verificamos como el Kernel del SO impide ejecutar una instruccion privilegiada a un proceso comun (que corre en modo usuario).

El programa intenta ejecutar una instruccion privilegiada. Dado que no es posible a traves del lenguaje C de alto nivel realizar una llamada a una instruccion de assembler, el programa utiliza la keyword propia del compilador gcc `__asm__` ([mas info aca](https://gcc.gnu.org/onlinedocs/gcc/Using-Assembly-Language-with-C.html#Using-Assembly-Language-with-C)) para lograr insertar en el binario compilado una llamada directa a la instruccion "CLI" (_Clear Interrupts_), la cual es privilegiada.


#### Para compilar
```bash
gcc -g execute_privileged_instruction.c
```

> Una vez compilado el programa, aparecera en el directorio actual un archivo llamado ___a.out___

#### Para ejecutar
```bash
./a.out
```

#### Analisis
* Analizar el comportamiento del programa. Describa lo que ocurrio.
* Validar que el programa compilado efectivamente tiene la instrucción deseada. Para ello, decompilar el programa con el comando: `objdump -d a.out` y luego analizar el assembler de la seccion correspondiente a la función _main()_ (si la salida es muy larga, ejecute  `objdump -d a.out | less` para navegar el resultado usando las teclas arriba/abajo)
* Una vez encontrada la instrucción, salga de la vista tipeando "q"


#### Preguntas
* ¿Que comportamiento tuvo Linux al encontrarse con un proceso que intento ejecutar una instrucción privilegiada?
* Descomente la llamada a la funcion `signal()`, linea 16, para que el proceso capture la señal y ejecute el codigo indicado en la funcion `sig_handler()`. Compile nuevamente y ejecute el programa. Analice el comportamiento del programa y describa lo que ocurrio ahora.
* Modifique el programa para que la funcion `sig_handler()` no llame a la funcion `exit(-1)`. Compile nuevamente y vuelva a ejecutar.
* Que ocurrio ahora? Analice e investigue para entender posibles usos de la funcion `signal()` para estos casos.
