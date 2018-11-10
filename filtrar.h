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

/*
 * filtar.h
 */
#ifndef _FILTAR_H_
#define _FILTAR_H_

/*
 * Este es el prototipo de la funcion de filtrado que debe
 * estar implementada en cada biblioteca de Filtrado.
 */
/*
 * Debe disenyarse para dejar pasar solamente los caracteres indicados.
 * Procesa <tam> caracteres de <buff_in> copiando
 * en <buff_out> aquellos que pasan el filtro.
 * Debe devolver el numero de caracteres que han pasado el filtro.
 */

typedef int tratar_t(char*buff_in, char*buff_out, int tam);

extern tratar_t  tratar;
extern tratar_t* tratar_ptr;

#endif/*_FILTAR_H_*/
