/**
 * @file news.h
 * @author Santiago Z
 * @brief Cabecera que define los tipos de noticia y un conversor de char tipo a index para manejar las noticias por aparte
 * 
 */
#pragma once
/**
 * @brief Enum para indicar los tipos de noticia
 */
typedef const enum NewsType{
    ARTE, FARANDULA, CIENCIA, POLITICA, SUCESOS
}NewsType;

/**
 * @brief Convierte char a index dependiendo del valor de este
 * @param chr char a convertir
 * @return NewsType según el tipo de noticia, su posición en el index
 */
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