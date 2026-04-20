#include "lista.h"

typedef struct nodo {
	void *dato;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *inicio;
	nodo_t *final;
	size_t tamanio;
};

struct lista_iterador {
	nodo_t *actual;
};

/** 
 * Crea e inicializa un nuevo nodo.
 * 
 * Devuelve NULL si no se pudo inicializar.
 */
nodo_t *crear_nodo(void *dato)
{
	nodo_t *n = malloc(sizeof(nodo_t));
	if (!n)
		return NULL;

	n->dato = dato;
	n->siguiente = NULL;
	return n;
}

/**
 * Crea una lista vacía.
 *
 */
lista_t *lista_crear()
{
	lista_t *lista = calloc(1, sizeof(lista_t));

	return lista;
}

/**
 * Inserta el elemento al final de la lista.
 *
 * Devuelve true si pudo insertar o false si no pudo
 */
bool lista_insertar(lista_t *lista, void *elemento)
{
	if (!lista)
		return false;

	nodo_t *nuevo = crear_nodo(elemento);
	if (!nuevo)
		return false;

	if (!lista->inicio) {
		lista->inicio = nuevo;
	} else {
		lista->final->siguiente = nuevo;
	}

	lista->final = nuevo;

	lista->tamanio++;
	return true;
}

/**
 * Devuelve el nodo en la posición dada o NULL si no existe.
 */
nodo_t *nodo_en_posicion(lista_t *lista, int posicion)
{
	if (!lista || posicion < 0 || posicion >= lista_tamanio(lista))
		return NULL;

	nodo_t *actual = lista->inicio;
	for (int i = 0; i < posicion; i++)
		actual = actual->siguiente;
	return actual;
}

/**
 * Inserta un elemento en la posición dada en la lista.
 *
 * Si la posición excede al tamaño de la lista, no se inserta nada.
 *
 * Devuelve true si pudo insertar o false si no pudo
 */
bool lista_insertar_en_posicion(lista_t *lista, int posicion, void *elemento)
{
	if (!lista || posicion > lista_tamanio(lista) || posicion < 0)
		return false;

	if (posicion == lista_tamanio(lista))
		return lista_insertar(lista, elemento);

	nodo_t *nuevo = crear_nodo(elemento);
	if (!nuevo)
		return false;

	if (posicion == 0) {
		nuevo->siguiente = lista->inicio;
		lista->inicio = nuevo;
		if (lista->tamanio == 0) {
			lista->final = nuevo;
		}
	} else {
		nodo_t *actual = nodo_en_posicion(lista, posicion);
		nuevo->siguiente = actual->siguiente;
		actual->siguiente = nuevo;
	}

	lista->tamanio++;
	return true;
}

/**
 * Devuelve la cantidad de elementos presentes en la lista.
 *
 * Devuelve 0 si no existe la lista.
 */
size_t lista_tamanio(lista_t *lista)
{
	if (!lista)
		return 0;

	return lista->tamanio;
}

/**
 * Obtiene el elemento en la posición dada o NULL si no existe.
 */
void *lista_obtener_elemento(lista_t *lista, int posicion)
{
	if (!lista || posicion < 0 || posicion >= lista_tamanio(lista))
		return NULL;

	if (posicion == lista_tamanio(lista) - 1)
		return lista->final->dato;

	nodo_t *actual = nodo_en_posicion(lista, posicion);

	return actual->dato;
}

/**
 * Elimina el nodo siguiente al nodo dado (o el primero si anterior es NULL).
 * Devuelve el dato del nodo eliminado.
 */
static void *eliminar_nodo(lista_t *lista, nodo_t *anterior)
{
	nodo_t *a_eliminar;
	void *dato;

	if (!anterior) {
		a_eliminar = lista->inicio;
		lista->inicio = a_eliminar->siguiente;
	} else {
		a_eliminar = anterior->siguiente;
		anterior->siguiente = a_eliminar->siguiente;
	}

	if (a_eliminar == lista->final)
		lista->final = anterior;

	dato = a_eliminar->dato;

	free(a_eliminar);
	lista->tamanio--;

	return dato;
}

/**
 * Saca de la lista el elemento en la posición dada y lo devuelve.
 *
 * Si no existe la posición devuelve null.
 */
void *lista_sacar_de_posicion(lista_t *lista, int posicion)
{
	if (!lista || posicion < 0 || posicion >= lista_tamanio(lista))
		return NULL;

	if (posicion == 0)
		return eliminar_nodo(lista, NULL);

	nodo_t *anterior = nodo_en_posicion(lista, posicion - 1);
	return eliminar_nodo(lista, anterior);
}

/**
 * Saca de la lista el elemento pasado por parámetro y lo devuelve.
 *
 * En caso de no existir dicho elemento, devuelve null.
 */
void *lista_sacar_elemento(lista_t *lista, void *elemento)
{
	if (!lista || lista_tamanio(lista) == 0)
		return NULL;

	int pos = lista_buscar_posicion(lista, elemento);
	if (pos == -1)
		return NULL;

	return lista_sacar_de_posicion(lista, pos);
}

/**
 * Devuelve la posición del elemento en la lista. Si no existe el elemento
 * devuelve -1.
 *
 */
int lista_buscar_posicion(lista_t *lista, void *elemento)
{
	if (!lista || lista_tamanio(lista) == 0)
		return -1;

	int posicion = 0;
	nodo_t *actual = lista->inicio;

	while (actual && actual->dato != elemento) {
		actual = actual->siguiente;
		posicion++;
	}

	if (actual) {
		return posicion;
	} else {
		return -1;
	}
}

/**
 * Busca en la lista el primer elemento que cumple con la función de criterio.
 * La función recibe como primer parámetro el elemento de la lista y como
 * segundo parámetro el contexto. Si la función devuelve true, el elemento
 * cumple el criterio.
 *
 * Devuelve el primer elemento que cumple con el criterio o NULL si
 * no hay ninguno.
 */
void *lista_buscar(lista_t *lista, bool (*criterio)(void *, void *),
		   void *contexto)
{
	if (!lista || !criterio || lista_tamanio(lista) == 0)
		return NULL;

	nodo_t *actual = lista->inicio;
	while (actual) {
		if (criterio(actual->dato, contexto))
			return actual->dato;

		actual = actual->siguiente;
	}

	return NULL;
}

/**
 * Itera los elementos de la lista aplicando la función f a cada elemento de la
 * misma. Contexto se pasa como segundo parámetro de f.
 *
 * Deja de iterar si la función f devuelve false.
 *
 * Devuelve la cantidad de veces que fue invocada f.
 */
int lista_iterar(lista_t *lista, bool (*f)(void *, void *), void *contexto)
{
	if (!lista || !f || lista_tamanio(lista) == 0)
		return 0;

	int iterados = 0;
	bool seguir_iterando = true;
	nodo_t *actual = lista->inicio;
	while (actual && seguir_iterando) {
		seguir_iterando = f(actual->dato, contexto);
		iterados++;

		actual = actual->siguiente;
	}

	return iterados;
}

/**
 * Destruye la lista.
 */
void lista_destruir(lista_t *lista)
{
	if (!lista)
		return;

	nodo_t *actual = lista->inicio;
	while (actual) {
		nodo_t *siguiente = actual->siguiente;
		free(actual);
		actual = siguiente;
	}

	free(lista);
}

/**
 * Crea un iterador para la lista.
 * 
 * Devuelve el iterador o NULL en caso de error.
 */
lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista || lista_tamanio(lista) == 0)
		return NULL;

	lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
	if (!iterador)
		return NULL;

	iterador->actual = lista->inicio;

	return iterador;
}

/**
 * Devuelve true si quedan elementos por recorrer en el iterador.
 * Devuelve false si no quedan elementos por recorrer.
 */
bool lista_iterador_quedan_elementos_por_recorrer(lista_iterador_t *iterador)
{
	return iterador && iterador->actual != NULL;
}

/**
 * Avanza el iterador a la siguiente posición.
 *
 * Si no quedan elementos por recorrer, no hace nada.
 */
void lista_iterador_proxima_iteracion(lista_iterador_t *iterador)
{
	if (iterador && iterador->actual)
		iterador->actual = iterador->actual->siguiente;
}

/**
 * Devuelve el elemento en la posición actual del iterador.
 *
 * Si no quedan elementos por recorrer, devuelve NULL.
 */
void *lista_iterador_obtener_elemento(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->actual)
		return NULL;
	return iterador->actual->dato;
}

/**
 * Destruye el iterador.
 *
 * No destruye la lista.
 */
void lista_iterador_destruir(lista_iterador_t *iterador)
{
	free(iterador);
}