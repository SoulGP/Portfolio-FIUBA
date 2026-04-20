#ifndef OPTPARSER_H
#define OPTPARSER_H

#include <getopt.h>
#include <math.h>
#include <stdio.h>

#define MAX_LINEA 1024

typedef struct {
	const char *archivo_traza; // Nombre del archivo de traza
	unsigned tamanio_cache; // C
	unsigned asociatividad; // E
	unsigned num_sets; // S
	int modo_verboso; // 0: no, 1: sí
	unsigned n; // rango [n, m] lineas a las cuales
	unsigned m; // imprimir x información
} cache_config_t;

typedef struct traza {
	unsigned ip; // Dirección de instrucción
	char op; // Operación: 'R' (lectura) o 'W' (escritura)
	unsigned dir; // Dirección de memoria
	unsigned tam; // Tamaño de la operación
	unsigned dat; // Datos involucrados en la operación
} traza_t;

extern cache_config_t param_c;

/** 
 *  Parsea los argumentos de la línea de comandos.
 *  Uso: <nombre del programa> archivo_traza C E S [ -v n m ]
 *  Devuelve 0 si los argumentos son válidos, -1 en caso de error
 */
int argparse(int argc, char *argv[]);

/**
 * Lee una línea de traza del archivo y la almacena en la estructura traza_t.
 * Devuelve 1 si se leyó correctamente, 0 si no hay más líneas o si hubo un error.
 */
int leer_traza(FILE *archivo_traza, traza_t *traza);

#endif /* OPTPARSER_H */