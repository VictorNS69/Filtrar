#
# filtrar: filtra por contenido los archivos del directorio indicado.
#
# Copyright (c) 2013,2018 Francisco Rosales <frosal@fi.upm.es>
# Todos los derechos reservados.
#
# Publicado bajo Licencia de Proyecto Educativo Práctico
# <http://laurel.datsi.fi.upm.es/~ssoo/LICENCIA/LPEP>
#
# Queda prohibida la difusión total o parcial por cualquier
# medio del material entregado al alumno para la realización 
# de este proyecto o de cualquier material derivado de este, 
# incluyendo la solución particular que desarrolle el alumno.
#

CC=gcc
CFLAGS=-Wall -g

ALL = filtrar libfiltra_alfa.so libfiltra_delay.so libfiltra_void.so
TGZ = filtrar.2018b.entrega.tar.gz
ENT = filtrar.c filtrar.h libfiltra_alfa.c

all : $(ALL)

filtrar : LDFLAGS += -ldl

filtrar : filtrar.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)
 
%.so : %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -fPIC -shared $< -o $@

tar : $(TGZ)
$(TGZ) : $(ENT)
	tar -zcf $@ $(ENT)

$(ENT) : 
	tar -m -zxf $(TGZ) $@
 
clean :
	-rm -f $(ALL)

$(ALL) : filtrar.h

entrega : autores.txt bitacora.txt $(TGZ)
	entrega.so6 filtrar.2018b

cleanall : clean
	-rm -rf _*
	-rm $(TGZ)
	-make -C libmal clean
