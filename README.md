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
