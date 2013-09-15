/**
 * @file saca_records.c
 *
 *
 * Este c&oacute;digo contiene la funci&oacute;n basicas para interactuar con archivos dbf
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

/**
 * @brief Contiene informacion de los campos del archivo DBF
 * @struct InfCampos
 */
typedef struct InfCampos {
    DBFFieldType dbfft;
    char nombre[12];
    int largo;
    int decimales;
} InfC;


typedef struct InfCampos *PInfC;

int bimp_nc = 0;

int bimp_tc = 0;

int numpos = 4;

extern void imprimeTag(FILE *aex, char *etiqueta, char *valor);
extern void imprimeTagA(FILE *aex, char *etiqueta, char *valor, char *atribu, char *ava);
extern void imprimeTagMA(FILE *aex, char *etiqueta, char *valor, char *av);

char* limpiaCadena(const char* sbuff,char * buffaux);

/**
 * @brief Funci&oacute;n que saca la informaci&oacute;n relacionada con un objeto determinado, si se le
 *        proporciona el archivo dbf y el objeto.
 *
 * <ul>
 * <li> Abre el archivo dbf
 * <li> Determina el n&uacute;mero de campos en la tabla (columnas)
 * <li> Recupera la informaci&oacute;n de los campos
 * <li> Recupera la informaci&oacute;n de cada campo y lo imprime en el archivo
 * </ul>
 *
 * @param nomchar Nombre del archivo dbf
 * @param ae Puntero al archivo de salida de formato intermedio
 * @param aex Puntero al archivo XML de diccionario
 * @param objeto N&uacute;mero del objeto del que se desea conocer la informacion
 *
 * @return Regresa un 1 si todo sale bien o 0 si hubo algun error
 */
int SacaInfObjeto(char *nomarch, FILE *ae, FILE *aex, int objeto);

int SacaInfObjeto(char *nomarch, FILE *ae, FILE *aex, int objeto) {

    DBFHandle dbfh = NULL;
    int numcampos, i;
    PInfC pinfc = NULL;
    char *archdbf = NULL;

    char *buff = (char *) calloc(1000, sizeof (char));
    char *buffaux;

    //abre el dbf para lectura
    archdbf = (char*) calloc(strlen(nomarch) + 5, sizeof (char));
    sprintf(archdbf, "%s.dbf", nomarch);
    dbfh = DBFOpen(archdbf, "rb");

    if (dbfh == NULL) {
        fprintf(stderr, "Error no se puede consultar el archivo: %s\n", archdbf);
        if (archdbf)free(archdbf);
        return 0;
    }

    //determina el numero de campos (columnas)
    numcampos = DBFGetFieldCount(dbfh);

    //crea un array de las estructuras InfCampos
    pinfc = (PInfC) calloc(numcampos, sizeof (InfC));

    //recupera la informacion de los campos para cada uno
    for (i = 0; i < numcampos; i++) {
        (pinfc + i)->dbfft = DBFGetFieldInfo(dbfh, i, pinfc->nombre, &(pinfc->largo), &(pinfc->decimales));



        //Imprime el separador de campo
        fprintf(ae, SEP);

        if ((pinfc + i)->dbfft == 0) {
            

            buffaux=(char *)calloc(strlen(DBFReadStringAttribute(dbfh, objeto, i)),sizeof(char));
            fprintf(ae, "%s", limpiaCadena(DBFReadStringAttribute(dbfh, objeto, i),buffaux));
            free(buffaux);

            
/*
            fprintf(ae, "%s", DBFReadStringAttribute(dbfh, objeto, i));
*/
            
            if (bimp_tc == 0) {
                sprintf(buff, "nombre=\"%s\" tipo=\"string\" largo=\"%i\" pos=\"%i\"", pinfc->nombre, (pinfc->largo), numpos);
                imprimeTagMA(aex, "campo", "", buff);
            }
        } else if ((pinfc + i)->dbfft == 1) {
            fprintf(ae, "%i", DBFReadIntegerAttribute(dbfh, objeto, i));
            if (bimp_tc == 0) {
                sprintf(buff, "nombre=\"%s\" tipo=\"int\" largo=\"%i\" deci=\"%i\" pos=\"%i\"", pinfc->nombre, (pinfc->largo), (pinfc->decimales), numpos);
                imprimeTagMA(aex, "campo", "", buff);
            }
        } else if ((pinfc + i)->dbfft == 2) {
            fprintf(ae, "%20.6f", DBFReadDoubleAttribute(dbfh, objeto, i));
            if (bimp_tc == 0) {
                sprintf(buff, "nombre=\"%s\" tipo=\"double\" largo=\"%i\" deci=\"%i\" pos=\"%i\"", pinfc->nombre, (pinfc->largo), (pinfc->decimales), numpos);
                imprimeTagMA(aex, "campo", "", buff);
            }
        }else{
        
        }

        numpos++;
    }

    bimp_tc = 1;



    if (bimp_nc == 0) {
        sprintf(buff, "%i", (numcampos + 3));
        imprimeTag(aex, "ncampos", buff);
        bimp_nc = 1;
    }

    if (pinfc)free(pinfc);

    DBFClose(dbfh);
    if (archdbf)free(archdbf);

    free(buff);

    return 1;
}


char* limpiaCadena(const char* sbuff,char *buffaux){
    
    int tam=0;
    int i=0;
    int cb=0;
    
    tam=strlen(sbuff);
    

    int j=0;
    
    
    for(i=0;i<tam;i++){
        cb=(int)*(sbuff+i);
        if(cb>=32){
            *(buffaux+j)=*(sbuff+i);
            j++;
        }
    }
    
    return buffaux;
}
