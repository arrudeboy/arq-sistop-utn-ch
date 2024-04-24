CREATE DATABASE IF NOT EXISTS mydb;

USE mydb;

CREATE TABLE IF NOT EXISTS usuarios (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(50),
    saldo DECIMAL(10, 2)
) ENGINE = InnoDB;

INSERT INTO usuarios (nombre, saldo) VALUES('Usuario 1', 1000.00);
INSERT INTO usuarios (nombre, saldo) VALUES('Usuario 2', 2000.00);
