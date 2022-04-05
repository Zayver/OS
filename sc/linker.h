#pragma once
#include "../suscriptor/struct.h"
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "struct.h"
#include "../util/list.h"
#include "../publicator/Datos.h"


void publicatorThread(void * rdata){

    //la primera entrega no es concurrente el acceso al archivo por lo que es posible hacer esto asi:
    publicator_thread_t *data = rdata;
    printf("Lista otro: %p \n", data->list);

    int fd = open(data->inputPipe, O_RDONLY);
    if(fd<0){
        perror("Error reading publicator pipe");
        exit(1);
    }
    datos_p received;
    generic_t temporal;

    while(true){
        size_t bytes = read(fd, &received, sizeof(datos_p));
        if(bytes!=0){
            //si bytes =0 no hay nada en el pipe y es esperar
            if(!checkPIDP(*data->list, received.pid)){
                temporal.pid=received.pid;
                insertList(data->list, temporal);
            }
        }     
    }
    
    //close(fd);
}
void suscriptorThread(void * rdata){
    suscriptor_thread_t *data = rdata; 
    int fd =open(data->inputPipe, O_RDONLY);
    if(fd<0){
        perror("Error reading suscriptor pipe ");
        exit(errno);
    }
    char buffer[255];
    //message;
    while(1){
        read(fd,buffer, sizeof(message_t));
        
        //insertList(&data->list, *message);
    }
    
    
}
