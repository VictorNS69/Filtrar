/*
* filtrar: filtra por contenido los archivos del directorio indicado.
*
* Copyright (c) 2013,2017 Francisco Rosales <frosal@fi.upm.es>
* Todos los derechos reservados.
*
* Publicado bajo Licencia de Proyecto Educativo Práctico
* <http://laurel.datsi.fi.upm.es/~ssoo/LICENCIA/LPEP>
*
* Queda prohibida la difusión total o parcial por cualquier
* medio del material entregado al alumno para la realización
* de este proyecto o de cualquier material derivado de este,
* incluyendo la solución particular que desarrolle el alumno.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "filtrar.h"

/* Este filtro deja pasar los caracteres NO alfabeticos. */
/* Devuelve el numero de caracteres que han pasado el filtro. */
int tratar(char* buff_in, char* buff_out, int tam){
	int i;
	int num_pas = 0;
	for (i = 0; i < tam; i++) {
		if (!isalpha(buff_in[i])) {
			buff_out[num_pas] = buff_in[i];
			num_pas++;
		}
	}
	return num_pas;
}
