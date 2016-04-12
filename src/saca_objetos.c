/**
 * @file saca_objetos.c
 *
 *
 * Este c&oacute;digo contiene las funciones basicas para interactuar con archivos shp, parsea
 * un archivo y determina la cantidad de objetos al leer el encabezado, posteriormente saca todos
 * los elementos.
 *
 * @author AAFR, GENEC, S.A. de C.V.
 * @version 1.0
 * @date 22 de noviembre de 2003
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/usr/include/libshp/shapefil.h"


#include "lector_shape.h"

typedef SHPObject *SHPO;

int bimp_cs = 0;

extern int SacaInfObjeto(char *nomarch, FILE *ae, FILE *aex, int objeto);


extern void imprimeHeaderXML(FILE *aex);
extern void imprimeFooterXML(FILE *aex);
extern void imprimeTag(FILE *aex, char *etiqueta, char *valor);
extern void imprimeTagMA(FILE *aex, char *etiqueta, char *valor, char *av);


// Definiciones de funciones 
/**
 * @brief Funci&oacute;n  que  obtiene todos los objetos de un shapefile,
 *        leyendo el encabezado.
 *
 * <ul>
 * <li> Abre el archivo Shapefile
 * <li> Carga el encabezado, determinando limites y tipos de objetos
 * <li> Determina la cantidad de records
 * <li> Crea el archivo de salida
 * </ul>
 * @param nomarch Nombre del archivo ShapeFile
 *
 * @return  Regresa 1, si todo sale bien, en caso contrario regresa 0.
 */
int SacaObjetos(char *nomarch);

/**
 * @brief Funci&oacute;n que saca un objeto especifico y lo escribe el
 *        archivo proporcionado
 *
 * @param shph Handler del archivo ShapeFile
 * @param nomarch Nombre del archivo
 * @param ae Puntero al archivo de salida de formato intermedio
 * @param aex Puntero al archivo de salida de xml diccionario
 * @param objeto N&uacute;mero de objeto a extraer
 *
 * @return Regresa 1, si pudo extraer el objeto, en caso contrario regresa cero
 * @see SHPReadObject,
 */
int SacaObjeto(SHPHandle shph, char *nomarch, FILE *ae, FILE *aex, int objeto);

/**
 * @brief Funcion que saca cada uno de los objetos
 */
int SacaObjetoU(SHPO shpo, char *nomarch, FILE *ae, FILE *aex, int objeto, int i_parte, int f_parte, double *X, double *Y);

int SacaObjetos(char * nomarch) {
    SHPHandle shph;
    int *pnEnt = NULL;
    int *pnTEnt = NULL;
    double *dmin = NULL;
    double *dmax = NULL;
    int numobj;
    int i;

    char *archshp = NULL;

    char *archsal = NULL;
    char *archsalx = NULL;


    FILE *ae = NULL;
    FILE *aex = NULL;

    printf("Comienza la extraccion de objetos. \n");

    char *buff = (char *) calloc(1000, sizeof (char));


    //apertura del archivo de salida en formato intermedio
    archsal = (char *) calloc(500, sizeof (char));
    sprintf(archsal, "%s.int", nomarch);
    printf("Archivo INT: %s\n", archsal);
    ae = (FILE *) fopen(archsal, "w");

    //apertura del archivo de salida xml (diccionario)
    archsalx = (char *) calloc(500, sizeof (char));
    sprintf(archsalx, "%s.xml", nomarch);
    printf("Archivo XML: %s\n", archsalx);
    aex = (FILE *) fopen(archsalx, "w");
    imprimeHeaderXML(aex);

    //apertura del archivo de shapefile

    printf("Archivo general: %s\n", nomarch);

    archshp = (char *) calloc(strlen(nomarch) + 10, sizeof (char));
    sprintf(archshp, "%s.shp", nomarch);

    printf("Archivo SHP: %s\n", archshp);


    shph = SHPOpen(archshp, "rb");
    SHPGetInfo(shph, pnEnt, pnTEnt, dmin, dmax); //carga el encabezado

    numobj = shph->nRecords; //saca la cantidad de objetos en un documento


    printf("numero de objetos: %i\n", numobj);

    sprintf(buff, "%i", numobj);

    imprimeTag(aex, "nobjetos", buff);

    for (i = 0; i < numobj; i++) {
        printf("%i\n", i);
        SacaObjeto(shph, nomarch, ae, aex, i);

    }

    SHPClose(shph);
    if (archshp)free(archshp);


    imprimeFooterXML(aex);
    if (aex)fclose(aex);
    if (archsalx)free(archsalx);

    if (ae)fclose(ae);
    if (archsal)free(archsal);


    free(buff);

    return 1;
}

int SacaObjeto(SHPHandle shph, char *nomarch, FILE *ae, FILE *aex, int objeto) {
    SHPO shpo = NULL;

    int p = 0;

    int i_parte = 0;
    int f_parte = 0;
    double *X = NULL, *Y = NULL;



    shpo = (SHPO) calloc(sizeof (SHPO), 1);

    shpo = SHPReadObject(shph, objeto);

    if (shpo->nSHPType == SHPT_NULL) {
        return 0;
    }


    X = (double *) calloc(sizeof (double), shpo->nVertices);
    Y = (double *) calloc(sizeof (double), shpo->nVertices);
    X = shpo->padfX;
    Y = shpo->padfY;

    if (shpo->nParts > 0) {
        fprintf(stderr, "Multiples partes. %d\n", shpo->nParts);

        for (p = 0; p < shpo->nParts; p++) {

            i_parte = *(shpo->panPartStart + p);

            if ((p + 1) == shpo->nParts) {
                f_parte = shpo->nVertices;
            } else {
                f_parte = *(shpo->panPartStart + p + 1);
            }
            printf("Parte %d  de %d hasta %d (%d)\n", p, i_parte, f_parte, shpo->nVertices);

            SacaObjetoU(shpo, nomarch, ae, aex, objeto, i_parte, f_parte, X, Y);
        }

    } else {
        SacaObjetoU(shpo, nomarch, ae, aex, objeto, 0, 1, X, Y);
    }

    free(X);
    free(Y);

    if (shpo)free(shpo);
    return 1;
}

int SacaObjetoU(SHPO shpo, char *nomarch, FILE *ae, FILE *aex, int objeto, int i_parte, int f_parte, double *X, double *Y) {

    int i = i_parte;

    //Determina e imprime el tipo de objeto del que se trata
    switch (shpo->nSHPType) {
        case SHPT_NULL:
            fprintf(ae, "SHPT_NULL");
            if (bimp_cs == 0)imprimeTag(aex, "tiposhp", "SHPT_NULL");
            break;
        case SHPT_POINT:
            fprintf(ae, "SHPT_POINT");
            if (bimp_cs == 0)imprimeTag(aex, "tiposhp", "SHPT_POINT");
            break;
        case SHPT_ARC:
            fprintf(ae, "SHPT_ARC");
            if (bimp_cs == 0)imprimeTag(aex, "tiposhp", "SHPT_ARC");
            break;
        case SHPT_POLYGON:
            fprintf(ae, "SHPT_POLYGON");
            if (bimp_cs == 0)imprimeTag(aex, "tiposhp", "SHPT_POLYGON");
            break;
        case SHPT_MULTIPOINT:
            fprintf(ae, "SHPT_MULTIPOINT");
            if (bimp_cs == 0)imprimeTag(aex, "tiposhp", "SHPT_MULTIPOINT");
            break;
        default:
            fprintf(ae, "SHPT_INDEFINIDO");
            if (bimp_cs == 0)imprimeTag(aex, "tiposhp", "SHPT_INDEFINIDO");
            break;
    }

    if (bimp_cs == 0) {
        imprimeTagMA(aex, "campo", "", "nombre=\"TIPOSHP\" tipo=\"string\"");
        imprimeTagMA(aex, "campo", "", "nombre=\"vertices\" tipo=\"int\"");
        imprimeTagMA(aex, "campo", "", "nombre=\"coordenadas\" tipo=\"double,double\"");
    }
    bimp_cs = 1;

    if (shpo->nSHPType != SHPT_NULL) {

        fprintf(ae, SEP);

        fprintf(ae, "%i%s", (f_parte - i_parte), SEP); //se imprime la cantidad de vertices del objeto

        //Saca las coordenadas de cada vertice

        while (1) {
            if (i >= i_parte) {

                fprintf(ae, "%.6f,%.6f", *(X + i), *(Y + i));
            }
            i++;
            if (i < f_parte)fprintf(ae, ",");
            else break;
        }

        SacaInfObjeto(nomarch, ae, aex, objeto); //llama al metodo para consultar el dbf

    }

    fprintf(ae, "\n"); //finaliza el objeto en formato intermedio

    return 1;
}