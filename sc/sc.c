/**
 * @file sc.c
 * @author Santiago Z 
 * @brief Sistema de comunicación central 
 */
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include "../util/flag.h"
#include "../util/list.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include "linker.h"
#include <signal.h>
#include <sys/wait.h>

/**
 * @brief  Inicializa todo en base a los flags otorgados como argument
 * @param f  Flags pasados por linea de comandos
 */
void init(Flags f){
    int status;
    status = mkfifo(f.publicator_pipe, 0666);
    if(status<0){
        perror("Publicator pipe");
        exit(errno);
    }
    status = mkfifo(f.suscriptor_pipe, 0666);
    if(status<0){
        perror("Suscriptor pipe");
        exit(errno);
    }
}

/**
 * @brief Signal handher de SIGTERM
 * @param value Valor de la signal
 */
void endProcess(int value){
    run= false;
    exit(1);
}

/**
 * @brief Si en algún punto el sc muere y los publicadores están conectados enviarles SIGTERM
 * @param pid pid por proceso
 */
void endPublicators(generic_t pid){
    if(kill(pid.pid, SIGTERM)<0){
        perror("Error sending signal ");
        if(errno==3){
            //si errno vale 3 entonces no existe el proceso por lo que es probable que ya haya terminado
            //mediante otro mecanismo, lo mas probable es un sigterm de la terminal 
            printf("No existe el proceso, es probable que ya se haya terminado el proceso con pid: %d\n", pid.pid);
        }
    }
}
/**
 * @brief Finalizar todos los suscriptores
 * @param sus Union generica de lista especializada para suscriptor
 */
void endSuscriptors(generic_t sus){
    if(kill(sus.suscriptor_t.pid, SIGTERM)<0){
        perror("Error sending signal ");
        if(errno==3){
            //si errno vale 3 entonces no existe el proceso por lo que es probable que ya haya terminado
            //mediante otro mecanismo, lo mas probable es un sigterm de la terminal 
            printf("No existe el proceso, es probable que ya se haya terminado el proceso con pid: %d\n", sus.suscriptor_t.pid);
        }
    }
    char buff[10];
    sprintf(buff,"%d", sus.suscriptor_t.pid);
    unlink(buff);
}

int main(int argc, char* argv[]){
    if(argc!= 7){
        help();
        exit(22);
    }
    //flags listas
    Flags flags = determineFlags(argc, argv);
    list_t publicators = initList(), suscriptors= initList(), news = initList();
    init(flags);

    //signal handlers
    signal(SIGINT, endProcess);

    //threads
    pthread_t suscriptor_thread, publicator_thread;
    suscriptor_thread_t suscriptor_data; suscriptor_data.inputPipe= flags.suscriptor_pipe;
    suscriptor_data.list= &suscriptors; suscriptor_data.news= &news;
    publicator_thread_t publicator_data; publicator_data.inputPipe= flags.publicator_pipe;
    publicator_data.list= &publicators; publicator_data.wait_time= flags.time;
    publicator_data.news=&news;

    if(pthread_create(&suscriptor_thread, NULL, (void*)suscriptorThread, (void*) &suscriptor_data)<0) {
        perror("Suscriptor thread ");
        exit(errno);
    }
    if(pthread_create(&publicator_thread, NULL, (void*)publicatorThread, (void*) &publicator_data)<0) {
 
        perror("Publicator thread ");
        exit(errno);
    }

    // todo está dictado por el thread de los publicadores 
    pthread_join(publicator_thread,NULL);


    //Enviar SIGTERM a todos los procesos suscritos al servidor
    foreachList(publicators,endPublicators);
    foreachList(suscriptors,endSuscriptors);

    //free recources and delete pipes
    unlink(flags.publicator_pipe);
    unlink(flags.suscriptor_pipe);
    deleteList(&publicators);
    deleteList(&suscriptors);
    //deleteList(&news);


    printf("Trabajo realizado :D \n");
    return 0;
}
