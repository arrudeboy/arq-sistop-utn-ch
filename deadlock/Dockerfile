FROM mysql:latest

COPY my.cnf /etc/mysql/conf.d/my.cnf

#COPY init.sql /docker-entrypoint-initdb.d/init.sql
COPY ./init.sql /tmp

EXPOSE 3306

CMD [ "mysqld", "--init-file=/tmp/init.sql" ]