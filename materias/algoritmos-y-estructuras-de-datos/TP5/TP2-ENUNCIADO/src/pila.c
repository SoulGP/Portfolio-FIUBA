#include "lista.h"
#include "pila.h"

/**
 * Crea una pila vacía.
 */
pila_t *pila_crear()
{
	lista_t *pila = lista_crear();

	return (pila_t *)pila;
}

/**
 * Apila un elemento en la pila.
 *
 * Devuelve true si pudo apilar o false si no pudo.
 */
bool pila_apilar(pila_t *pila, void *elemento)
{
	if (!pila)
		return false;

	return lista_insertar_en_posicion((lista_t *)pila, 0, elemento);
}

/**
 * Desapila un elemento de la pila y lo devuelve.
 *
 * Devuelve NULL si no hay elementos en la pila.
 */
void *pila_desapilar(pila_t *pila)
{
	if (!pila || pila_vacia(pila))
		return NULL;

	return lista_sacar_de_posicion((lista_t *)pila, 0);
}

/**
 * Devuelve true si la pila está vacía.
 *
 * Devuelve false si no lo está.
 */
bool pila_vacia(pila_t *pila)
{
	if (!pila)
		return true;

	return (lista_tamanio((lista_t *)pila) == 0);
}

/**
 * Devuelve la cantidad de elementos presentes en la pila.
 *
 * Devuelve 0 si no existe la pila.
 */
size_t pila_tamanio(pila_t *pila)
{
	if (!pila)
		return 0;

	return lista_tamanio((lista_t *)pila);
}

/**
 * Devuelve el elemento en el tope de la pila sin desapilarlo.
 *
 * Devuelve NULL si no hay elementos en la pila.
 */
void *pila_tope(pila_t *pila)
{
	if (!pila || pila_vacia(pila))
		return NULL;

	return lista_obtener_elemento((lista_t *)pila, 0);
}

/**
 * Destruye la pila.
 */
void pila_destruir(pila_t *pila)
{
	if (!pila)
		return;

	lista_destruir((lista_t *)pila);
}
