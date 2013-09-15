/**
 * 
 * @file imprime_xml.c
 * 
 * Este codigo concentra todo el manejo de XML
 * 
 * @author AAFR
 * @version 1.0
 * @date 3 de diciembre de 2010
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/usr/include/libshp/shapefil.h"

#include "lector_shape.h"

/**
 * @brief Funcion que imprime el header de un XML bien formado
 * @param aex
 */
void imprimeHeaderXML(FILE *aex);

/**
 * @brief Funcion que imprime el footer del XML
 * @param aex
 */
void imprimeFooterXML(FILE *aex);

/**
 * @brief Funcion que imprime una etiqueta en el diccionario
 * @param aex
 * @param etiqueta
 * @param valor
 */
void  imprimeTag(FILE *aex,char *etiqueta,char *valor);

/**
 * @brief 
 * @param aex
 * @param etiqueta
 * @param valor
 * @param atribu
 * @param ava
 */
void imprimeTagA(FILE *aex, char *etiqueta, char *valor,char *atribu, char *ava);

/**
 * 
 * @param aex
 * @param etiqueta
 * @param valor
 * @param atribu
 * @param ava
 */
void imprimeTagMA(FILE *aex, char *etiqueta, char *valor,char *av);

/**
 * @brief Funcion que procesa los atributos de un xml
 * @param cadena
 * @param atribu
 * @param ava
 * @return char* con la cadena de atributos y valores 
 */
char* procesaAtributos( char *cadena, char *atribu,char *ava);

/**
 * 
 * @param cadena
 * @param atribu
 * @param ava
 * @return 
 */
char* procesaMAtributos( char *cadena, char *av);




void imprimeHeaderXML(FILE *aex) {
    fprintf(aex,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    fprintf(aex,"<root>\n");
}

void imprimeFooterXML(FILE *aex) {
    fprintf(aex,"</root>");
}

void imprimeTag(FILE *aex, char *etiqueta, char *valor) {
    if (strlen(valor) > 0) {
        fprintf(aex, "<%s>%s</%s>\n", etiqueta, valor, etiqueta);
    } else {
        fprintf(aex, "<%s />\n",etiqueta);
    }
}

void imprimeTagA(FILE *aex, char *etiqueta, char *valor,char *atribu, char *ava){
    char *cadena =(char *)calloc(1024,sizeof(char));
    if (strlen(valor) > 0) {
        fprintf(aex, "<%s %s>%s</%s>\n", etiqueta,procesaAtributos(cadena,atribu,ava), valor, etiqueta);
    } else {
        fprintf(aex, "<%s %s />\n",etiqueta,procesaAtributos(cadena,atribu,ava));
    }
    
    free(cadena);
}


char* procesaAtributos( char *cadena, char *atribu,char *ava){
    
    
        strcat(cadena,atribu);
        strcat(cadena,"=\"");
        strcat(cadena,ava);
        strcat(cadena,"\" ");
        
    
    
    return cadena;
}


void imprimeTagMA(FILE *aex, char *etiqueta, char *valor,char *av){
    char *cadena =(char *)calloc(1024,sizeof(char));
    if (strlen(valor) > 0) {
        fprintf(aex, "<%s %s>%s</%s>\n", etiqueta,procesaMAtributos(cadena,av), valor, etiqueta);
    } else {
        fprintf(aex, "<%s %s/>\n",etiqueta,procesaMAtributos(cadena,av));
    }
    
    free(cadena);
}



char* procesaMAtributos( char *cadena, char *av){
    
    
   
        strcat(cadena,av);
    
    
    return cadena;
}
