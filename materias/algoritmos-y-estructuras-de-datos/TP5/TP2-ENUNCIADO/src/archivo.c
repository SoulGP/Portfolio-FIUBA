#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "archivo.h"

#define BUFFER_INICIAL 10
#define MODO_LECTURA "r"

const size_t DUPLICAR_BUFFER = 2;
const int NO_HAY_LINEAS = 0;
const int HAY_LINEAS = 1;
const char FINAL_DE_LINEA = '\0';
const char SALTO_DE_LINEA = '\n';

struct Archivo {
	FILE *puntero_archivo;
	char *linea_actual;
	size_t lineas_leidas;
	bool ultima_linea_vacia;
};

/**
 * Abre un archivo DE TEXTO para su posterior lectura.
 * 
 * Devuelve el archivo si se pudo abrir, NULL en caso contrario.
 */
Archivo *archivo_abrir(const char *nombre)
{
	if (!nombre) {
		return NULL;
	}

	Archivo *archivo = calloc(1, sizeof(Archivo));
	if (!archivo) {
		return NULL;
	}

	archivo->puntero_archivo = fopen(nombre, MODO_LECTURA);
	if (!archivo->puntero_archivo) {
		free(archivo);
		return NULL;
	}

	return archivo;
}

/**
 * Lee una línea del archivo.
 * 
 * Devuelve un puntero a la línea leída, o NULL si no hay más líneas.
 */
const char *archivo_leer_linea(Archivo *archivo)
{
	if (!archivo || !archivo->puntero_archivo) {
		return NULL;
	}

	if (archivo->ultima_linea_vacia) {
		*archivo->linea_actual = FINAL_DE_LINEA;
		archivo->ultima_linea_vacia = false;
		archivo->lineas_leidas++;
		return archivo->linea_actual;
	}

	size_t buffer = BUFFER_INICIAL;
	size_t longitud = 0;
	bool linea_completa = false;
	bool leyo_null = false;
	bool error_memoria = false;

	archivo->linea_actual = realloc(archivo->linea_actual, buffer);
	if (!archivo->linea_actual) {
		return NULL;
	}

	char *posicion = archivo->linea_actual;
	while (!linea_completa && !error_memoria && !leyo_null) {
		char *resultado = fgets(posicion, (int)(buffer - longitud),
					archivo->puntero_archivo);

		if (resultado != NULL || longitud > 0) {
			size_t len_actual = strlen(posicion);
			longitud += len_actual;

			if (longitud > 0 &&
			    archivo->linea_actual[longitud - 1] ==
				    SALTO_DE_LINEA) {
				int caracter = fgetc(archivo->puntero_archivo);
				if (caracter == EOF) {
					archivo->ultima_linea_vacia = true;
				}
				ungetc(caracter, archivo->puntero_archivo);
				archivo->linea_actual[longitud - 1] =
					FINAL_DE_LINEA;
				linea_completa = true;
			} else if (resultado == NULL) {
				archivo->linea_actual[longitud] =
					FINAL_DE_LINEA;
				linea_completa = true;
			} else {
				buffer *= DUPLICAR_BUFFER;
				char *nuevo_buffer =
					realloc(archivo->linea_actual, buffer);
				if (!nuevo_buffer) {
					error_memoria = true;
				} else {
					archivo->linea_actual = nuevo_buffer;
					posicion = archivo->linea_actual +
						   longitud;
				}
			}
		} else {
			leyo_null = true;
		}
	}

	if (error_memoria) {
		free(archivo->linea_actual);
		archivo->linea_actual = NULL;
		return archivo->linea_actual;
	}

	if (leyo_null) {
		return NULL;
	}

	archivo->lineas_leidas++;

	return archivo->linea_actual;
}

/**
 * Indica si hay más líneas por leer en el archivo.
 * 
 * Devuelve 1 si hay más líneas, 0 en caso contrario.
 */
int archivo_hay_mas_lineas(Archivo *archivo)
{
	if (!archivo || !archivo->puntero_archivo) {
		return NO_HAY_LINEAS;
	}

	if (archivo->ultima_linea_vacia) {
		return HAY_LINEAS;
	}

	int caracter = fgetc(archivo->puntero_archivo);
	if (caracter == EOF) {
		return NO_HAY_LINEAS;
	}
	ungetc(caracter, archivo->puntero_archivo);

	return HAY_LINEAS;
}

/**
 * Devuelve la cantidad de líneas leídas hasta el momento.
 * 
 * Devuelve 0 si el archivo es NULL.
 */
int archivo_lineas_leidas(Archivo *archivo)
{
	if (!archivo) {
		return NO_HAY_LINEAS;
	}

	return (int)archivo->lineas_leidas;
}

/**
 * Cierra el archivo y libera toda la memoria reservada.
 */
void archivo_cerrar(Archivo *archivo)
{
	if (!archivo) {
		return;
	}

	if (archivo->puntero_archivo) {
		fclose(archivo->puntero_archivo);
	}

	free(archivo->linea_actual);
	free(archivo);
}