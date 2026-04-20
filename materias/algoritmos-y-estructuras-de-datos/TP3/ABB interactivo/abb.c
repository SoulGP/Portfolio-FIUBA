#include "abb.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	void **vector;
	size_t capacidad;
	size_t guardados;
} vectorizar_ctx_t;

/**
 * Crea un ABB vacío con el comparador dado.
 * En caso de error devuelve NULL.
 */
abb_t *abb_crear(abb_comparador comparador)
{
    if (!comparador)
        return NULL;

    abb_t *abb = calloc(1, sizeof(abb_t));
    if (!abb)
        return NULL;

    abb->comparador = comparador;

    return abb;
}

/**
 * Devuelve el abb con la nueva hoja insertada o NULL en caso de error.
 */
nodo_abb_t *abb_insertar_nodo(nodo_abb_t *nodo, void *elemento,
                              abb_comparador cmp, bool *insertado)
{
    if (!nodo) {
        nodo = calloc(1, sizeof(nodo_abb_t));
        if (!nodo) {
            *insertado = false;
            return NULL;
        }
        nodo->elemento = (void *)elemento;
        *insertado = true;
        return nodo;
    }

    int comparador = cmp(elemento, nodo->elemento);

    if (comparador <= 0) {
        nodo->izquierda =
            abb_insertar_nodo(nodo->izquierda, elemento, cmp, insertado);
    } else {
        nodo->derecha =
            abb_insertar_nodo(nodo->derecha, elemento, cmp, insertado);
    }

    return nodo;
}

/**
 * Inserta un elemento en el ABB. Admite elementos repetidos.
 * 
 * Devuelve el ABB actualizado o NULL en caso de error.
 */
abb_t *abb_insertar(abb_t *abb, void *elemento)
{
    if (!abb || !abb->comparador)
        return NULL;

    bool insertado = false;
    abb->nodo_raiz = abb_insertar_nodo(abb->nodo_raiz, elemento, abb->comparador, &insertado);

    if (insertado)
        abb->tamanio++;

    return abb;
}

/**
 * Devuelve el elemento si lo encuentra, o NULL si no existe.
 */
void *abb_buscar_nodo(nodo_abb_t *nodo, void *elemento,
                      abb_comparador cmp)
{
    if (!nodo)
        return NULL;

    int comparador = cmp(elemento, nodo->elemento);

    if (comparador == 0) {
        return nodo->elemento;
    } else if (comparador < 0) {
        return abb_buscar_nodo(nodo->izquierda, elemento, cmp);
    } else {
        return abb_buscar_nodo(nodo->derecha, elemento, cmp);
    }
}

/**
 * Busca un elemento en el ABB y lo devuelve.
 * 
 * Devuelve el elemento si existe, NULL si no existe o en caso de error.
 */
void *abb_buscar(abb_t *abb, void *elemento)
{
    if (!abb || !abb->comparador)
        return NULL;

    return abb_buscar_nodo(abb->nodo_raiz, elemento, abb->comparador);
}

/**
 * Libera un nodo hoja
 */
nodo_abb_t *eliminar_hoja(nodo_abb_t *nodo)
{
	free(nodo);
	return NULL;
}

/**
 * Libera un nodo con un solo hijo (izquierdauierdo o derechaecho)
 */
nodo_abb_t *eliminar_con_un_hijo(nodo_abb_t *nodo, nodo_abb_t *hijo)
{
	free(nodo);
	return hijo;
}

/**
 * Reemplaza el nodo a eliminar con el predecesor inorden.
 */
nodo_abb_t *reemplazar_con_predecesor(nodo_abb_t *nodo)
{
	nodo_abb_t *padre_predecesor = nodo;
	nodo_abb_t *predecesor = nodo->izquierda;

	while (predecesor->derecha) {
		padre_predecesor = predecesor;
		predecesor = predecesor->derecha;
	}

	nodo->elemento = predecesor->elemento;

	if (padre_predecesor == nodo) {
		padre_predecesor->izquierda = predecesor->izquierda;
	} else {
		padre_predecesor->derecha = predecesor->izquierda;
	}

	free(predecesor);
	return nodo;
}

/**
 * Borra el nodo del ABB.
 */
nodo_abb_t *abb_buscar_nodo_a_borrar(nodo_abb_t *nodo, void **elemento_borrado)
{
	*elemento_borrado = nodo->elemento;

	if (!nodo->izquierda && !nodo->derecha) {
		return eliminar_hoja(nodo);
	}

	if (nodo->izquierda && !nodo->derecha) {
		return eliminar_con_un_hijo(nodo, nodo->izquierda);
	}

	if (!nodo->izquierda && nodo->derecha) {
		return eliminar_con_un_hijo(nodo, nodo->derecha);
	}

	return reemplazar_con_predecesor(nodo);
}

/**
 * Devuelve el elemento si lo encuentra y lo saca del ABB, o NULL si no existe.
 */
nodo_abb_t *abb_borrar_nodo(nodo_abb_t *nodo, void *elemento,
                            abb_comparador cmp, void **elemento_borrado)
{
    if (!nodo)
        return NULL;

    int comparador = cmp(elemento, nodo->elemento);

    if (comparador < 0) {
        nodo->izquierda = abb_borrar_nodo(nodo->izquierda, elemento, cmp, elemento_borrado);
    } else if (comparador > 0) {
        nodo->derecha = abb_borrar_nodo(nodo->derecha, elemento, cmp, elemento_borrado);
    } else {
        return abb_buscar_nodo_a_borrar(nodo, elemento_borrado);
    }

    return nodo;
}

/**
 * Busca un elemento en el ABB y lo elimina.
 * 
 * Devuelve el elemento eliminado si existe, NULL si no existe o en caso de error.
 */
void *abb_quitar(abb_t *abb, void *elemento)
{
	if (!abb || !abb->comparador)
		return NULL;

	void *elemento_borrado = NULL;
	abb->nodo_raiz = abb_borrar_nodo(abb->nodo_raiz, elemento, abb->comparador,
				    &elemento_borrado);
	
	if(elemento_borrado) 
		abb->tamanio--;
	
	return elemento_borrado;
}

/**
 * Devuelve la cantidad de elementos en el ABB.
 */
size_t abb_tamanio(abb_t *abb)
{
    if (!abb)
        return 0;

    return abb->tamanio;
}

/**
 * Devuelve true si el ABB está vacío, false si no lo está.
 */
bool abb_vacio(abb_t *abb)
{
    return abb_tamanio(abb) == 0;
}

/**
 * Aplica la función `f` a cada elemento y utiliza `ctx` como contexto.
 * 
 * Devuelve la cantidad de elementos recorridos.
 */
size_t abb_recorrer_nodos(nodo_abb_t *nodo, abb_recorrido modo,
			  bool (*f)(void *, void *), void *ctx)
{
	if (!nodo)
		return 0;

	size_t contador = 0;

	if (modo == PREORDEN) {
		if (!f(nodo->elemento, ctx))
			return contador;
		contador++;
	}

	contador += abb_recorrer_nodos(nodo->izquierda, modo, f, ctx);

	if (modo == INORDEN) {
		if (!f(nodo->elemento, ctx))
			return contador;
		contador++;
	}

	contador += abb_recorrer_nodos(nodo->derecha, modo, f, ctx);

	if (modo == POSTORDEN) {
		if (!f(nodo->elemento, ctx))
			return contador;
		contador++;
	}

	return contador;
}

bool guardar_en_vector(void *elemento, void *ctx)
{
	vectorizar_ctx_t *contexto = ctx;

	if (contexto->guardados >= contexto->capacidad)
		return false;

	contexto->vector[contexto->guardados++] = elemento;
	return true;
}
/**
 * Recorre el ABB en el orden indicado y guarda los elementos en un array.
 * 
 * Devuelve la cantidad de elementos guardados.
 */
size_t abb_recorrer(abb_t *abb, abb_recorrido recorrido, void **array, size_t tamanio_array)
{
    if (!abb || !array || tamanio_array == 0)
        return 0;

    vectorizar_ctx_t contexto = { .vector = array, .capacidad = tamanio_array, .guardados = 0 };

    abb_recorrer_nodos(abb->nodo_raiz, recorrido, guardar_en_vector, &contexto);

    return contexto.guardados;
}


/**
 * Libera los nodos del abb, si se proporciona un destructor, lo aplica a cada elemento.
 */
static void abb_destruir_nodos(nodo_abb_t *nodo, void (*destructor)(void *))
{
	if (!nodo)
		return;

	abb_destruir_nodos(nodo->izquierda, destructor);
	abb_destruir_nodos(nodo->derecha, destructor);

	if (destructor && nodo->elemento)
		destructor(nodo->elemento);

	free(nodo);
}

/**
 * Destruye el ABB y libera toda la memoria asignada.
 */
void abb_destruir(abb_t *abb)
{
	if (!abb)
		return;

	abb_destruir_nodos(abb->nodo_raiz, NULL);

	free(abb);
}

/**
 * Destruye el ABB y libera toda la memoria asignada, aplicando la función destructor a cada elemento.
 * 
 * Si el destructor es NULL, no se aplica ninguna función a los elementos.
 */
void abb_destruir_todo(abb_t *abb, void (*destructor)(void *))
{
	if (!abb)
		return;

	abb_destruir_nodos(abb->nodo_raiz, destructor);

	free(abb);
}
