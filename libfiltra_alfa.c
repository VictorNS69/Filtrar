/*
* Autor: Víctor Nieves Sánchez
* filtrar: filtra por contenido los archivos del directorio indicado.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "filtrar.h"

/* Este filtro deja pasar los caracteres NO alfabeticos. */
/* Devuelve el numero de caracteres que han pasado el filtro. */
int tratar(char* buff_in, char* buff_out, int tam){
	int i;
	int o = 0;
	for (i = 0; i < tam; i++) {
		if (!isalpha(buff_in[i])) {
			buff_out[o] = buff_in[i];
			o++;
		}
	}
	return o;
}
