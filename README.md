# Practica Filtrar Sistemas Operativos curso 18/19

Filtrar: filtra por contenido los archivos del directorio indicado.

## Autor

[Víctor Nieves Sánchez](https://twitter.com/VictorNS69)

## Compilación

$ make

```
gcc -Wall  -ldl  filtrar.c filtrar.h   -o filtrar
gcc -Wall -fPIC -shared libfiltra_alfa.c  -o libfiltra_alfa.so
gcc -Wall -fPIC -shared libfiltra_delay.c -o libfiltra_delay.so
gcc -Wall -fPIC -shared libfiltra_void.c  -o libfiltra_void.so
```

## Funcionamiento
```
filtrar directorio [filtro...]
```
_filtrar_ tiene como objetivo aplicar los filtros indicados a la concatenación de los archivos contenidos en el directorio especificado. Es decir, su comportamiento sería similar a:
```
cat directorio/* | filtro1 | filtro2 | ... | filtroN
```
Pero con las siguientes diferencias:
- Los filtros (indicados como argumentos opcionales) podrán ser de dos tipos:
  - Biblioteca dinámica, si su nombre termina en ”.so”.
  - Mandato estándar sin argumentos, para cualquier otro nombre.
- Sólo se procesarán las entradas de directorio que no empiecen por punto y no sean a su vez directorios.
- Se implementará un control de tiempo máximo de ejecución.
