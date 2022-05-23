/**
 * @file noticia.h
 * @author Javier Ramirez
 * @brief Tipo de dato noticia, la que se recibe por el pipe
 * 
 */
#pragma once
#define MAXDAT 1000
#define MAXINF 81 
typedef struct noticia {
    char info[MAXINF];
    char tipo;
    int len;
} Noticia;