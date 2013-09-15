/**
 * @file main.c 
 *
 *
 * Este c&oacute;digo es la rutina principal que parsea los archivos de poligonos en formato
 * ShapeFile (*.shp), genera por cada uno de los archivos un archivo de salida en formato
 * intermedio (*.int), para su posterior procesamiento en el c&oacute;digo de corte.
 *
 * @author AAFR, GENEC, S.A. de C.V.
 * @version 1.0
 * @date 22 de noviembre de 2003
 */

#include <stdio.h>
#include <stdlib.h>


// Declaracion de funciones 
/**
 * @brief Funci&oacute;n que indica el uso del codigo
 *
 * @return No regresa ningun valor
 */

void uso(void);

// Funciones Externas definidas en saca_objetos.c
extern int SacaObjetos(char *);

/**
 * @brief Funci&oacute;n principal 
 *
 * @param argc N&uacute;mero de parametros proporcionados 
 * @param argv Punteros a los punteros de los argumnentos proporcionados
 *  
 * @return Regresa un entero 1 si todo va bien 0 si hubo algun problema
 * @retval 1 Todo bien
 * @retval 0 Hubo alg&uacute;n problema
 */

int main(int argc, char ** argv) {
    int i;

    if (argc < 2)uso();

    for (i = 1; i < argc; i++) {

        printf("%i Procesando:\t%s\n", i, *(argv + i));
        SacaObjetos(*(argv + i));
    }



    return 1;
}


void uso(void) {

    fprintf(stderr, "shp2int.exe <lista de shapes a convertir a formato interno sin extensin>\n");

}




