/**
 * @file sc.c
 * @author Santiago Z 
 * @brief Sistema de comunicación central 
 */
#include <stdio.h>
#include "../util/flag.h"


void init(Flags f){
        
}
int main(int argc, char* argv[]){
    if(argc!= 7){
        printf("Error \n");
        exit(22);
        //TODO implementar ayuda
    }
    Flags flags = determineFlags(argc, argv);
    


}
