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

void endProcess(int value){
    run= false;
}

void endPublicators(generic_t pid){
    if(kill(pid.pid, SIGTERM)<0){
        perror("Error sending signal ");
        //Debería exit ? 
    }
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
    suscriptor_data.list= suscriptors;
    publicator_thread_t publicator_data; publicator_data.inputPipe= flags.publicator_pipe;
    publicator_data.list= &publicators;

    if(pthread_create(&suscriptor_thread, NULL, (void*)suscriptorThread, (void*) &suscriptor_data)<0) {
        perror("Suscriptor thread ");
        exit(errno);
    }
    if(pthread_create(&publicator_thread, NULL, (void*)publicatorThread, (void*) &publicator_data)<0) {
 
        perror("Publicator thread ");
        exit(errno);
    }

    //main loop
    while(run){
        pause();
    }

    //Send signal to all suscripted processes to the server
    foreachList(publicators,endPublicators);

    //free recources and delete pipes
    unlink(flags.publicator_pipe);
    unlink(flags.suscriptor_pipe);
    deleteList(&publicators);
    deleteList(&suscriptors);
    return 0;
}
