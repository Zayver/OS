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
#include "news.h"
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
            pthread_mutex_lock(&publicator_mutex); // lock a un mutex para modificar la lista
                                                   // de noticias de forma atomica
            insertList(&data->news[toNewsIndex(temp.news.tipo)], temp);
            new_new = received.tipo;
            pthread_mutex_unlock(&publicator_mutex);
            pthread_cond_broadcast(&sub_suscriptor_cond); // broadcast a los subhilos para que
                                                          // envien la noticia
            if (received.len == 0) {
                // si len es igual a 0 no hay noticia y significa que el
                // publicador finaliza eliminarlo de la lista de publicadores
                pthread_mutex_lock(&publicator_mutex);
                removeList(data->list, received.pid);
                pthread_mutex_unlock(&publicator_mutex);
                if (data->list->size == 0) {
                    sleep(data->wait_time); // esperar el tiempo a ver si read
                                            // produce 0 bytes si si es que ningún
                    // publicador seguirá enviando entonces finalizar thread
                }
            } else if (checkPIDP(*data->list, received.pid)) {
                temporal.pid = received.pid;
                pthread_mutex_lock(&publicator_mutex);
                insertList(data->list, temporal);
                pthread_mutex_unlock(&publicator_mutex);
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
    sub_suscriptor_thread_t *data = rdata;
    char buff[10];
    sprintf(buff, "%d", data->pid);
    if (mkfifo(buff, 0666)) {
        perror("Error creating dedicated pipe");
        exit(errno);
    }
    // primera rutina del thread debería ser en base al enunciado, enviar todas
    // las noticias de los temas
    int fd = open(buff, O_WRONLY);
    if (fd < 0) {
        perror("Opening pipe");
        exit(1);
    }
    printf("HERE\n");
    for (char *p = data->news_preference; *p != '\0'; p++) {
        printf("FOR\n");
        node_t *t = data->news[toNewsIndex(*p)].first;
        if(t == NULL)
            continue;
        while(t!=NULL){
            write(fd, &t->data.news, sizeof(Noticia));
            t=t->next;
        }
    }
    close(fd);
    printf("HERE22222\n");

    while (true) {
        pthread_cond_wait(&sub_suscriptor_cond, &sub_suscriptor_mutex); // esperar a que haya una noticia

        if (strchr(data->news_preference,new_new)) { // si la nueva noticia recibida es de interes enviarla
            fd = open(buff, O_WRONLY);
            if (fd < 0) {
                perror("Opening pipe");
                exit(1);
            }
            write(fd, &data->news[toNewsIndex(new_new)].last->data.news, sizeof(Noticia));
            close(fd);
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

    pthread_t sender; // serían varios 1 por suscriptor;
    while (true) {
        Suscriptor received;
        generic_t temporal;
        sub_suscriptor_thread_t sub_data;
        size_t bytes = read(fd, &received, sizeof(Suscriptor));

        if (bytes != 0) {
            // asumiendo que los suscriptores nunca se desconectan
            sub_data.news = data->news;
            sub_data.pid = received.pid;
            strcpy(sub_data.news_preference, received.categorias);
            temporal.pid = received.pid;
            insertList(data->list, temporal);

            if (pthread_create(&sender, NULL, (void *)subSuscriptorThread, &sub_data) < 0) {
                perror("Error creating thread for suscribers 2 ");
                exit(errno);
            }
        }
    }
}
