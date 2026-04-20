#include "lista.h"
#include "cola.h"

/**
 * Crea una cola vacía.
 *
 * Devuelve NULL si no se pudo crear la cola.
 */
cola_t *cola_crear()
{
	lista_t *cola = lista_crear();

	return (cola_t *)cola;
}

/**
 * Encola un elemento en la cola.
 *
 * Devuelve true si pudo encolar o false si no pudo.
 */
bool cola_encolar(cola_t *cola, void *elemento)
{
	if (!cola)
		return false;

	return lista_insertar(cola, elemento);
}

/**
 * Desencola un elemento de la cola y lo devuelve.
 *
 * Devuelve NULL si no hay elementos en la cola.
 */
void *cola_desencolar(cola_t *cola)
{
	if (!cola || cola_vacia(cola))
		return NULL;

	return lista_sacar_de_posicion((lista_t *)cola, 0);
}

/**
 * Devuelve true si la cola está vacía.
 *
 * Devuelve false si no lo está.
 */
bool cola_vacia(cola_t *cola)
{
	if (!cola)
		return true;

	return (lista_tamanio((lista_t *)cola) == 0);
}

/**
 * Devuelve la cantidad de elementos presentes en la cola.
 *
 * Devuelve 0 si no existe la cola.
 */
size_t cola_tamanio(cola_t *cola)
{
	if (!cola)
		return 0;

	return lista_tamanio((lista_t *)cola);
}

/**
 * Devuelve el elemento en el frente de la cola sin desencolarlo.
 *
 * Devuelve NULL si no hay elementos en la cola.
 */
void *cola_frente(cola_t *cola)
{
	if (!cola || cola_vacia(cola))
		return NULL;

	return lista_obtener_elemento((lista_t *)cola, 0);
}

/**
 * Destruye la cola.
 */
void cola_destruir(cola_t *cola)
{
	if (!cola)
		return;

	lista_destruir((lista_t *)cola);
}