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
#include <bits/pthreadtypes.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static volatile bool suscriptor_thread_run = true;
static volatile bool publicator_thread_run = true;

void suscriptor_handler(int signal) { pthread_exit(NULL); }
void publicator_handler(int signal) { publicator_thread_run = false; }
/**
 * @brief Subthread del publicador para contador asincrónico, con eso es posible que siempre haya un
 * thread escuchando a cualquier publicador
 * @param time tiempo a dormir
 * @details Un contador de time segundos que arroja una señal si no esterminado correctamente SIGUSR2 o con SIGTERM
 */
void subPublicatorThread(const void *time) {
    sleep(*(int *)time);
    raise(SIGUSR1);
}
/**
 * @brief Thread principal de los publicadores
 * @param rdata void* de los datos que recibe el thread
 */
void publicatorThread(void *rdata) {
    signal(SIGUSR1, publicator_handler); // para exit del thread cuando el timer finilice (si es que lo hace)
    publicator_thread_t *data = rdata;
    int fd = open(data->inputPipe, O_RDONLY);
    if (fd < 0) {
        perror("Error reading publicator pipe");
        exit(1);
    }
    datos_p received;
    generic_t temporal, temp;
    pthread_t subthread;
    bool initialized = false;

    while (publicator_thread_run) {
        size_t bytes = read(fd, &received, sizeof(datos_p));

        if (bytes != 0) {
            if (initialized)
                pthread_cancel(subthread); // cancelar el thread, una señal no ya que el subthread hereda los signalhandlers
            // si bytes =0 no hay nada en el pipe y es esperar
            if (received.tipo != '-') {
                strcpy(temp.news.info, received.notifica);
                temp.news.tipo = received.tipo;
                temp.news.len = received.len;
                insertList(&data->news[toNewsIndex(temp.news.tipo)], temp);
                new_new = received.tipo;
                pthread_mutex_lock(&sub_suscriptor_threads_mutex);
                for (int i = 0; i < sender_index; i++) {
                    pthread_kill(sender[i], SIGUSR1);
                }
                pthread_mutex_unlock(&sub_suscriptor_threads_mutex);
            }

            if (received.len == 0) {
                // si len es igual a 0 no hay noticia y significa que el
                // publicador finaliza eliminarlo de la lista de publicadores
                removeList(data->list, received.pid);
                if (data->list->size == 0) {
                    initialized = true;
                    if (pthread_create(&subthread, NULL, (void *)subPublicatorThread, (void *)&data->wait_time) < 0) {
                        perror("subpublicatorThread fail ");
                        exit(errno);
                    } // esperar el tiempo a ver si read produce 0 bytes si si es que ningún publicador seguirá enviando entonces finalizar thread
                }
            } else if (checkPIDP(*data->list, received.pid)) {
                temporal.pid = received.pid;
                insertList(data->list, temporal);
            }
        }
    }

    // ultimo mensaje para que todos los subthreads de publicadores finalicen y limpien recursos
    // posiblemente debería hacerlo en el hilo suscriptor maestro pero como read es bloqueante, imposible saber cuando dejar de leer
    // sin busy waiting para terminar los subsuscriptores
    pthread_mutex_lock(&sub_suscriptor_threads_mutex);
    for (int i = 0; i < sender_index; i++) {
        pthread_kill(sender[i], SIGUSR2);
    }
    for (int i = 0; i < sender_index; i++) {
        pthread_join(sender[i], NULL);
    }
    pthread_mutex_unlock(&sub_suscriptor_threads_mutex);
    pthread_exit(NULL);
}

/**
 * @brief Subthread de los suscribtores, 1 por suscriptor
 * @param rdata Data generica pasada al thread
 */
void subSuscriptorThread(void *rdata) {
    // mask de signals
    int sig;
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGUSR1); // SIGUSR1 para enviar noticia
    sigaddset(&signal_set, SIGUSR2); // SIGUSR2 para terminar el thread
    pthread_sigmask(SIG_BLOCK, &signal_set, NULL);

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
    for (char *p = data->news_preference; *p != '\0'; p++) {
        if (*p == '-')
            continue;
        node_t *t = data->news[toNewsIndex(*p)].first;
        while (t != NULL) {
            write(fd, &t->data.news, sizeof(Noticia));
            t = t->next;
        }
    }
    close(fd);
    while (true) {
        sigwait(&signal_set, &sig);
        if (sig == SIGUSR2) {
            break;
        }
        if (strchr(data->news_preference, new_new) != NULL) {
            // si la nueva noticia recibida es de interes enviarla
            fd = open(buff, O_WRONLY);
            if (fd < 0) {
                perror("Opening pipe");
                exit(1);
            }
            write(fd, &data->news[toNewsIndex(new_new)].last->data.news, sizeof(Noticia));
            close(fd);
        }
    }
    free(data);
    pthread_exit(NULL);
}
/**
 * @brief Thread general de los suscriptores que escucha el pipe por peticiones
 * @param rdata data generica al thread
 */
void suscriptorThread(void *rdata) {
    signal(SIGTERM, suscriptor_handler);
    suscriptor_thread_t *data = rdata;
    int fd = open(data->inputPipe, O_RDONLY);
    if (fd < 0) {
        perror("Error reading suscriptor pipe ");
        exit(errno);
    }

    // serían varios 1 por suscriptor, podría hacerse con la lista pero no veo posible que hayan más de 100 suscriptores

    while (suscriptor_thread_run) {
        Suscriptor received;
        generic_t temporal;
        size_t bytes = read(fd, &received, sizeof(Suscriptor));
        sub_suscriptor_thread_t *sub_data = malloc(sizeof(sub_suscriptor_thread_t));

        if (bytes != 0) {
            // asumiendo que los suscriptores nunca se desconectan
            sub_data->news = data->news;
            sub_data->pid = received.pid;
            strcpy(sub_data->news_preference, received.categorias);
            temporal.pid = received.pid;
            insertList(data->list, temporal);

            pthread_mutex_lock(&sub_suscriptor_threads_mutex);
            if (pthread_create(&sender[sender_index], NULL, (void *)subSuscriptorThread, sub_data) < 0) {
                perror("Error creating thread for suscribers 2 ");
                exit(errno);
            }
            sender_index++;
            pthread_mutex_unlock(&sub_suscriptor_threads_mutex);
        } 
    }
    pthread_exit(NULL);
}
