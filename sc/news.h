#pragma once
typedef const enum NewsType{
    ARTE, FARANDULA, CIENCIA, POLITICA, SUCESOS
}NewsType;

NewsType toNewsIndex(const char chr){
    switch(chr){
        case 'A':
            return ARTE;
            break;
        case 'E':
            return FARANDULA;
            break;
        case 'C':
            return CIENCIA;
            break;
        case 'P':
            return POLITICA;
            break;
        case 'S':
            return SUCESOS;
            break;
    }
    return -1;
}