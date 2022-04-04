/**
 * @file sc.c
 * @author Santiago Z 
 * @brief Sistema de comunicación central 
 */
#include <stdio.h>
#include "../util/flag.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include "suscriptor.h" 
#include <unistd.h>

int main (int argc, char **argv)
{
  int fd, pid, n, bytes=-1;
  Suscriptor s;
  
  mode_t fifo_mode = S_IRUSR | S_IWUSR;
  
  unlink(argv[1]);
  if (mkfifo (argv[1], fifo_mode) == -1) {
     perror("mkfifo");
     exit(1);
  }

  
  fd = open (argv[1], O_RDONLY);
  printf ("Abrio el pipe\n");


  bytes=read(fd,&s,sizeof(Suscriptor));
  printf("categorias %s \n\n", s.categorias);
	

    // Aqui va el codigo de lectura de la informacion por el pipe. 
    
    
}
