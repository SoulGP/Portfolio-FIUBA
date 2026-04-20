#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "optparser.h"
#include "cache.h"

int main(int argc, char *argv[])
{
	if (argparse(argc, argv))
		return -1;

	cache_t *cache = cache_crear();

	FILE *archivo_traza = fopen(param_c.archivo_traza, "r");
	traza_t traza;
	while (leer_traza(archivo_traza, &traza))
		procesar_linea(cache, &traza);

	fclose(archivo_traza);
	imprimir_metricas(cache);
	cache_destruir(cache);
	return 0;
}
