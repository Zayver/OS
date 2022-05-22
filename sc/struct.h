/**
 * @file struct.h
 * @author Santiago Z
 * @brief Cabecera de las estructuras de datos de los threads 
 */
#include "../util/list.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <sched.h>
#pragma once
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
}publicator_thread_t;

typedef struct sub_suscriptor_thread_t{
    pid_t pid;
    char news_preference[NEWS_LEN+1]; //+1 por null character
    list_t * news;
}sub_suscriptor_thread_t;

char new_new; //flag para saber cual fue la última noticia en llegar

pthread_mutex_t publicator_mutex= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t suscriptor_mutex= PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t sub_suscriptor_mutex= PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t sub_suscriptor_cond = PTHREAD_COND_INITIALIZER;

