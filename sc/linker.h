#pragma once
#include "../suscriptor/struct.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "struct.h"
#include "../util/list.h"

void publicatorThread(void * data){

    while(1){

    }

}
void suscriptorThread(void * rdata){
    suscriptor_thread_t *data = rdata; 
    int fd =open(data->inputPipe, O_RDONLY);
    if(fd<0){
        perror("Error reading suscriptor pipe ");
        exit(errno);
    }
    char buffer[255];
    generic_t *message;
    while(1){
        read(fd,buffer, sizeof(message_t));
        message = buffer;
        insertList(&data->list, *message);
    }
    
}