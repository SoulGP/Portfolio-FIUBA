#include "abb.h"
#include "abb_estructura_privada.h"

typedef struct {
	void **vector;
	size_t capacidad;
	size_t guardados;
} vectorizar_ctx_t;

abb_t *abb_crear(int (*cmp)(const void *, const void *))
{
	if (!cmp)
		return NULL;

	abb_t *abb = calloc(1, sizeof(abb_t));
	if (!abb)
		return NULL;

	abb->comparador = cmp;

	return abb;
}

/**
 * Crea un nodo hoja y lo inicializa.
 * 
 * Devuelve el nodo creado o NULL en caso de error.
 */
nodo_t *crear_nodo(const void *elemento, bool *insertado)
{
	nodo_t *nodo = calloc(1, sizeof(nodo_t));
	if (!nodo) {
		*insertado = false;
		return NULL;
	}
	nodo->elemento = (void *)elemento;
	*insertado = true;
	return nodo;
}

/**
 * Devuelve el abb con el nuevo nodo hoja o NULL en caso de error.
 */
nodo_t *abb_insertar_nodo(nodo_t *nodo, const void *elemento,
			  int (*cmp)(const void *, const void *),
			  bool *insertado)
{
	if (!nodo)
		return crear_nodo(elemento, insertado);

	int comparador = cmp(elemento, nodo->elemento);

	if (comparador <= 0)
		nodo->izq =
			abb_insertar_nodo(nodo->izq, elemento, cmp, insertado);
	else
		nodo->der =
			abb_insertar_nodo(nodo->der, elemento, cmp, insertado);

	return nodo;
}

bool abb_insertar(abb_t *abb, const void *elemento)
{
	if (!abb || !abb->comparador)
		return false;

	bool insertado = false;
	abb->raiz = abb_insertar_nodo(abb->raiz, elemento, abb->comparador,
				      &insertado);

	if (insertado)
		abb->nodos++;

	return insertado;
}

/**
 * Busca un nodo en el ABB.
 * 
 * Devuelve el nodo encontrado o NULL si no existe.
 */
nodo_t *abb_buscar_nodo(nodo_t *nodo, const void *elemento,
			int (*cmp)(const void *, const void *))
{
	if (!nodo)
		return NULL;

	int comparador = cmp(elemento, nodo->elemento);

	if (comparador == 0)
		return nodo;
	else if (comparador < 0)
		return abb_buscar_nodo(nodo->izq, elemento, cmp);
	else
		return abb_buscar_nodo(nodo->der, elemento, cmp);
}

bool abb_existe(const abb_t *abb, const void *elemento)
{
	if (abb_vacio(abb) || !abb->comparador)
		return false;

	return abb_buscar_nodo(abb->raiz, elemento, abb->comparador) != NULL;
}

void *abb_buscar(const abb_t *abb, const void *elemento)
{
	if (abb_vacio(abb) || !abb->comparador)
		return NULL;

	nodo_t *nodo_buscado =
		abb_buscar_nodo(abb->raiz, elemento, abb->comparador);

	if (nodo_buscado)
		return nodo_buscado->elemento;
	else
		return NULL;
}

/**
 * Libera un nodo hoja
 */
nodo_t *eliminar_hoja(nodo_t *nodo)
{
	free(nodo);
	return NULL;
}

/**
 * Libera un nodo con un solo hijo (izquierdo o derecho)
 */
nodo_t *eliminar_con_un_hijo(nodo_t *nodo, nodo_t *hijo)
{
	free(nodo);
	return hijo;
}

/**
 * Busca el predecesor inorden de un nodo y lo devuelve.
 * 
 * Devuelve el predecesor o NULL si no existe.
 */
nodo_t *obtener_predecesor(nodo_t *actual, nodo_t **padre)
{
	if (!actual->der)
		return actual;

	*padre = actual;

	return obtener_predecesor(actual->der, padre);
}

/**
 * Reemplaza el nodo a eliminar con el predecesor inorden.
 */
nodo_t *reemplazar_con_predecesor_inorden(nodo_t *nodo)
{
	nodo_t *padre_predecesor = nodo;
	nodo_t *predecesor = obtener_predecesor(nodo->izq, &padre_predecesor);

	nodo->elemento = predecesor->elemento;

	if (padre_predecesor == nodo)
		padre_predecesor->izq = predecesor->izq;
	else
		padre_predecesor->der = predecesor->izq;

	free(predecesor);
	return nodo;
}

/**
 * Borra el nodo del ABB y guarda el elemento borrado.
 */
nodo_t *abb_borrar_nodo(nodo_t *nodo, void **elem_borrado)
{
	*elem_borrado = nodo->elemento;

	if (!nodo->izq && !nodo->der)
		return eliminar_hoja(nodo);

	if (nodo->izq && !nodo->der)
		return eliminar_con_un_hijo(nodo, nodo->izq);

	if (!nodo->izq && nodo->der)
		return eliminar_con_un_hijo(nodo, nodo->der);

	return reemplazar_con_predecesor_inorden(nodo);
}

/**
 * Busca un nodo en el ABB y lo elimina.
 * 
 * Devuelve el abb actualizado o NULL si no existe.
 */
nodo_t *abb_buscar_nodo_a_sacar(nodo_t *nodo, const void *elemento,
				int (*cmp)(const void *, const void *),
				void **elem_borrado, size_t *tamaño)
{
	if (!nodo)
		return NULL;

	int comparador = cmp(elemento, nodo->elemento);

	if (comparador < 0)
		nodo->izq = abb_buscar_nodo_a_sacar(nodo->izq, elemento, cmp,
						    elem_borrado, tamaño);
	else if (comparador > 0)
		nodo->der = abb_buscar_nodo_a_sacar(nodo->der, elemento, cmp,
						    elem_borrado, tamaño);
	else {
		(*tamaño)--;
		return abb_borrar_nodo(nodo, elem_borrado);
	}

	return nodo;
}

void *abb_sacar(abb_t *abb, const void *elemento)
{
	if (abb_vacio(abb) || !abb->comparador)
		return NULL;

	void *elem_borrado = NULL;
	abb->raiz = abb_buscar_nodo_a_sacar(abb->raiz, elemento,
					    abb->comparador, &elem_borrado,
					    &abb->nodos);

	return elem_borrado;
}

size_t abb_tamanio(const abb_t *abb)
{
	if (!abb)
		return 0;

	return abb->nodos;
}

bool abb_vacio(const abb_t *abb)
{
	return abb_tamanio(abb) == 0;
}

/**
 * Aplica la función `f` a un nodo y suma uno al contador.
 * 
 * Devuelve true si se debe continuar el recorrido, false si no.
 */
bool aplicar_funcion_nodo(const nodo_t *nodo, bool (*f)(void *, void *),
			  void *ctx, bool *continuar, size_t *contador)
{
	(*contador)++;
	if (!f(nodo->elemento, ctx)) {
		*continuar = false;
		return false;
	}
	return true;
}

/**
 * Devuelve la cantidad de elementos recorridos.
 */
size_t abb_recorrer_nodos(const nodo_t *nodo, enum abb_recorrido modo,
			  bool (*f)(void *, void *), void *ctx, bool *continuar)
{
	if (!nodo)
		return 0;

	size_t contador = 0;

	if (modo == ABB_PREORDEN &&
	    !aplicar_funcion_nodo(nodo, f, ctx, continuar, &contador))
		return contador;

	contador += abb_recorrer_nodos(nodo->izq, modo, f, ctx, continuar);
	if (!*continuar)
		return contador;

	if (modo == ABB_INORDEN &&
	    !aplicar_funcion_nodo(nodo, f, ctx, continuar, &contador))
		return contador;

	contador += abb_recorrer_nodos(nodo->der, modo, f, ctx, continuar);
	if (!*continuar)
		return contador;

	if (modo == ABB_POSTORDEN)
		aplicar_funcion_nodo(nodo, f, ctx, continuar, &contador);

	return contador;
}

size_t abb_recorrer(const abb_t *abb, enum abb_recorrido modo,
		    bool (*f)(void *, void *), void *ctx)
{
	if (abb_vacio(abb) || !f || modo < ABB_INORDEN || modo > ABB_POSTORDEN)
		return 0;

	bool continuar = true;
	return abb_recorrer_nodos(abb->raiz, modo, f, ctx, &continuar);
}

/**
 * Guarda el elemento en el vector y aumenta el contador de guardados.
 * 
 * Devuelve true si se pudo guardar, false si no.
 */
bool guardar_en_vector(void *elemento, void *ctx)
{
	vectorizar_ctx_t *contexto = ctx;

	if (contexto->guardados >= contexto->capacidad)
		return false;

	contexto->vector[contexto->guardados++] = elemento;
	return true;
}

size_t abb_vectorizar(const abb_t *abb, enum abb_recorrido modo, void **vector,
		      size_t capacidad)
{
	if (abb_vacio(abb) || !vector || capacidad == 0)
		return 0;

	vectorizar_ctx_t contexto = { .vector = vector,
				      .capacidad = capacidad,
				      .guardados = 0 };

	bool continuar = true;
	abb_recorrer_nodos(abb->raiz, modo, guardar_en_vector, &contexto,
			   &continuar);

	return contexto.guardados;
}

/**
 * Libera los nodos del abb, si se proporciona un destructor, lo aplica a cada elemento.
 */
void abb_destruir_nodos(nodo_t *nodo, void (*destructor)(void *))
{
	if (!nodo)
		return;

	abb_destruir_nodos(nodo->izq, destructor);
	abb_destruir_nodos(nodo->der, destructor);

	if (destructor && nodo->elemento)
		destructor(nodo->elemento);

	free(nodo);
}

void abb_destruir(abb_t *abb)
{
	abb_destruir_todo(abb, NULL);
}

void abb_destruir_todo(abb_t *abb, void (*destructor)(void *))
{
	if (!abb)
		return;

	abb_destruir_nodos(abb->raiz, destructor);

	free(abb);
}
