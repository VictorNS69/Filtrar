/* Autor: Victor Nieves Sanchez
*
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

//Ingcludes del enunciado
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>

#include "filtrar.h"

//Includes propios
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <ctype.h>

//errores y avisos
const char* MSG_USO = "Uso: %s directorio [filtro...]\n";
const char* ERR_ABRIR_DIRECTORIO = "Error al abrir el directorio '%s'\n";
const char* AVI_STAT_FICHERO = "AVISO: No se puede stat el fichero '%s'!\n";
const char* AVI_ABRIR_FICHERO = "AVISO: No se puede abrir el fichero '%s'!\n";
const char* AVI_EMITIR_FICHERO = "AVISO: No se puede emitir el fichero '%s'!\n";
const char* ERR_LEER_DIRECTORIO = "Error al leer el directorio '%s'\n";
const char* ERR_CREAR_PIPE = "Error al crear el pipe\n";
const char* ERR_CREAR_PROCESO = "Error al crear proceso %d\n";
const char* ERR_EJECUTAR_MANDATO = "Error al ejecutar el mandato '%s'\n";
const char* ERR_EJECUTAR_FILTRO = "Error al ejecutar el filtro '%s'\n";
const char* ERR_ESPERAR_PROCESO = "Error al esperar proceso %d\n";
const char* FIN_PROCESO_CODIGO = "%s: %d\n";
const char* FIN_PROCESO_SENYAL = "%s: senyal %d\n";
const char* ERR_ABRIR_BIBLIOTECA = "Error al abrir la biblioteca '%s'\n";
const char* ERR_BUSCAR_SIMBOLO = "Error al buscar el simbolo '%s' en '%s'\n";
const char* ERR_MATAR_PROCESO = "Error al intentar matar proceso %d\n";
const char* AVI_ALARMA = "AVISO: La alarma ha saltado!\n";
const char* ERR_FILTRAR_TIMEOUT = "Error FILTRAR_TIMEOUT no es entero positivo: '%s'\n";
const char* AVI_ALARMA_VENCERA = "AVISO: La alarma vencera tras %d segundos!\n";

/* ---------------- PROTOTIPOS ----------------- */

/* Esta funcion monta el filtro indicado y busca el simbolo "tratar"
   que debe contener, y aplica dicha funcion "tratar()" para filtrar
   toda la informacion que le llega por su entrada estandar antes
   de enviarla hacia su salida estandar. */
extern void filtrar_con_filtro(char* nombre_filtro);

/* Esta funcion lanza todos los procesos necesarios para ejecutar los filtros.
   Dichos procesos tendran que tener redirigida su entrada y su salida. */
void preparar_filtros(void);

/* Esta funcion recorrera el directorio pasado como argumento y por cada entrada
   que no sea un directorio o cuyo nombre comience por un punto '.' la lee y
   la escribe por la salida estandar (que seria redirigida al primero de los
   filtros, si existe). */
void recorrer_directorio(char* nombre_dir);

/* Esta funcion recorre los procesos arrancados para ejecutar los filtros,
   esperando a su terminacion y recogiendo su estado de terminacion. */
void esperar_terminacion(void);

/* Desarrolle una funcion que permita controlar la temporizacion de la ejecucion
   de los filtros. */
extern void preparar_alarma(void);

/* ---------------- IMPLEMENTACIONES ----------------- */
char** filtros;   /* Lista de nombres de los filtros a aplicar */
int    n_filtros; /* Tama~no de dicha lista */
pid_t* pids;      /* Lista de los PIDs de los procesos que ejecutan los filtros */


/* ---------------- FUNCIONES ----------------- */
void preparar_filtros(void){
	char *fichero;
	int pip[2];
	int i;
	for (i = 0; i < n_filtros;i++){
		/* Tuberia hacia el hijo (que es el proceso que filtra). */
		if (pipe(pip) <0){
			fprintf(stderr, "%s", ERR_CREAR_PIPE);
			exit(1);
		}
		/* Lanzar nuevo proceso */
		int p = fork();
		switch(p){
			case -1:
				/* Error. Mostrar y terminar. */
				fprintf(stderr, ERR_CREAR_PROCESO, p);
        exit(1);
			case  0:
				/* Hijo: Redireccion y Ejecuta el filtro. */
				dup2(pip[0], 0);
				close (pip[0]);
				close (pip[1]);
				//	if ()	/* El nombre termina en ".so" ? */
				fichero = strrchr(filtros[i], '.');
				if (fichero != NULL && strcmp(fichero, ".so") == 0){	/* SI. Montar biblioteca y utilizar filtro. */
					//		filtrar_con_filtro(filtros[p]);
					filtrar_con_filtro(filtros[p]);
					exit(0);
				}
				//	else
				else{	/* NO. Ejecutar como mandato estandar. */
					execlp(filtros[i], filtros[i], NULL, NULL);
					fprintf(stderr, ERR_EJECUTAR_MANDATO, filtros[i]);
					exit(1);
				}
			//
			default:
				/* Padre: Redireccion */
				close (pip[0]);
				dup2(pip[1], 1);
				close (pip[1]);
				pids[i] = p;
				break;
		}
	}
}

void recorrer_directorio(char* nombre_dir){
	DIR* dir = NULL;
	struct dirent* ent;
	char fich[1024];
	char buff[4096];
	int fd;
	struct stat status;
	struct sigaction act;
	/* Abrir el directorio */
	dir = opendir(nombre_dir);
	/* Tratamiento del error. */
	if (dir == NULL){
		fprintf(stderr, ERR_ABRIR_DIRECTORIO, nombre_dir );
		exit(1);
	}
	/* Recorremos las entradas del directorio */
	while((ent=readdir(dir))!=NULL){
		/* Nos saltamos las que comienzan por un punto "." */
		if(ent->d_name[0]=='.')
			continue;
		/* fich debe contener la ruta completa al fichero */
		strcpy(fich, nombre_dir);
		strcat(fich, "/");
		strcat(fich, ent->d_name);
		if (stat(fich, &status) < 0) {
			fprintf(stderr, AVI_STAT_FICHERO, fich);
			exit(0);
		}
		/* Nos saltamos las rutas que sean directorios. */
		if (S_ISDIR(status.st_mode))
			continue;
		/* Abrir el archivo. */
		fd = open(fich, O_RDONLY);
		/* Tratamiento del error. */
		if (fd == -1) {
			fprintf(stderr, AVI_ABRIR_FICHERO, fich);
			exit(0);
		}
		/* Cuidado con escribir en un pipe sin lectores! */
		if (errno == EPIPE) {
			fprintf(stderr, AVI_EMITIR_FICHERO, fich);
			close(fd);
			exit(1);
		}
		act.sa_flags = 0;
		act.sa_handler = SIG_IGN;
		sigaction(SIGPIPE, &act, NULL);
		/* Emitimos el contenido del archivo por la salida estandar. */
		while(write(1,buff,read(fd,buff,4096)) > 0)
			continue;
		if (errno) {
			fprintf(stderr, ERR_LEER_DIRECTORIO, nombre_dir );
			exit(1);
		}
		/* Cerrar. */
		close(fd);
		errno = 0; //reseteamos errno para futuras llamadas
	}
	/* IMPORTANTE:
	* Para que los lectores del pipe puedan terminar
	* no deben quedar escritores al otro extremo. */
	// IMPORTANTE
	closedir(dir);
}


/* Funcion que se ejecutará cuando salte la alarma
*/
void alarma(){
	fprintf(stderr, "%s", AVI_ALARMA);
	int i;
	for (i = 0; i < n_filtros; i++) {
    if (kill(pids[i], 0) == 0) {
      if ((kill(pids[i], SIGKILL)) < 0) {
        fprintf(stderr, ERR_MATAR_PROCESO, pids[i]);
        exit(1);
      }
    }
  }
}

void preparar_alarma(void){
	struct sigaction act;
	int timeout;
	char *timeout_env = getenv("FILTRAR_TIMEOUT");
	if (timeout_env == NULL)
		return; //si es NULL salimos ya
	// Que sea positivo
	int i;
	for (i = 0; i < strlen(timeout_env); i++){
		if (isdigit(timeout_env[i])){
			fprintf(stderr, ERR_FILTRAR_TIMEOUT, timeout_env);
			exit(1);
		}
	}
	timeout = atoi(timeout_env);
  fprintf(stderr, AVI_ALARMA_VENCERA, timeout);
	act.sa_flags = SA_RESTART;
  act.sa_handler = &alarma;
  sigaction(SIGALRM, &act, NULL);
  alarm(timeout);
}

void imprimir_estado(char* filtro, int status){
	/* Imprimimos el nombre del filtro y su estado de terminacion */
	if(WIFEXITED(status))
		fprintf(stderr,"%s: %d\n",filtro,WEXITSTATUS(status));
	else
		fprintf(stderr,"%s: senal %d\n",filtro,WTERMSIG(status));
}

void esperar_terminacion(void){
	int p, status;
	close(1);
	for(p=0;p<n_filtros;p++){
		/* Espera al proceso pids[p] */
		if (waitpid(pids[p], &status, 0) < 0) {
      fprintf(stderr, ERR_ESPERAR_PROCESO, pids[p]);
      exit(1);
    }
		/* Muestra su estado. */
		imprimir_estado(filtros[p], status);
		//exit(0);
	}
}
	void filtrar_con_filtro(char* nombre_filtro){
	  char  bin[4096], bout[4096];
	  int (*filtro) (char*, char*, int);
	  void* biblioteca;
	  biblioteca = dlopen(nombre_filtro, RTLD_LAZY);
	  if (biblioteca == NULL) {
	    fprintf(stderr, ERR_ABRIR_BIBLIOTECA, nombre_filtro);
	    exit(1);
	  }
	  filtro = dlsym(biblioteca, "tratar");
	  if (filtro == NULL) {
	    fprintf(stderr, ERR_BUSCAR_SIMBOLO, nombre_filtro);
	    exit(1);
	  }
		int num_bytes;
	  while ((num_bytes = read(0, bin, 4096)) > 0) {
	    int filtrar = filtro(bin, bout, num_bytes);
	    write(1, bout, filtrar);
	    if (filtrar < 0) {
	      fprintf(stderr, ERR_EJECUTAR_FILTRO, nombre_filtro);
	      exit(1);
	    }
	  }
  	dlclose(biblioteca);
	}

/*----------------------FUNCION PRINCIPAL------------*/
int main(int argc, char* argv[]){
	/* Chequeo de argumentos */
	if(argc<2){
		fprintf(stderr,  MSG_USO, argv[0]);
		exit(1);
	}
	/* Invocacion sin argumentos  o con un numero de argumentos insuficiente */
	//exit(0);

	filtros = &(argv[2]);                             /* Lista de filtros a aplicar */
	n_filtros = argc-2;                               /* Numero de filtros a usar */
	pids = (pid_t*)malloc(sizeof(pid_t)*n_filtros);   /* Lista de pids */

	preparar_alarma();

	preparar_filtros();

	recorrer_directorio(argv[1]);

	esperar_terminacion();

	return 0;
}
