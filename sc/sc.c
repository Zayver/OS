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


static bool volatile run = true; // * Para liberar recursos con signal

void init(Flags f){
    int status;
    status = mkfifo(f.publicator_pipe, O_RDONLY);
    if(status<0){
        perror("Publicator pipe");
        exit(errno);
    }
    status = mkfifo(f.suscriptor_pipe, O_RDONLY);
    if(status<0){
        perror("Suscriptor pipe");
        exit(errno);
    }
}

void endProcess(int value){
    run= false;
}

int main(int argc, char* argv[]){
    if(argc!= 7){
        printf("Error \n");
        exit(22);
        //TODO implement help
    }
    //flags listas
    Flags flags = determineFlags(argc, argv);
    list_t publicators = initList(), suscriptors= initList();
    init(flags);

    //signal handlers
    signal(SIGINT, endProcess);

    //threads
    pthread_t suscriptor_thread, publicator_thread;


    if(pthread_create(&suscriptor_thread, NULL, (void*)suscriptorThread, (void*) &suscriptors)<0) {
        perror("Suscriptor thread ");
        exit(errno);
    }
    if(pthread_create(&publicator_thread, NULL, (void*)publicatorThread, (void*) &publicators)<0) {
        perror("Publicator thread ");
        exit(errno);
    }

    //main loop
    while(run){
        //DO STUFF
        sleep(4);
        printf("Actualizacion\n");
    }

    //TODO send signal to all suscripted processes to the server
   

    //free recources and delete pipes
    deleteList(&publicators);
    deleteList(&suscriptors);
    unlink(flags.publicator_pipe);
    unlink(flags.suscriptor_pipe);

    return 0;
}
