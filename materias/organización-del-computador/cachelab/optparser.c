#include <stdlib.h>
#include <string.h>
#include "optparser.h"

cache_config_t param_c = { 0 };

/**
 * Devuelve 1 si n es potencia de 2; 0 en caso contrario
*/
int es_potencia_de_2(unsigned int n)
{
	return n && !(n & (n - 1));
}

int argparse(int argc, char *argv[])
{
	int opt;

	while ((opt = getopt(argc, argv, "v")) != -1) {
		switch (opt) {
		case 'v':
			param_c.modo_verboso = 1;
			break;
		case '?':
			fprintf(stderr,
				"Uso: %s archivo_traza C E S [ -v n m ]\n",
				argv[0]);
			return -1;
		}
	}

	if (argc != 5 && argc != 8) {
		fprintf(stderr, "Uso: %s archivo_traza C E S [ -v n m ]\n",
			argv[0]);
		return -1;
	}

	param_c.archivo_traza = argv[optind];
	param_c.tamanio_cache = atoi(argv[optind + 1]);
	param_c.asociatividad = atoi(argv[optind + 2]);
	param_c.num_sets = atoi(argv[optind + 3]);
	if (param_c.modo_verboso == 1) {
		param_c.n = atoi(argv[optind + 4]);
		param_c.m = atoi(argv[optind + 5]);
	} else {
		param_c.n = 0;
		param_c.m = 0;
	}

	if (param_c.tamanio_cache == 0 || param_c.asociatividad == 0 ||
	    param_c.num_sets == 0) {
		fprintf(stderr,
			"Error: Tamaño de caché, asociatividad y cantidad de sets deben ser mayores a 0.\n");
		return -1;
	} else if (param_c.tamanio_cache <
		   param_c.asociatividad * param_c.num_sets) {
		fprintf(stderr,
			"Error: Tamaño de caché debe ser mayor o igual a asociatividad * cantidad de sets.\n");
		return -1;
	} else if (!es_potencia_de_2(param_c.tamanio_cache) ||
		   !es_potencia_de_2(param_c.asociatividad) ||
		   !es_potencia_de_2(param_c.num_sets)) {
		fprintf(stderr,
			"Error: Tamaño de caché, asociatividad y cantidad de sets deben ser potencias de 2.\n");
		return -1;
	}
	if (param_c.modo_verboso == 1 &&
	    (param_c.m == 0 || param_c.n > param_c.m)) {
		fprintf(stderr,
			"Error: Si se usa el modo verboso, n debe ser mayor o igual a 0 y m debe ser mayor a 0.\n");
		return -1;
	}

	FILE *archivo_traza = fopen(param_c.archivo_traza, "r");
	if (!archivo_traza) {
		fprintf(stderr, "No se pudo abrir el archivo de traza: %s\n",
			param_c.archivo_traza);
		return -1;
	}
	fclose(archivo_traza);

	return 0;
}

int leer_traza(FILE *archivo_traza, traza_t *traza)
{
	char linea[MAX_LINEA];
	if (fgets(linea, sizeof(linea), archivo_traza) == NULL)
		return 0;

	int n = sscanf(linea, "0x%8x: %c 0x%8x %u %x", &traza->ip, &traza->op,
		       &traza->dir, &traza->tam, &traza->dat);
	if (n != 5)
		return 0;
	return 1;
}