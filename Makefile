#
# filtrar: filtra por contenido los archivos del directorio indicado.
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
