#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pokedex.h"
#include "archivo.h"

#define FORMATO ';'
#define LEER 1
#define TERMINAR_LEER 2

#define TOTAL_DATOS_PKM 6
#define NO_HAY_PKM 0

typedef struct pokemon pokemon_t;

struct pokedex {
	pokemon_t *pokemones;
	pokemon_t **pokemones_por_id;
	pokemon_t **pokemones_por_nombre;
	unsigned cant_pokemones;
};

/**
 * reserva memoria para un puntero a la pokedex e inicializa sus campos principales.
 * 
 * Devuelve un puntero a la pokedex si se inicializo correctamente, NULL en caso contrario.
*/
pokedex_t *inicializar_pokedex()
{
	pokedex_t *pokedex = calloc(1, sizeof(pokedex_t));

	return pokedex;
}

/**
 * reasigna memoria para un puntero al nombre de un pokemon con un tamaño pasado por parametro.
 * 
 * Devuelve True si se reasigno correctamente, False en caso contrario.
*/
bool allocar_nombre_pkm(const char **nombre, size_t nuevo_tam)
{
	free((char *)(*nombre));
	char *nombre_temp = malloc(nuevo_tam);
	if (!nombre_temp) {
		*nombre = NULL;
		return false;
	}
	*nombre = nombre_temp;
	return true;
}

/**
 * reasigna memoria para un puntero a la pokedex para agregar un nuevo pokemon.
 * 
 * Devuelve True si se reasigno correctamente, False en caso contrario.
*/
bool allocar_pokemon(pokedex_t *pokedex, pokemon_t **nuevo_pkm)
{
	pokemon_t *pkm_temp =
		realloc(pokedex->pokemones,
			(pokedex->cant_pokemones + 1) * sizeof(pokemon_t));
	if (!pkm_temp) {
		*nuevo_pkm = NULL;
		return false;
	}
	pokedex->pokemones = pkm_temp;
	*nuevo_pkm = &pokedex->pokemones[pokedex->cant_pokemones];
	(*nuevo_pkm)->nombre = NULL;
	return true;
}

/**
 * Asigna a un pokemon el tipo enviado por parametro.
 * 
 * Devuelve True en caso de haber coincidencias, False en caso contrario.
*/
bool asignar_tipo_pkm(tipo_pokemon *pokemon, char tipo_pkm)
{
	switch (tipo_pkm) {
	case 'A':
		*pokemon = TIPO_AGUA;
		break;
	case 'F':
		*pokemon = TIPO_FUEGO;
		break;
	case 'P':
		*pokemon = TIPO_PLANTA;
		break;
	case 'R':
		*pokemon = TIPO_ROCA;
		break;
	case 'E':
		*pokemon = TIPO_ELECTRICO;
		break;
	case 'N':
		*pokemon = TIPO_NORMAL;
		break;
	case 'L':
		*pokemon = TIPO_LUCHA;
		break;
	default:
		return false;
	}
	return true;
}

/**
 * Devuelve la cantidad de caracteres que tiene el nombre del pokemon.
*/
size_t contar_longitud_nombre_pkm(const char *linea)
{
	const char *primer_sep = strchr(linea, FORMATO);
	if (!primer_sep)
		return 0;

	const char *segundo_sep = strchr(primer_sep + 1, FORMATO);
	if (!segundo_sep)
		return 0;

	return (size_t)(segundo_sep - (primer_sep + 1));
}

/**
 * Libera la memoria reservada del puntero a pokedex.
 * 
 * Tras liberar retorna NULL.
*/
pokedex_t *destruir_pokedex_invalida(pokedex_t *pokedex)
{
	if (pokedex->pokemones) {
		for (size_t i = 0; i < pokedex->cant_pokemones; i++) {
			free((char *)pokedex->pokemones[i].nombre);
		}
		free(pokedex->pokemones);
	}
	free(pokedex->pokemones_por_id);
	free(pokedex->pokemones_por_nombre);
	free(pokedex);
	return NULL;
}

/**
 * Inicializa dos vectores de pokemones y copia la informacion de un puntero pokedex pasado por parametro.
 * 
 * Ordena un vector por id/creciente y el otro por nombre/creciente.
 * 
 * Devuelve True en caso de ser creados exitosamente, False en caso contrario.
*/
bool crear_vectores_ordenados(pokedex_t *pokedex)
{
	pokedex->pokemones_por_id =
		malloc(pokedex->cant_pokemones * sizeof(pokemon_t *));
	pokedex->pokemones_por_nombre =
		malloc(pokedex->cant_pokemones * sizeof(pokemon_t *));

	if (!pokedex->pokemones_por_id || !pokedex->pokemones_por_nombre) {
		free(pokedex->pokemones_por_id);
		free(pokedex->pokemones_por_nombre);
		return false;
	}

	for (unsigned i = 0; i < pokedex->cant_pokemones; i++) {
		pokedex->pokemones_por_id[i] = &pokedex->pokemones[i];
		pokedex->pokemones_por_nombre[i] = &pokedex->pokemones[i];
	}

	for (size_t i = 1; i < pokedex->cant_pokemones; i++) {
		pokemon_t *actual = pokedex->pokemones_por_id[i];
		int j = (int)i - 1;
		while (j >= 0 &&
		       pokedex->pokemones_por_id[j]->id > actual->id) {
			pokedex->pokemones_por_id[j + 1] =
				pokedex->pokemones_por_id[j];
			j--;
		}
		pokedex->pokemones_por_id[j + 1] = actual;
	}
	for (size_t i = 1; i < pokedex->cant_pokemones; i++) {
		pokemon_t *actual = pokedex->pokemones_por_nombre[i];
		int j = (int)i - 1;
		while (j >= 0 &&
		       strcmp(pokedex->pokemones_por_nombre[j]->nombre,
			      actual->nombre) > 0) {
			pokedex->pokemones_por_nombre[j + 1] =
				pokedex->pokemones_por_nombre[j];
			j--;
		}
		pokedex->pokemones_por_nombre[j + 1] = actual;
	}

	return true;
}

/**
 * Función auxiliar para procesar una línea del archivo y agregar el Pokémon a la pokedex
 */
bool procesar_linea_pokedex(pokedex_t *pokedex, const char *linea)
{
	size_t long_nombre = contar_longitud_nombre_pkm(linea);
	if (long_nombre == 0)
		return false;

	pokemon_t *pkm = NULL;
	if (!allocar_pokemon(pokedex, &pkm))
		return false;

	if (!allocar_nombre_pkm(&pkm->nombre, long_nombre + 1))
		return false;

	memset((char *)pkm->nombre, 0, long_nombre + 1);

	char tipo_pkm = '\0';
	int leidos = sscanf(linea, "%u;%[^;];%c;%u;%u;%u", &pkm->id,
			    (char *)pkm->nombre, &tipo_pkm, &pkm->fuerza,
			    &pkm->destreza, &pkm->inteligencia);

	if (leidos != TOTAL_DATOS_PKM)
		return false;

	if (!asignar_tipo_pkm(&pkm->tipo, tipo_pkm))
		return false;

	return true;
}

/**
 * Crea una nueva pokedex a partir de un archivo.
 * 
 * Devuelve un puntero a la pokedex si se pudo abrir el archivo, o NULL en caso contrario.
 * 
 * Cada pokemon en el archivo debe tener el siguiente formato: id;nombre;tipo;fuerza;destreza;inteligencia
 * 
 * Al encontrar un pokemon inválido, se ignora y deja de leer del archivo. 
 * 
 * Se leen todos los pokemon válidos del archivo y se almacenan en la pokedex. Si no se pudo leer ningún pokemon válido, se devuelve NULL.
 * 
*/
pokedex_t *pokedex_abrir(const char *archivo)
{
	pokedex_t *pokedex = inicializar_pokedex();
	if (!pokedex)
		return NULL;

	Archivo *data = archivo_abrir(archivo);
	if (!data) {
		destruir_pokedex_invalida(pokedex);
		return NULL;
	}

	bool fallo_formato = false;
	while (archivo_hay_mas_lineas(data) && !fallo_formato) {
		const char *linea = archivo_leer_linea(data);

		if (procesar_linea_pokedex(pokedex, linea)) {
			pokedex->cant_pokemones++;
		} else {
			fallo_formato = true;
			if (pokedex->cant_pokemones > 0) {
				pokemon_t *ultimo =
					&pokedex->pokemones
						 [pokedex->cant_pokemones - 1];
				if (ultimo->nombre) {
					free((char *)ultimo->nombre);
					ultimo->nombre = NULL;
					pokedex->cant_pokemones--;
				}
			}
		}
	}

	archivo_cerrar(data);

	if ((pokedex->cant_pokemones) == NO_HAY_PKM)
		return destruir_pokedex_invalida(pokedex);

	if (!crear_vectores_ordenados(pokedex))
		return destruir_pokedex_invalida(pokedex);

	return pokedex;
}

/**
 * Devuelve la cantidad de pokemones en la pokedex.
*/
unsigned pokedex_cantidad_pokemones(pokedex_t *pokedex)
{
	if (!pokedex)
		return NO_HAY_PKM;

	return pokedex->cant_pokemones;
}

/**
 * La función devuelve un puntero al pokemon según el criterio de busqueda dado (por nombre o por id).
 * Devuelve NULL en caso contrario.
*/
const struct pokemon *pokedex_buscar_pokemon(pokedex_t *pokedex,
					     enum modo_iteracion modo,
					     void *filtro)
{
	if (!pokedex || !filtro)
		return NULL;

	pokemon_t *pkm = NULL;
	bool encontro_pokemon = false;
	size_t i = 0;
	while (i < pokedex->cant_pokemones && !encontro_pokemon) {
		pkm = &pokedex->pokemones[i];

		if (modo == ITERAR_NOMBRE) {
			const char *nombre_buscado = (const char *)filtro;
			if (strcmp(pkm->nombre, nombre_buscado) == 0)
				encontro_pokemon = true;
		} else if (modo == ITERAR_ID) {
			unsigned id_buscado = *(unsigned *)filtro;
			if (pkm->id == id_buscado)
				encontro_pokemon = true;
		}
		i++;
	}

	if (encontro_pokemon)
		return pkm;

	return NULL;
}

/**
 * Busca un pokemon con el nombre especificado en la pokedex.
 * 
 * Devuelve un puntero al pokemon si se encontró, o NULL en caso contrario.
*/
const struct pokemon *pokedex_buscar_pokemon_nombre(pokedex_t *pokedex,
						    const char *nombre)
{
	return pokedex_buscar_pokemon(pokedex, ITERAR_NOMBRE, (void *)nombre);
}

/**
 * Busca un pokemon con el id especificado en la pokedex.
 * 
 * Devuelve un puntero al pokemon si se encontró, o NULL en caso contrario.
*/
const struct pokemon *pokedex_buscar_pokemon_id(pokedex_t *pokedex, unsigned id)
{
	return pokedex_buscar_pokemon(pokedex, ITERAR_ID, (void *)&id);
}

/**
 * Invoca la función especificada para cada pokemon en la pokedex.
 * 
 * La función se debe invocar con los pokemon ordenados según el criterio de iteracion dado (creciente por nombre o por id).
 * 
 * La función de iteración recibe un puntero al pokemon y un puntero al contexto y devuelve 
 * true si se debe seguir iterando, o false en caso contrario.
 * 
 * Devuelve la cantidad de pokemones iterados.
 * 
 * Restriccion del TP: implementar con complejidad O(n) en tiempo y O(1) en espacio.
*/
unsigned pokedex_iterar_pokemones(pokedex_t *pokedex, enum modo_iteracion modo,
				  bool (*funcion)(struct pokemon *, void *),
				  void *ctx)
{
	unsigned i = 0;
	unsigned iterados = 0;
	unsigned cantidad = 0;
	bool seguir_iterando = true;
	pokemon_t **lista = NULL;

	if (pokedex && funcion) {
		if (modo == ITERAR_ID) {
			lista = pokedex->pokemones_por_id;
		} else {
			lista = pokedex->pokemones_por_nombre;
		}
		cantidad = pokedex->cant_pokemones;
	}

	while (i < cantidad && seguir_iterando) {
		seguir_iterando = funcion(lista[i], ctx);
		iterados++;
		i++;
	}

	return iterados;
}

/**
 * Destruye la pokedex.
*/
void pokedex_destruir(pokedex_t *pokedex)
{
	if (!pokedex)
		return;

	for (size_t i = 0; i < pokedex->cant_pokemones; i++) {
		free((char *)pokedex->pokemones[i].nombre);
	}

	free(pokedex->pokemones);
	free(pokedex->pokemones_por_id);
	free(pokedex->pokemones_por_nombre);

	free(pokedex);

	return;
}