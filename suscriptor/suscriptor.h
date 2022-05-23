/**
 * @file suscriptor.h
 * @author Javier Ramirez
 * @brief Define tipo de dato Suscriptor_ que es el que se envia al SC
 */
#include <sys/types.h>
#pragma once
#define MAXCAT 5

typedef struct Suscriptor_ {
  pid_t pid;
  char categorias[MAXCAT];
} Suscriptor;