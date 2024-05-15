# Deadlock example (MySQL database)

#### Proposito y objetivo

Este ejemplo levanta una instancia de MySQL server dentro de un contenedor Docker. Dicho servidor se inicializa automaticamente con una base de datos llamada "mydb" la cual contiene una tabla "usuarios" con dos registros. Ademas, se especifica una configuracion de startup (archivo `my.cnf`) que evita el _autocommit_ y aisla cada transaccion con un lock de escritura [[ver doc]](https://dev.mysql.com/doc/refman/5.7/en/innodb-transaction-isolation-levels.html).

De esta forma se intenta con dos procesos _clientes_ (_A_ y _B_) adquirir y modificar registros (filas) de la tabla "usuarios" al mismo tiempo en transacciones indepedientes con el objetivo de generar un deadlock y observar como actua el servidor en consecuencia.

#### Prerequisitos

Tener instalado [Docker](https://docs.docker.com/get-docker/) en tu maquina.

#### Pasos a seguir

1. Crear la imagen del servidor MySQL ejecutando el comando:
    ```bash
    docker build -t mysql-deadlock .
    ```
2. Crear un contenedor para iniciarl el servidor MySQL a partir de la imagen generada en el punto anterior:
    ```bash
    docker run -p 3306:3306 --name mysql-container -e MYSQL_ROOT_PASSWORD=root mysql-deadlock
    ```
3. En dos terminales aparte conectarse al CLI de MySQL server para iniciar dos clientes (A y B). En ambas terminales ejecute:
    ```bash
    docker exec -it mysql-container bash
    mysql -u root -p
    ```
    > Le pedira una password, ponemos _root_

4. Ya estando en la CLI de MySQL server, nos conectamos a la base de datos desde ambas terminales (clientes A y B):
    ```bash
    use mydb;
    ```

5. En el cliente A ejecutamos las siguientes instrucciones:
    ```bash
    BEGIN;
    UPDATE usuarios SET saldo = saldo - 1000.0 WHERE id = 1;
    ```
    >> _A_ obtiene y bloquea el recurso (registro 1) para poder modificarlo.

6. En el cliente B ejecutamos las siguientes instrucciones:
    ```bash
    BEGIN;
    UPDATE usuarios SET saldo = saldo + 1000.0 WHERE id = 2;
    UPDATE usuarios SET saldo = saldo - 1000.0 WHERE id = 1;
    ```
    >> _B_ obtiene y bloquea el recurso (registro 2) para poder modificarlo. Seguidamente intenta obtener otro recurso (registro 1) pero no puede hacerlo porque _A_ lo esta usando y lo bloqueo, por lo tanto _B_ debe esperar.

7. Volvemos al cliente A y ejecutamos:
    ```bash
    UPDATE usuarios SET saldo = saldo + 1000.0 WHERE id = 2;
    ```
    >> _A_ intenta obtener un nuevo recurso (registro 2) pero no puede hacerlo porque _B_ lo esta usando y lo bloqueo, por lo tanto debe esperar.

#### Resultado y Conclusion

En este momento se produce un __deadlock__ ya que _A_ obtuvo y retiene al registro 1 mientras espera obtener el registro 2. Mientras que _B_ hace lo propio con el registro 2 mientras espera obtener el registro 1.

Mysql server nos muestra el siguiente mensaje en el cliente A:

    mysql> UPDATE usuarios SET saldo = saldo + 1000.0 WHERE id = 2;
    ERROR 1213 (40001): Deadlock found when trying to get lock; try restarting transaction

Y la accion de recuperacion que toma este servidor de base de datos es abortar las transacciones en curso que estan involucradas en el deadlock, es decir las ultimas transacciones que iniciamos con `BEGIN` tanto en el cliente A como en el cliente B.
