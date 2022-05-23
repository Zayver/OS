/**
 * @file struct.h
 * @author Santiago Z
 * @brief Cabecera de las estructuras de datos de los threads 
 */
#pragma once
#include "../util/list.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <sched.h>
#define NEWS_LEN 5 // 5 tipos de noticia

typedef struct suscriptor_thread_t{
    char * inputPipe; //nombre del imput pipe
    list_t *list; // lista de suscriptores
    list_t * news; //arrays de lista de noticias 

}suscriptor_thread_t;

typedef struct publicator_thread_t{
    char * inputPipe; //nombre del input pipe
    list_t *list; //lista de publicadores suscritos al servidor 
    unsigned int wait_time; //wait time para terminar el servidor
    list_t* news; //arrays de lista de noticia
    pthread_t suscriptor_pid;
}publicator_thread_t;

typedef struct sub_suscriptor_thread_t{
    pid_t pid;
    char news_preference[NEWS_LEN+1]; //+1 por null character
    list_t * news;
}sub_suscriptor_thread_t;

char new_new; //flag para saber cual fue la última noticia en llegar

pthread_t sender[100]; //arreglo de suscriptores
unsigned int sender_index=0; //index y contaror de suscriptores

pthread_mutex_t sub_suscriptor_threads_mutex = PTHREAD_MUTEX_INITIALIZER; //mutex para manejar el arreglo de suscriptores
