/**
 * @file publicator.c
 * @author Mario Ortiz
 * @brief Publicador principal
 * 
 */
#include "Datos.h"
#include "Flags.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @brief Enviar Pipe, abre el pipe y envia los datos
 * 
 * @param midat Datos a enviar a pipe
 * @param cuantos Cantidad a enviar
 * @param pipe Nombre del pipe
 * @return int file descriptor del pipe
 */
int EnviarPipe(datos_p *midat, int cuantos, char pipe[]) {

    int i, fd;
    for(int tries= 0;;tries++) {
        if(tries==5){
            printf("5 failed attemps to connect pipe, exiting...\n");
            exit(1);
        }
        fd = open(pipe, O_WRONLY);
        if(fd<0){
            perror("Pipe");
            sleep(3);
        }
        else {
            break;
        }
        
    };

    // write(fd,&pidEnviar,sizeof(ElPid));
    for (i = 0; i < cuantos; i++) {
        printf("%c -- %s",midat[i].tipo, midat[i].notifica);
        write(fd, &midat[i], sizeof(midat[i]));
    }
    return fd;
    
}

/**
 * @brief asignar inserta una nueva noticia en la lista en la posicion e inicializa el tipo de noticia, el contenido y el tamaño del mismo
 * 
 * @param dat data
 * @param prueba tipo de noticia
 * @param notifica noticia a enviar
 * @param pos posicion actual del arreglo
 */
void asignar(datos_p *dat, char prueba, char *notifica, int pos) {
    dat[pos].len = (strlen(notifica));
    dat[pos].tipo = prueba;
    strcpy(dat[pos].notifica, notifica);
	dat->pid= getpid();
}

/**
 * @brief DeterminarFlags Valida que tipo de argumento se tiene segun los flags correspondientes en este caso se ignora el nombre
 * 
 * @param len longitud del arreglo
 * @param args arreglo de flags
 * @return Flags flags en su respectiva estrructura
 */
Flags determinarFlags(int len, char *args[]) {
    Flags f;
    char *p;
    //int value;
    for (int i = 1; i < len; i += 2) {
        p = args[i];
        switch (p[1]) {
        case 'f':
            f.NombreArchivo = args[i + 1];
            break;
        case 'p':
            f.pipe = args[i + 1];
            break;
        case 't':
            f.timeN = atoi(args[i + 1]);
            break;
        default:
            printf("Flag desconocido: %c\n", p[1]);
            exit(-1);
            break;
        }
    }
    return f;
}

/**
 * @brief ayuda cuando no se ingresan la cantidad de argumentos que se eseran explica el tipo de flags que se pueden utlizar y sus funciones
 * 
 */
void ayuda() {
    printf("El numero de argumentos ingresado es incorrecto\n");
    printf("-p pipe nominal por donde los publicadores envían información\n");
    printf("-f nombre de un archivo donde están diferentes noticias\n");
    printf("-t tiempo en segundos entre el envío de una noticia y el envío de "
           "la siguiente\n");
}

int main(int argc, char *argv[]) {
    // Se espera que se ingerese 7 arguemntos, en caso contrario se muestra la
    // funcion ayuda
    if (argc != 7) {
        ayuda();
        exit(-1);
    }
    // Se guardan los argumentos enviados en en una variable tipo Flags
    Flags flags = determinarFlags(argc, argv);
    // Se guarda el PID

    // Se definen las variables que guardaran la informacion de los publicadores
    datos_p midat[MAXDAT];
    char not [MAXINF];
    char tipo;
    FILE *fp;
    int i;

    fp = fopen(flags.NombreArchivo, "r");
    if(fp == NULL){
        perror("Error opening file ");
        exit(errno);
    }
    i = 0;
    int fd;
    while (!feof(fp)) {

        // Se lee el tipo de noticia
        fscanf(fp, "%c\n", &tipo);
        // Se lee la noticia
        fgets(not, 80, fp);

        if (tipo != '#') {
            // Entre cada noticia se espera el tiempo solicitado
            sleep(flags.timeN);
            asignar(midat, tipo, not, i++);
            fd=EnviarPipe(midat, i, flags.pipe);
            i = 0;
        }
    }
    fclose(fp);
    //mensaje final
    datos_p final; final.len=0; final.pid=getpid(); final.tipo='-';
    write(fd,&final, sizeof(datos_p));
    close(fd);
}
