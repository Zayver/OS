/**
 * @file test.c
 * @author Santiago Z (s.zunigam@javeriana.edu.co)
 * @brief Implementación simple de una double linked list en C puro con tipo generico
 */
#pragma once
#include "../suscriptor/struct.h"
#include <stdlib.h>

#define DEBUG(X) printf("%s",X) 


typedef union genetic_t{
    struct suscriptor_message_t suscriptor_message_t;
    //struct test b;
} generic_t;

typedef struct node_t{
    struct node_t* prev;
    generic_t data;
    struct node_t* next;
}node_t;

typedef struct list_t{
    unsigned long size;
    node_t *first;
    node_t *last;
}list_t;



list_t initList(){
    list_t l ;
    l.size=0;
    l.first=NULL;
    l.last=NULL;
    return l;
}

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


generic_t * getIndex(list_t list, unsigned int index){
    node_t * aux= list.first;
    if(index>list.size)
        return NULL;

    for(int i=0;i!=index;i++){
        aux=aux->next;
    }
    return &aux->data;
}

void foreachList(list_t list, void func(generic_t (f) )){
    node_t* aux=list.first;
    while(aux->next!=NULL){
        func(aux->data);
        aux=aux->next;
    }
}

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
}