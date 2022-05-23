/**
 * @file Datos.h
 * @author Mario Ortiz
 * @brief Tipo de dato para el publicador
 * 
 */
#include <sys/types.h>
#pragma once
#define MAXDAT 1000
#define MAXINF 81 //Segun enunciado
typedef struct datos_p {
    char notifica[MAXINF]; //noticia
    char tipo; //tipo de noticia
    int len; //longitud de la noticia
    pid_t pid; //pid del publicador
} datos_p;
