#include "hash.h"
#include <string.h>

#define FACTOR_CARGA 0.66
#define CAPACIDAD_INICIAL_MINIMA 3

typedef struct elemento {
	const char *clave;
	void *valor;
	bool borrado;
} elemento_t;

struct hash {
	elemento_t *tabla;
	size_t capacidad;
	size_t cant_claves;
	size_t (*func_hash)(const char *);
};

bool strdup(const char **destino, const char *clave)
{
	char *copia = malloc(strlen(clave) + 1);
	if (!copia)
		return false;

	strcpy(copia, clave);
	*destino = copia;
	return true;
}

/*
 * Función hash por defecto.
 * Utiliza el algoritmo djb2.
 */
size_t func_hash(const char *clave)
{
	size_t hash = 5381;
	size_t c;

	while ((c = (unsigned char)*clave++))
		hash = hash * 33 + c;

	return hash;
}

hash_t *hash_crear(size_t capacidad_inicial)
{
	return hash_crear_con_funcion(capacidad_inicial, func_hash);
}

hash_t *hash_crear_con_funcion(size_t capacidad_inicial,
			       size_t (*f)(const char *))
{
	if (!f)
		return NULL;

	if (capacidad_inicial < CAPACIDAD_INICIAL_MINIMA)
		capacidad_inicial = CAPACIDAD_INICIAL_MINIMA;

	hash_t *h = malloc(sizeof(hash_t));
	if (!h)
		return NULL;

	h->tabla = calloc(capacidad_inicial, sizeof(elemento_t));
	if (!h->tabla) {
		free(h);
		return NULL;
	}

	h->capacidad = capacidad_inicial;
	h->cant_claves = 0;
	h->func_hash = f;

	return h;
}

/*
 * Reubica los elementos existentes en la nueva tabla.
 */
void rehashear_tabla(hash_t *h, elemento_t *tabla_vieja, size_t capacidad_vieja)
{
	for (size_t i = 0; i < capacidad_vieja; i++) {
		if (tabla_vieja[i].clave) {
			if (!tabla_vieja[i].borrado)
				hash_insertar(h, tabla_vieja[i].clave,
					      tabla_vieja[i].valor, NULL);
			free((char *)tabla_vieja[i].clave);
		}
	}
}

/*
 * Redimensiona la tabla de hash con el doble de capacidad.
 *
 * Reubica los elementos existentes en la nueva tabla.
 *
 * Devuelve true si se pudo redimensionar, false en caso contrario.
 */
bool hash_redimensionar(hash_t *h)
{
	size_t nueva_capacidad = h->capacidad * 2;
	elemento_t *nueva_tabla = calloc(nueva_capacidad, sizeof(elemento_t));
	if (!nueva_tabla)
		return false;

	elemento_t *tabla_vieja = h->tabla;
	size_t capacidad_vieja = h->capacidad;

	h->tabla = nueva_tabla;
	h->capacidad = nueva_capacidad;
	h->cant_claves = 0;

	rehashear_tabla(h, tabla_vieja, capacidad_vieja);

	free(tabla_vieja);
	return true;
}

/*
 * Agrega una nueva clave al elemento.
 *
 * Si anterior no es nulo, se guarda un puntero a NULL en el puntero.
 * 
 * Devuelve true si se pudo agregar la clave, false en caso contrario.
 */
bool agregar_nueva_clave(elemento_t *elem, const char *clave, void *valor,
			 void **anterior, size_t *cant_claves)
{
	if (anterior)
		*anterior = NULL;
	if (!strdup(&elem->clave, clave))
		return false;
	elem->valor = valor;
	(*cant_claves)++;
	return true;
}

/*
 * Modifica el valor de una clave existente o la agrega si estaba borrada.
 *
 * Si anterior no es nulo, se guarda un puntero al valor anterior en el puntero.
 * 
 * Devuelve true tras modificar o agregar la clave.
 */
bool modificar_clave_existente(elemento_t *elem, const char *clave, void *valor,
			       void **anterior, size_t *cant_claves)
{
	if (!elem->borrado) {
		if (anterior)
			*anterior = elem->valor;
		elem->valor = valor;
		return true;
	} else {
		if (anterior)
			*anterior = NULL;
		elem->borrado = false;
		elem->valor = valor;
		(*cant_claves)++;
		return true;
	}
}

/*
 * Inserta un elemento en la tabla hash.
 *
 * Si previamente existe un valor asociado a esa clave y anterior no es nulo, se
 * guarda un puntero al valor anterior en el puntero.
 * 
 * Devuelve true si se pudo insertar, false en caso contrario.
 */
bool insertar_elemento(elemento_t *elem, const char *clave, void *valor,
		       void **anterior, size_t *cant_claves)
{
	if (!elem->clave)
		return agregar_nueva_clave(elem, clave, valor, anterior,
					   cant_claves);
	else if (strcmp(elem->clave, clave) == 0)
		return modificar_clave_existente(elem, clave, valor, anterior,
						 cant_claves);
	return false;
}

bool hash_insertar(hash_t *h, const char *clave, void *valor, void **anterior)
{
	if (!h || !h->func_hash || !clave)
		return false;

	if (((float)h->cant_claves / (float)h->capacidad) >= FACTOR_CARGA) {
		bool se_pudo = hash_redimensionar(h);
		if (!se_pudo)
			return se_pudo;
	}

	size_t hash = h->func_hash(clave);
	bool insertado = false;
	size_t i = 0;
	while (i < h->capacidad && !insertado) {
		size_t pos = (hash + i) % h->capacidad;
		elemento_t *elem = &h->tabla[pos];

		insertado = insertar_elemento(elem, clave, valor, anterior,
					      &h->cant_claves);
		i++;
	}

	return insertado;
}

/*
 * Busca un elemento que coincida con la clave en la tabla hash.
 *
 * Devuelve el elemento encontrado o NULL si no existe.
 */
elemento_t *buscar_clave(hash_t *h, const char *clave, size_t hash)
{
	bool seguir = true;
	size_t i = 0;
	elemento_t *resultado = NULL;
	while (i < h->capacidad && seguir) {
		size_t pos = (hash + i) % h->capacidad;
		elemento_t *elem = &h->tabla[pos];

		if (elem->clave && !elem->borrado &&
		    strcmp(elem->clave, clave) == 0) {
			resultado = elem;
			seguir = false;
		} else if (!elem->clave)
			seguir = false;

		i++;
	}

	return resultado;
}

void *hash_sacar(hash_t *h, const char *clave)
{
	if (hash_tamanio(h) == 0 || !h->func_hash || !clave)
		return NULL;

	elemento_t *elem = buscar_clave(h, clave, h->func_hash(clave));
	if (elem) {
		void *valor = elem->valor;
		elem->borrado = true;
		h->cant_claves--;
		return valor;
	}

	return NULL;
}

void *hash_buscar(hash_t *h, const char *clave)
{
	if (hash_tamanio(h) == 0 || !h->func_hash || !clave)
		return NULL;

	elemento_t *elem = buscar_clave(h, clave, h->func_hash(clave));

	if (elem)
		return elem->valor;
	return elem;
}

bool hash_existe(hash_t *h, const char *clave)
{
	if (hash_tamanio(h) == 0 || !h->func_hash || !clave)
		return NULL;

	return buscar_clave(h, clave, h->func_hash(clave)) != NULL;
}

size_t hash_tamanio(hash_t *h)
{
	if (!h)
		return 0;

	return h->cant_claves;
}

void hash_destruir(hash_t *h)
{
	hash_destruir_todo(h, NULL);
}

void hash_destruir_todo(hash_t *h, void (*destructor)(void *))
{
	if (!h)
		return;

	for (size_t i = 0; i < h->capacidad; i++) {
		if (h->tabla[i].clave) {
			if (destructor && h->tabla[i].valor)
				destructor(h->tabla[i].valor);
			free((char *)h->tabla[i].clave);
		}
	}
	free(h->tabla);
	free(h);
}

size_t hash_iterar_claves(hash_t *h, bool (*f)(const char *, void *), void *ctx)
{
	if (hash_tamanio(h) == 0 || !h->func_hash || !f)
		return 0;

	bool seguir = true;
	size_t contador = 0;
	size_t i = 0;
	while (i < h->capacidad && seguir) {
		if (h->tabla[i].clave && !h->tabla[i].borrado) {
			seguir = f(h->tabla[i].clave, ctx);
			contador++;
		}
		i++;
	}

	return contador;
}