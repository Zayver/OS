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

//#define DEBUG(X) printf("%s",X) 


typedef union genetic_t{
    pid_t pid; //! Solo para el publicador ya que no tiene sentido guardar info de este diferente (por el moomento)
    struct Suscriptor_ suscriptor_t;
    struct noticia news;
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
    if(list.size==0)
        return;
    node_t* aux=list.first;
    while(aux!=NULL){
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
    list->size=0;
}

/**
 * @brief 
 * 
 * @param list 
 * @param pid 
 * @return true si ya se encuentra el pid en la lista
 * @return false 
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