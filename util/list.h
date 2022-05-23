/**
 * @file test.c
 * @author Santiago Z (s.zunigam@javeriana.edu.co)
 * @brief Implementación simple de una double linked list en C puro con tipo generico
 */
#pragma once
#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../suscriptor/suscriptor.h"
# include "../suscriptor/noticia.h"

/**
 * @brief Tipo de template para lista genérica
 */
typedef union genetic_t{
    pid_t pid;
    struct Suscriptor_ suscriptor_t;
    struct noticia news;
    pthread_t id_suscruptor;
    //struct test b;
} generic_t;

/**
 * @brief Node type
 */
typedef struct node_t{
    struct node_t* prev;
    generic_t data;
    struct node_t* next;
}node_t;

/**
 * @brief List type
 */
typedef struct list_t{
    unsigned long size;
    node_t *first;
    node_t *last;
}list_t;


/**
 * @brief inicializar lista
 * 
 * @return list_t lista inicializada
 */
list_t initList(){
    list_t l ;
    l.size=0;
    l.first=NULL;
    l.last=NULL;
    return l;
}

/**
 * @brief Insertar en lista
 * 
 * @param list Lista en la que insertar
 * @param data dato a insertar
 */
void insertList(list_t *list, generic_t data){
    node_t *add = malloc(sizeof(node_t));
    add->prev=list->last; add->next=NULL;
    add->data= data;
    //add->data=data;
    if(list->size==0){
        list->first= add;
        list->last=add;
    }
    else {
        list->last->next=add;
        list->last=add;
    }
    list->size++;
}

/**
 * @brief Obtener el objeto de la lista según el index
 * @param list Lista
 * @param index index a obtener
 * @return generic_t* puntero al objeto deseado, NULL si no se encuentra
 */
generic_t * getIndex(list_t list, unsigned int index){
    node_t * aux= list.first;
    if(index>list.size)
        return NULL;

    for(int i=0;i!=index;i++){
        aux=aux->next;
    }
    return &aux->data;
}

/**
 * @brief Ejecuta una función para todos los elementos de una lista
 * @param list lista 
 * @param func función de tipo void fn(generic_t)
 */
void foreachList(list_t list, void func(generic_t (f) )){
    if(list.size==0)
        return;
    node_t* aux=list.first;
    while(aux!=NULL){
        func(aux->data);
        aux=aux->next;
    }
}

/**
 * @brief Borra una lista
 * @param list Lista a borrar
 */
void deleteList(list_t* list){
    if(list->size==0)
        return;
    node_t* aux= list->first, *aux2;
    while(aux->next!=NULL){
        aux2= aux;
        aux= aux->next;
        free(aux2);
    }
    free(aux);
    list->size=0;
}

/**
 * @brief Buscar PID en la lista
 * @param list lista a buscar 
 * @param pid pid a buscar
 * @return true si ya se encuentra el pid en la lista
 * @return false si no
 */
bool checkPIDP(list_t list, pid_t pid){
    if(list.size==0)
        return false;
    node_t* aux= list.first;
    while (aux!=NULL) {
        
        if(aux->data.pid== pid){
            return true;

        }
            
        aux= aux->next;
    }
    return false;

}
/**
 * @brief Borrar un elemento de una lista
 * @param list lista
 * @param pid pid a borrar de list
 */
void removeList(list_t* list, pid_t pid){
    node_t* aux= list->first;
    while (aux!=NULL) {
        if(aux->data.pid==pid){
            if(aux->prev!=NULL)
                aux->prev->next= aux->next;
            if(aux->next!=NULL)
                aux->next->prev= aux->prev;
            free(aux);
            list->size--;
            return ;
        }
        aux= aux->next;
    }
}