/**
 * @file linker.h
 * @author Santiago Z
 * @brief Cabecera que define las rutinas de los hilos del sc
 */
#pragma once
#include "../publicator/Datos.h"
#include "../suscriptor/noticia.h"
#include "../suscriptor/suscriptor.h"
#include "../util/list.h"
#include "struct.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static volatile bool checkNew = false;
/**
 * @brief Thread principal de los publicadores
 * @param rdata void* de los datos que recibe el thread
 */
void publicatorThread(void *rdata) {

    // la primera entrega no es concurrente el acceso al archivo por lo que es
    // posible hacer esto asi:
    publicator_thread_t *data = rdata;
    int fd = open(data->inputPipe, O_RDONLY);
    if (fd < 0) {
        perror("Error reading publicator pipe");
        exit(1);
    }
    datos_p received;
    generic_t temporal, temp;

    while (true) {
        size_t bytes = read(fd, &received, sizeof(datos_p));
        if (bytes != 0) {
            // si bytes =0 no hay nada en el pipe y es esperar
            strcpy(temp.news.info, received.notifica);
            temp.news.tipo = received.tipo;
            temp.news.len = received.len;
            insertList(data->news, temp);
            checkNew= true;
            if (received.len == 0) {
                // si len es igual a 0 no hay noticia y significa que el
                // publicador finaliza eliminarlo de la lista de publicadores
                removeList(data->list, received.pid);
                if (data->list->size == 0) {
                    sleep(
                        data->wait_time); // esperar el tiempo a ver si read
                                          // produce 0 bytes si si es que ningún
                    // publicador seguirá enviando entonces finalizar thread
                }
            } else if (checkPIDP(*data->list, received.pid)) {
                temporal.pid = received.pid;
                insertList(data->list, temporal);
            }
        } else {
            break;
        }
    }
    pthread_exit(NULL);
}

/**
 * @brief Subthread de los suscribtores, 1 por suscriptor
 * @param rdata Data generica pasada al thread
 */
void subSuscriptorThread(void *rdata) {
    suscriptor_thread_t *data = rdata; char buff[10];
    sprintf(buff,"%d", data->list->last->data.suscriptor_t.pid);
    if(mkfifo(buff,0666)){
        perror("Error creating dedicated pipe");
        exit(errno);
    }
    while (true) {
        // si ckecknew true entonces hay noticias nuevas que se podrían enviar
        // si son suscritas
        if(checkNew){
            if (strchr(data->list->last->data.suscriptor_t.categorias, data->news->last->data.news.tipo) !=
                NULL) {
                // si le intereza, enviarselo

                int fd = open(buff, O_WRONLY);
                if (fd < 0) {
                    perror("Opening pipe");
                    exit(1);
                }
                if(data->news->last->data.news.len!=0){
                    size_t bytes = write(fd, &data->news->last->data.news, sizeof(Noticia));
                }
                
                checkNew = false; // requeriría atomicidad y es por eso que es
                                  // solución temporal
            }
        }
    }
}
/**
 * @brief Thread general de los suscriptores que escucha el pipe por peticiones
 * @param rdata data generica al thread
 */
void suscriptorThread(void *rdata) {
    suscriptor_thread_t *data = rdata;
    int fd = open(data->inputPipe, O_RDONLY);
    if (fd < 0) {
        perror("Error reading suscriptor pipe ");
        exit(errno);
    }
    Suscriptor received;
    generic_t temporal;
    char pid_array[10];
    pthread_t sender; // serían varios 1 por suscriptor;
    while (true) {
        size_t bytes = read(fd, &received, sizeof(Suscriptor));

        if (bytes != 0) {
            // asumiendo que los suscriptores nunca se desconectan
            strcpy(temporal.suscriptor_t.categorias, received.categorias);
            temporal.suscriptor_t.pid = received.pid;
            insertList(data->list, temporal);
            sprintf(pid_array, "%d", received.pid);

            // para la segunda entrega con multiples suscriptores un thread por
            // suscriptor por ahora primera entrega así
            if (pthread_create(&sender, NULL, (void *)subSuscriptorThread, rdata) <
                0) {
                perror("Error creating thread for suscribers 2 ");
                exit(errno);
            }
        }

        // insertList(&data->list, *message);
    }
}
