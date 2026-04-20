#include "cache.h"

cache_t *cache_crear()
{
	cache_t *cache = malloc(sizeof(cache_t));
	if (!cache) {
		fprintf(stderr, "Error al crear la caché.\n");
		return NULL;
	}

	cache->tamanio_cache = param_c.tamanio_cache;
	cache->asociatividad = param_c.asociatividad;
	cache->cant_sets = param_c.num_sets;
	cache->contador_lru = 0;

	// Reservar espacio para todas las líneas: sets * lineas por set
	cache->lineas = malloc(cache->cant_sets * cache->asociatividad *
			       sizeof(linea_t));
	if (!cache->lineas) {
		fprintf(stderr, "Error al crear las líneas de la caché.\n");
		free(cache);
		return NULL;
	}

	for (unsigned i = 0; i < cache->cant_sets * cache->asociatividad; i++) {
		cache->lineas[i].valido = 0;
		cache->lineas[i].dirty = 0;
		cache->lineas[i].tag = -1;
		cache->lineas[i].lru = 0;
		cache->lineas[i].last_used_op_idx = 0;
	}

	// Inicializar métricas
	cache->metricas = (metricas_t){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	return cache;
}

void procesar_linea(cache_t *cache, traza_t *traza)
{
	unsigned tamanio_bloque = param_c.tamanio_cache /
				  (param_c.asociatividad * param_c.num_sets);
	unsigned offset_bloque = log2(tamanio_bloque);
	unsigned offset_set = log2(param_c.num_sets);

	unsigned set = (traza->dir >> offset_bloque) & ((1 << offset_set) - 1);
	unsigned tag = traza->dir >> (offset_bloque + offset_set);

	int hit = 0, linea_hit = -1, linea_vacia = -1, linea_lru = 0;
	int min_lru = __INT_MAX__;

	for (unsigned linea = 0; linea < cache->asociatividad;
	     linea++) { // Busco en las líneas del set correspondiente
		linea_t *l = &cache->lineas[set * cache->asociatividad + linea];
		if (l->valido &&
		    l->tag ==
			    tag) { // Si hay una línea válida con el tag buscado, es un hit
			hit = 1;
			linea_hit = linea;
			break;
		}
		if (!l->valido &&
		    linea_vacia ==
			    -1) // Si no, busco una línea vacía o la menos recientemente usada (LRU)
			linea_vacia = linea;
		if (l->valido && l->lru < min_lru) {
			min_lru = l->lru;
			linea_lru = linea;
		}
	}

	if (traza->op == 'R') // Actualización de métricas
		cache->metricas.loads++;
	else if (traza->op == 'W')
		cache->metricas.stores++;

	int linea_a_usar; // Determinar la línea que se va a usar
	if (hit)
		linea_a_usar = linea_hit;
	else if (linea_vacia != -1)
		linea_a_usar = linea_vacia;
	else
		linea_a_usar = linea_lru;

	linea_t *l = &cache->lineas[set * cache->asociatividad + linea_a_usar];

	int dirty_miss = (l->valido && l->dirty);

	int prev_valid = l->valido;
	int prev_tag = prev_valid ? (int)l->tag : -1;
	int prev_dirty = l->dirty;
	unsigned prev_last_used = l->last_used_op_idx;

	static unsigned op_idx = 0;
	if (param_c.modo_verboso &&
	    op_idx >= param_c.n && // Imprimo los detalles del modo verboso
	    op_idx <= param_c.m) {
		const char *caso = hit ? "1" : (dirty_miss ? "2b" : "2a");
		printf("%u %s %x %x %u ", op_idx, caso, set, tag, linea_a_usar);
		if (prev_valid)
			printf("%x", prev_tag);
		else
			printf("%d", prev_tag);
		printf(" %d %d", prev_valid, prev_dirty);
		if (cache->asociatividad > 1)
			printf(" %u", prev_last_used);
		printf("\n");
	}
	op_idx++;

	l->last_used_op_idx = op_idx - 1;

	if (hit) { // Actualizo metricas relacionadas a un hit
		l->lru = ++cache->contador_lru;
		if (traza->op == 'W')
			l->dirty = 1;
		if (traza->op == 'R')
			cache->metricas.read_time += 1;
		else
			cache->metricas.write_time += 1;
		return;
	}

	if (traza->op ==
	    'R') { // Actualizo métricas relacionadas a un miss/dirty miss de lectura
		cache->metricas.rmiss++;
		cache->metricas.bytes_read += tamanio_bloque;
		if (dirty_miss) {
			cache->metricas.dirty_rmiss++;
			cache->metricas.bytes_written += tamanio_bloque;
			cache->metricas.read_time += 1 + 2 * PENALTY_MEM;
		} else
			cache->metricas.read_time += 1 + PENALTY_MEM;
	} else { // Actualizo métricas relacionadas a un miss/dirty miss de escritura
		cache->metricas.wmiss++;
		cache->metricas.bytes_read += tamanio_bloque;
		if (dirty_miss) {
			cache->metricas.dirty_wmiss++;
			cache->metricas.bytes_written += tamanio_bloque;
			cache->metricas.write_time += 1 + 2 * PENALTY_MEM;
		} else
			cache->metricas.write_time += 1 + PENALTY_MEM;
	}

	// Actualizar línea
	l->valido = 1;
	l->tag = tag;
	l->lru = ++cache->contador_lru;
	if (traza->op == 'W')
		l->dirty = 1;
	else if (traza->op == 'R')
		l->dirty = 0;
}

void imprimir_metricas(cache_t *cache)
{
	if (cache->asociatividad == 1)
		printf("direct-mapped, %u sets, size = %uKB\n",
		       cache->cant_sets, cache->tamanio_cache / 1024);
	else
		printf("%u-way, %u sets, size = %uKB\n", cache->asociatividad,
		       cache->cant_sets, cache->tamanio_cache / 1024);
	printf("loads %u stores %u total %u\n", cache->metricas.loads,
	       cache->metricas.stores,
	       cache->metricas.loads + cache->metricas.stores);
	printf("rmiss %u wmiss %u total %u\n", cache->metricas.rmiss,
	       cache->metricas.wmiss,
	       cache->metricas.rmiss + cache->metricas.wmiss);
	printf("dirty rmiss %u dirty wmiss %u\n", cache->metricas.dirty_rmiss,
	       cache->metricas.dirty_wmiss);
	printf("bytes read %u bytes written %u\n", cache->metricas.bytes_read,
	       cache->metricas.bytes_written);
	printf("read time %u write time %u\n", cache->metricas.read_time,
	       cache->metricas.write_time);
	printf("miss rate %.6f\n",
	       (cache->metricas.rmiss + cache->metricas.wmiss) /
		       (double)(cache->metricas.loads +
				cache->metricas.stores));
}

void cache_destruir(cache_t *cache)
{
	if (cache)
		if (cache->lineas)
			free(cache->lineas);
	free(cache);
}