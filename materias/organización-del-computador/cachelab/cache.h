#ifndef CACHE_H
#define CACHE_H

#include "optparser.h"
#include <stdlib.h>

#define PENALTY_MEM 100

typedef struct {
	unsigned loads, stores; // total de lecturas / escrituras
	unsigned rmiss, wmiss; // miss en lecturas / escrituras
	unsigned dirty_rmiss, dirty_wmiss;
	unsigned bytes_read, bytes_written;
	unsigned read_time, write_time;
} metricas_t;

typedef struct linea {
	unsigned valido; // 0: invalido, 1: valido
	unsigned dirty; // 0: no modificado, 1: modificado
	unsigned tag;
	int lru; // Contador LRU para reemplazo
	unsigned last_used_op_idx;
} linea_t;

typedef struct cache {
	unsigned tamanio_cache; // C
	unsigned asociatividad; // E
	unsigned cant_sets; // S
	linea_t *lineas;
	unsigned contador_lru; // Contador LRU para reemplazo
	metricas_t metricas;
} cache_t;

/**
 * Crea una nueva caché inicializada con los parámetros de configuración.
 * Devuelve un puntero a la caché creada o NULL si hubo un error.
 */
cache_t *cache_crear();

/**
 * Procesa una línea de traza y actualiza la caché.
 * actualiza las métricas y el estado de las líneas.
 */
void procesar_linea(cache_t *cache, traza_t *traza);

/**
 * Imprime las metricas de la caché.
 * Muestra el tipo de caché, número de sets, tamaño, métricas de acceso,
 * tasa de fallos y tiempos de lectura/escritura.
 */
void imprimir_metricas(cache_t *cache);

/**
 * Libera la memoria reservada utilizada por la caché.
 */
void cache_destruir(cache_t *cache);

#endif /* CACHE_H */