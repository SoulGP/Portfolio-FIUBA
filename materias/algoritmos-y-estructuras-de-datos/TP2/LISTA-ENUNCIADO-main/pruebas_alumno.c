#include <string.h>
#include "pa2m.h"
#include "src/lista.h"
#include "src/cola.h"
#include "src/pila.h"

void no_se_puede_insertar_porque_la_lista_no_existe()
{
	lista_t *l = NULL;
	int uno = 1;

	bool se_pudo_insertar = lista_insertar(l, &uno);
	pa2m_afirmar(
		se_pudo_insertar == false,
		"No se pueden insertar elementos porque la lista no existe");

	se_pudo_insertar = lista_insertar_en_posicion(l, 0, &uno);
	pa2m_afirmar(
		se_pudo_insertar == false,
		"No se pueden insertar elementos en x posición porque la lista no existe");
}

void no_se_puede_sacar_un_elemento_porque_la_lista_no_existe()
{
	lista_t *l = NULL;
	int uno = 1;

	int *elemento = lista_sacar_elemento(l, &uno);
	pa2m_afirmar(elemento == NULL,
		     "No se puede sacar un elemento porque la lista no existe");

	elemento = lista_sacar_de_posicion(l, 0);
	pa2m_afirmar(
		elemento == NULL,
		"No se puede sacar un elemento por su posición porque la lista no existe");
}

void no_se_puede_obtener_un_elemento_porque_la_lista_no_existe()
{
	lista_t *l = NULL;

	int *elemento = lista_obtener_elemento(l, 0);
	pa2m_afirmar(
		elemento == NULL,
		"No se puede obtener un elemento por su posición porque la lista no existe");
}

void no_se_puede_buscar_la_posicion_de_un_elemento_porque_la_lista_no_existe()
{
	lista_t *l = NULL;
	int uno = 1;

	int posicion = lista_buscar_posicion(l, &uno);
	pa2m_afirmar(
		posicion == -1,
		"No se puede buscar la posicion de un elemento porque la lista no existe");
}

bool es_igual_a_contexto(void *elemento, void *contexto)
{
	if (!elemento || !contexto)
		return false;

	int *valor = elemento;
	int *objetivo = contexto;

	return *valor == *objetivo;
}

void no_se_puede_buscar_un_elemento_porque_la_lista_no_existe()
{
	lista_t *l = NULL;
	int uno = 1;

	int *resultado = lista_buscar(l, es_igual_a_contexto, &uno);
	pa2m_afirmar(
		resultado == NULL,
		"No se puede buscar un elemento porque la lista no existe");
}

bool contar_elementos(void *elemento, void *contexto)
{
	return true;
}

void no_se_puede_iterar_porque_la_lista_no_existe()
{
	lista_t *l = NULL;

	int posicion = 0;
	int cantidad_iterados = lista_iterar(l, contar_elementos, &posicion);
	pa2m_afirmar(cantidad_iterados == 0,
		     "No se puede iterar la lista porque no existe");
}

void lista_tamanio_devuelve_la_cantidad_de_elementos_en_una_lista_correctamente()
{
	int uno = 1;
	int dos = 2;
	int tres = 3;

	lista_t *l = NULL;
	size_t n = lista_tamanio(l);
	pa2m_afirmar(
		n == 0,
		"No existe la lista, lista_tamanio devuelve 0 (valor obtenido: %i)",
		n);

	l = lista_crear();
	n = lista_tamanio(l);
	pa2m_afirmar(
		n == 0,
		"No hay elementos en la lista, lista_tamanio devuelve 0 (valor obtenido: %i)",
		n);

	lista_insertar(l, &uno);
	lista_insertar(l, &dos);
	lista_insertar(l, &tres);
	lista_insertar(l, &tres);
	lista_insertar(l, &tres);
	lista_insertar(l, &tres);

	n = lista_tamanio(l);
	pa2m_afirmar(
		n == 6,
		"Hay elementos en la lista, lista_tamanio devuelve 6 (valor obtenido: %i)",
		n);

	lista_destruir(l);
}

void lista_insertar_verificando_que_los_elementos_se_inserten_correctamente()
{
	lista_t *l = lista_crear();
	int uno = 1;
	int dos = 2;
	int tres = 3;

	lista_insertar(l, &uno);
	lista_insertar(l, &dos);
	lista_insertar(l, &tres);

	int *n = lista_obtener_elemento(l, 2);
	pa2m_afirmar(
		*n == 3,
		"lista_insertar inserta los elementos correctamente (último elemento de la lista == 3, valor obtenido: %i)",
		*n);
	n = lista_obtener_elemento(l, 3);
	pa2m_afirmar(
		n == NULL,
		"Obtener un elemento más allá de la lista devuevle NULL, valor obtenido: %i)",
		n);

	lista_destruir(l);
}

void lista_insertar_en_posicion_invalida_devuelve_NULL()
{
	int uno = 1;
	lista_t *l = lista_crear();

	lista_insertar_en_posicion(l, 40, &uno);

	int *n = lista_obtener_elemento(l, 0);
	pa2m_afirmar(
		n == NULL,
		"En una lista vacía, lista_insertar_en_posicion en una posición inválida no inserta el elemento y al intentar obtener un elemento devuelve NULL (valor obtenido: %i)",
		n);

	lista_destruir(l);
}

void lista_insertar_en_posicion_verificando_que_los_elementos_se_inserten_correctamente()
{
	int uno = 1;
	int dos = 2;
	int tres = 3;
	lista_t *l = lista_crear();

	lista_insertar(l, &uno);
	lista_insertar(l, &dos);
	lista_insertar_en_posicion(l, 1, &tres);

	int *n = lista_obtener_elemento(l, 1);
	pa2m_afirmar(
		*n == 3,
		"lista_insertar_en_posicion en la posicion 1 se inserta correctamente (elemento en la posicion 1 de la lista == 3, valor obtenido: %i)",
		*n);
	n = lista_obtener_elemento(l, 2);
	pa2m_afirmar(
		*n == 2,
		"Tras insertar un elemento en x posicion el antiguo elemento se desplaza correctamente (elemento en la posicion siguiente de la lista == 2, valor obtenido: %i)",
		*n);

	lista_destruir(l);
}

void lista_sacar_de_posicion_devuelve_el_elemento_correcto()
{
	int uno = 1;
	int dos = 2;
	int tres = 3;
	lista_t *l = lista_crear();

	lista_insertar(l, &uno);
	lista_insertar(l, &dos);
	lista_insertar(l, &tres);

	int *n = lista_sacar_de_posicion(l, 1);
	pa2m_afirmar(
		*n == 2,
		"lista_sacar_de_posicion de x posicion devuelve el elemento correcto (elemento == 2, valor obtenido: %i)",
		*n);
	n = lista_obtener_elemento(l, 1);
	pa2m_afirmar(
		*n == 3,
		"Tras sacar un elemento de la lista de x posicion se puede acceder al siguiente elemento (elemento == 3, valor obtenido: %i)",
		*n);

	size_t m = lista_tamanio(l);
	pa2m_afirmar(
		m == 2,
		"Tras sacar un elemento de la lista de x posicion se reduce correctamente el tamaño de la lista (tamanio == 2, valor obtenido: %i)",
		m);

	lista_destruir(l);
}

void lista_sacar_de_posicion_invalida_devuelve_NULL()
{
	int uno = 1;
	lista_t *l = lista_crear();

	lista_insertar(l, &uno);

	int *n = lista_sacar_de_posicion(l, 27);
	pa2m_afirmar(
		n == NULL,
		"lista_sacar_de_posicion de posición inválida devuelve NULL (valor obtenido: %i)",
		n);
	size_t m = lista_tamanio(l);
	pa2m_afirmar(
		m == 1,
		"Tras no poder sacar un elemento de la lista de x posicion se mantiene el tamaño de la lista (tamanio == 1, valor obtenido: %i)",
		m);

	lista_destruir(l);
}

void lista_sacar_elemento_devuelve_el_elemento_correcto_y_mantiene_el_orden_de_la_lista()
{
	int uno = 1;
	int dos = 2;
	int cinco = 5;
	lista_t *l = lista_crear();

	lista_insertar(l, &uno);
	lista_insertar(l, &dos);
	lista_insertar(l, &cinco);

	int *n = lista_sacar_elemento(l, &uno);
	pa2m_afirmar(
		*n == 1,
		"lista_sacar_elemento de x posicion devuelve el elemento correcto (elemento == 1, valor obtenido: %i)",
		*n);
	n = lista_obtener_elemento(l, 1);
	pa2m_afirmar(
		*n == 5,
		"Tras sacar un elemento de la lista de x posicion se puede acceder al siguiente elemento (elemento == 5, valor obtenido: %i)",
		*n);
	size_t m = lista_tamanio(l);
	pa2m_afirmar(
		m == 2,
		"Tras sacar un elemento de la lista de x posicion se reduce correctamente el tamaño de la lista (tamanio == 2, valor obtenido: %i)",
		m);

	lista_destruir(l);
}

void lista_sacar_elemento_invalida_devuelve_NULL()
{
	int uno = 1;
	int cuarenta = 40;
	lista_t *l = lista_crear();

	lista_insertar(l, &uno);

	int *n = lista_sacar_elemento(l, &cuarenta);
	pa2m_afirmar(
		n == NULL,
		"lista_sacar_elemento de posición inválida devuelve NULL (valor obtenido: %i)",
		n);
	size_t m = lista_tamanio(l);
	pa2m_afirmar(
		m == 1,
		"Tras no poder sacar un elemento de la lista de x posicion se mantiene el tamaño de la lista (tamanio == 1, valor obtenido: %i)",
		m);

	lista_destruir(l);
}

void lista_buscar_posicion_devuelve_la_posición_correcta()
{
	int uno = 1;
	int dos = 2;
	int tres = 3;
	int cincuenti_cinco = 55;
	int nueve = 9;
	lista_t *l = lista_crear();

	lista_insertar(l, &uno);
	lista_insertar(l, &dos);
	lista_insertar(l, &tres);
	lista_insertar(l, &tres);
	lista_insertar(l, &tres);
	lista_insertar(l, &cincuenti_cinco);
	lista_insertar(l, &tres);
	lista_insertar(l, &cincuenti_cinco);
	lista_insertar(l, &cincuenti_cinco);
	lista_insertar(l, &cincuenti_cinco);

	int n = lista_buscar_posicion(l, &nueve);
	pa2m_afirmar(
		n == -1,
		"lista_buscar_posicion devuelve -1 cuando no encuentra un elemento (valor obtenido: %i)",
		n);
	n = lista_buscar_posicion(l, &cincuenti_cinco);
	pa2m_afirmar(
		n == 5,
		"lista_buscar_posicion encuentra un elemento en la posición 5 (valor obtenido: %i)",
		n);

	lista_destruir(l);
}

void lista_buscar_posicion_devuelve_la_posición_correcta_de_un_elemento_nulo()
{
	int uno = 1;
	void *null = NULL;
	lista_t *l = lista_crear();

	lista_insertar(l, &uno);
	lista_insertar(l, null);

	int n = lista_buscar_posicion(l, null);
	pa2m_afirmar(
		n == 1,
		"lista_buscar_posicion devuelve 1 al buscar un elemento nulo (valor obtenido: %i)",
		n);

	void *elemento = lista_sacar_elemento(l, null);
	pa2m_afirmar(
		elemento == NULL,
		"lista_sacar_elemento devuelve NULL al sacar un elemento NULL");

	size_t tamanio = lista_tamanio(l);
	pa2m_afirmar(tamanio == 1,
		     "lista_tamanio devuelve 1 (valor obtenido: %i)", tamanio);

	lista_destruir(l);
}

void lista_buscar_devuelve_el_elemento_con_x_función_e_y_criterio()
{
	int uno = 1;
	int dos = 2;
	int tres = 3;
	int cincuenti_cinco = 55;
	int nueve = 9;
	void *null = NULL;
	lista_t *l = lista_crear();

	lista_insertar(l, &uno);
	lista_insertar(l, &dos);
	lista_insertar(l, &tres);
	lista_insertar(l, &tres);
	lista_insertar(l, &tres);
	lista_insertar(l, &cincuenti_cinco);
	lista_insertar(l, &tres);
	lista_insertar(l, &cincuenti_cinco);
	lista_insertar(l, null);
	lista_insertar(l, &cincuenti_cinco);
	lista_insertar(l, &cincuenti_cinco);

	int objetivo = cincuenti_cinco;
	int *resultado = lista_buscar(l, es_igual_a_contexto, &objetivo);
	pa2m_afirmar(
		*resultado == 55,
		"lista_buscar con x funcion e y criterio devuelve 55 (valor obtenido: %i)",
		*resultado);

	void *o_null = null;
	void *r_null = lista_buscar(l, es_igual_a_contexto, o_null);
	pa2m_afirmar(
		r_null == NULL,
		"lista_buscar con x funcion e y criterio devuelve NULL (valor obtenido: %i)",
		r_null);

	objetivo = nueve;
	resultado = lista_buscar(l, es_igual_a_contexto, &objetivo);
	pa2m_afirmar(
		resultado == NULL,
		"lista_buscar con x funcion e y criterio no encuentra algún elemento que cumpla con el criterio (valor obtenido: %i)",
		resultado);

	lista_destruir(l);
}

bool contar_primer_elemento(void *elemento, void *contexto)
{
	int *posicion = contexto;
	if (*posicion == 2) {
		return false;
	}
	(*posicion)++;

	return true;
}

void lista_iterar_devuelve_la_cantidad_de_elementos_iterados_correcta()
{
	int uno = 1;
	int dos = 2;
	int tres = 3;
	int cincuenti_cinco = 55;
	lista_t *l = lista_crear();

	lista_insertar(l, &uno);
	lista_insertar(l, &dos);
	lista_insertar(l, &tres);
	lista_insertar(l, &cincuenti_cinco);
	lista_insertar(l, &tres);
	lista_insertar(l, &cincuenti_cinco);
	lista_insertar(l, &cincuenti_cinco);

	int posicion = 0;
	int cantidad_iterados =
		lista_iterar(l, contar_primer_elemento, &posicion);
	pa2m_afirmar(
		cantidad_iterados == 3,
		"lista_iterar con x funcion e y contexto devuelve 3 iterados (valor obtenido: %i)",
		cantidad_iterados);

	cantidad_iterados = lista_iterar(l, contar_elementos, NULL);
	pa2m_afirmar(cantidad_iterados == 7,
		     "lista_iterar devuelve 7 iterados (valor obtenido: %i)",
		     cantidad_iterados);

	cantidad_iterados = lista_iterar(l, NULL, NULL);
	pa2m_afirmar(
		cantidad_iterados == 0,
		"lista_iterar sin una funcion devuelve 0 iterado (valor obtenido: %i)",
		cantidad_iterados);

	lista_destruir(l);
}

void lista_iterador_se_crea_y_funciona_correctamente()
{
	int uno = 1;
	int dos = 2;
	int tres = 3;
	lista_t *l = lista_crear();

	lista_iterador_t *li = lista_iterador_crear(l);
	bool quedan_elementos_por_recorrer =
		lista_iterador_quedan_elementos_por_recorrer(li);

	pa2m_afirmar(
		quedan_elementos_por_recorrer == false,
		"lista_iterador_quedan_elementos_por_recorrer devuelve false porque no hay elementos en la lista (valor obtenido)");

	int *n = lista_iterador_obtener_elemento(li);
	pa2m_afirmar(
		n == NULL,
		"lista_iterador_obtener_elemento en una lista vacía devuelve 0 (valor_obtenido: %i)",
		n);

	lista_iterador_destruir(li);

	lista_insertar(l, &uno);
	lista_insertar(l, &dos);
	lista_insertar(l, &tres);
	int posicion = 0;
	for (li = lista_iterador_crear(l);
	     lista_iterador_quedan_elementos_por_recorrer(li);
	     lista_iterador_proxima_iteracion(li)) {
		int *n = lista_iterador_obtener_elemento(li);
		if (posicion == 2) {
			pa2m_afirmar(
				*n == 3,
				"lista_iterador_obtener_elemento devuelve 3 (valor_obtenido: %i)",
				*n);
		}
		posicion++;
	}

	lista_iterador_destruir(li);
	lista_destruir(l);
}

void testeo_de_uso_promedio_de_pila()
{
	int uno = 1;
	int dos = 2;
	int tres = 3;
	int cuatro = 4;
	int cinco = 5;
	pila_t *p = pila_crear();

	pa2m_afirmar(pila_vacia(p) == true,
		     "Tras crear la pila, pila_vacia devuelve true");

	pila_apilar(p, &uno);
	pila_apilar(p, &dos);
	pila_apilar(p, &tres);
	pila_apilar(p, &cuatro);
	pila_apilar(p, &cinco);

	pa2m_afirmar(pila_tamanio(p) == 5, "pila_tamanio devuelve 5");

	pa2m_afirmar(
		pila_vacia(p) == false,
		"Tras apilar elementos en la pila, pila_vacia devuelve false");

	int *i = pila_desapilar(p);
	pa2m_afirmar(*i == 5, "pila_desapilar mantiene la política LIFO");
	i = pila_desapilar(p);
	pa2m_afirmar(*i == 4, "pila_desapilar mantiene la política LIFO");
	i = pila_desapilar(p);
	pa2m_afirmar(*i == 3, "pila_desapilar mantiene la política LIFO");

	pa2m_afirmar(pila_tamanio(p) == 2, "pila_tamanio devuelve 2");

	*i = 10;
	pa2m_afirmar(
		pila_apilar(p, i) == true,
		"Tras desapilar elementos en la pila, se puede seguir apilando elementos");

	i = pila_tope(p);
	pa2m_afirmar(
		*i == 10,
		"pila_tope devuelve el ultimo elemento apilado (valor obtenido: %i)",
		*i);

	pila_destruir(p);
}

void testeo_de_uso_promedio_de_cola()
{
	int uno = 1;
	int dos = 2;
	int tres = 3;
	int cuatro = 4;
	int cinco = 5;
	cola_t *c = cola_crear();

	cola_encolar(c, &uno);
	cola_encolar(c, &dos);
	cola_encolar(c, &tres);
	cola_encolar(c, &cuatro);
	cola_encolar(c, &cinco);

	pa2m_afirmar(cola_tamanio(c) == 5, "cola_tamanio devuelve 5");

	pa2m_afirmar(
		cola_vacia(c) == false,
		"Tras encolar elementos en la cola, cola_vacia devuelve false");

	int *i = cola_desencolar(c);
	pa2m_afirmar(*i == 1, "cola_desencolar mantiene la política FIFO");
	i = cola_desencolar(c);
	pa2m_afirmar(*i == 2, "cola_desencolar mantiene la política FIFO");
	i = cola_desencolar(c);
	pa2m_afirmar(*i == 3, "cola_desencolar mantiene la política FIFO");

	pa2m_afirmar(cola_tamanio(c) == 2, "cola_tamanio devuelve 2");

	*i = 10;
	pa2m_afirmar(
		cola_encolar(c, i) == true,
		"Tras desencolar elementos en la cola, se puede seguir encolando elementos");

	i = cola_frente(c);
	pa2m_afirmar(
		*i == 4,
		"cola_frente devuelve el primer elemento encolado (valor obtenido: %i)",
		*i);

	cola_destruir(c);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas lista nula");
	no_se_puede_insertar_porque_la_lista_no_existe();
	no_se_puede_sacar_un_elemento_porque_la_lista_no_existe();
	no_se_puede_obtener_un_elemento_porque_la_lista_no_existe();
	no_se_puede_buscar_la_posicion_de_un_elemento_porque_la_lista_no_existe();
	no_se_puede_buscar_un_elemento_porque_la_lista_no_existe();
	no_se_puede_iterar_porque_la_lista_no_existe();

	pa2m_nuevo_grupo("Pruebas lista con elementos");
	lista_insertar_verificando_que_los_elementos_se_inserten_correctamente();
	lista_insertar_en_posicion_verificando_que_los_elementos_se_inserten_correctamente();
	lista_insertar_en_posicion_invalida_devuelve_NULL();
	lista_tamanio_devuelve_la_cantidad_de_elementos_en_una_lista_correctamente();
	lista_buscar_posicion_devuelve_la_posición_correcta();
	lista_buscar_devuelve_el_elemento_con_x_función_e_y_criterio();
	lista_iterar_devuelve_la_cantidad_de_elementos_iterados_correcta();
	lista_buscar_posicion_devuelve_la_posición_correcta_de_un_elemento_nulo();

	pa2m_nuevo_grupo("Pruebas lista_sacar_de_posicion");
	lista_sacar_de_posicion_devuelve_el_elemento_correcto();
	lista_sacar_de_posicion_invalida_devuelve_NULL();

	pa2m_nuevo_grupo("Pruebas lista_sacar_elemento");
	lista_sacar_elemento_devuelve_el_elemento_correcto_y_mantiene_el_orden_de_la_lista();
	lista_sacar_elemento_invalida_devuelve_NULL();

	pa2m_nuevo_grupo("Pruebas lista_iterador");
	lista_iterador_se_crea_y_funciona_correctamente();

	pa2m_nuevo_grupo("Pruebas pila");
	testeo_de_uso_promedio_de_pila();

	pa2m_nuevo_grupo("Pruebas cola");
	testeo_de_uso_promedio_de_cola();

	return pa2m_mostrar_reporte();
}
