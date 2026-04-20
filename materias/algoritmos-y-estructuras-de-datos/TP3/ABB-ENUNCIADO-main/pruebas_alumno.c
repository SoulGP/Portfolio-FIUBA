#include "pa2m.h"
#include "src/abb.h"

int comparar_enteros(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}
int comparar_null(const void *a, const void *b)
{
	if (a == NULL && b == NULL)
		return 0;
	if (a == NULL)
		return -1;
	if (b == NULL)
		return 1;
	return 0;
}

void abb_crear_inicializa_y_destruye_correctamente_el_abb()
{
	abb_t *abb = abb_crear(comparar_enteros);
	pa2m_afirmar(abb != NULL, "Se puede crear un ABB.");

	abb_destruir(abb);
	pa2m_afirmar(true, "Se destruye el ABB.");
}

void abb_destruir_todo_libera_elementos_correctamente()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int *uno = malloc(sizeof(int));
	*uno = 1;
	abb_insertar(abb, uno);
	abb_destruir_todo(abb, free);
	pa2m_afirmar(true, "Se destruye el ABB y se liberan los elementos.");
}

void abb_insertar_inserta_un_elemento_en_el_abb_correctamente()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int uno = 1;
	pa2m_afirmar(abb_insertar(abb, &uno),
		     "Se insertan elementos en el ABB.");

	abb_destruir(abb);
}

void abb_insertar_inserta_un_elemento_NULL_en_el_abb_correctamente()
{
	abb_t *abb = abb_crear(comparar_null);

	void *null = NULL;
	pa2m_afirmar(abb_insertar(abb, null),
		     "Se inserta el elemento \"NULL\" en el ABB.");

	abb_destruir(abb);
}

void abb_existe_encuentra_un_elemento_insertado_previamente()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int uno = 1;
	abb_insertar(abb, &uno);
	pa2m_afirmar(abb_existe(abb, &uno),
		     "Existe el elemento \"1\" en el ABB.");

	abb_destruir(abb);
}

void abb_existe_encuentra_un_elemento_NULL_insertado_previamente()
{
	abb_t *abb = abb_crear(comparar_null);
	int *n = NULL;

	abb_insertar(abb, n);
	pa2m_afirmar(abb_existe(abb, NULL),
		     "existe el elemento \"NULL\" en el ABB.");

	abb_destruir(abb);
}

void abb_existe_no_encuentra_elemento_porque_no_ha_sido_insertado()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int siete = 7;
	pa2m_afirmar(abb_existe(abb, &siete) == false,
		     "No existe el elemento \"7\" en el ABB.");

	abb_destruir(abb);
}

void abb_buscar_busca_un_elemento_insertado_previamente_y_lo_devuelve()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int dos = 2;
	abb_insertar(abb, &dos);
	int *n = abb_buscar(abb, &dos);
	pa2m_afirmar(
		*n == 2,
		"Se busca el elemento \"2\", se encuentra y se devuelve correctamente (valor obtenido: %i).",
		*n);

	abb_destruir(abb);
}

void abb_buscar_busca_un_elemento_no_insertado_devuelve_NULL()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int uno = 1;
	int *n = abb_buscar(abb, &uno);
	pa2m_afirmar(n == NULL,
		     "Al buscar un elemento no insertado, se devuelve NULL.");

	abb_destruir(abb);
}

void abb_buscar_busca_un_elemento_null_y_lo_devuelve()
{
	abb_t *abb = abb_crear(comparar_enteros);

	void *null = NULL;
	void *n = abb_buscar(abb, null);
	pa2m_afirmar(n == NULL,
		     "Al buscar un elemento NULL, se devuelve NULL.");

	abb_destruir(abb);
}

void abb_tamanio_de_un_abb_recien_creado_devuelve_0()
{
	abb_t *abb = abb_crear(comparar_enteros);

	size_t tamanio = abb_tamanio(abb);
	pa2m_afirmar(tamanio == 0,
		     "El tamaño del ABB es igual a \"0\" (valor obtenido: %i).",
		     tamanio);

	abb_destruir(abb);
}

void abb_tamanio_devuelve_la_cantidad_correcta_de_elementos_guardados_en_el_abb()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int tres = 3;
	abb_insertar(abb, &tres);
	abb_insertar(abb, &tres);
	abb_insertar(abb, &tres);
	size_t tamanio = abb_tamanio(abb);
	pa2m_afirmar(tamanio == 3,
		     "El tamaño del ABB es igual a \"3\" (valor obtenido: %i).",
		     tamanio);

	abb_destruir(abb);
}

void abb_vacio_devuelve_true_al_no_haber_elementos_en_el_abb()
{
	abb_t *abb = abb_crear(comparar_enteros);

	pa2m_afirmar(abb_vacio(abb) == true, "El ABB se encuentra vacio");

	abb_destruir(abb);
}

void abb_vacio_devuelve_false_al_haber_elementos_en_el_abb()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int uno = 1;
	abb_insertar(abb, &uno);
	pa2m_afirmar(abb_vacio(abb) == false, "El ABB no se encuentra vacio");

	abb_destruir(abb);
}

void abb_sacar_elimina_un_elemento_y_lo_devuelve()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int cuatro = 4;
	abb_insertar(abb, &cuatro);
	int *n = abb_sacar(abb, &cuatro);
	pa2m_afirmar(
		*n == 4,
		"Se elimina el elemento \"4\" y se devuelve correctamente (valor obtenido: %i).",
		*n);

	pa2m_afirmar(
		abb_existe(abb, &cuatro) == false,
		"tras eliminar el elemento \"4\" del ABB al buscarlo no existe.");

	pa2m_afirmar(abb_vacio(abb) == true,
		     "Se reduce correctamente el tamaño del ABB");

	abb_destruir(abb);
}

void abb_eliminar_elemento_inexistente()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int elemento = 42;
	int inexistente = 99;

	abb_insertar(abb, &elemento);

	int *eliminado = abb_sacar(abb, &inexistente);
	pa2m_afirmar(
		eliminado == NULL,
		"Intentar eliminar un elemento inexistente devuelve NULL.");
	pa2m_afirmar(
		!abb_vacio(abb),
		"El ABB no está vacío después de intentar eliminar un elemento inexistente.");

	abb_destruir(abb);
}

bool sumar_elementos(void *elemento, void *ctx)
{
	int *suma = ctx;
	*suma += *(int *)elemento;
	return true;
}
void abb_recorrer_aplica_funcion_a_cada_elemento_en_el_orden_INORDERN()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int elementos[] = { 10, 5, 15, 3, 7, 12, 18 };
	for (int i = 0; i < 7; i++)
		abb_insertar(abb, &elementos[i]);

	int suma = 0;
	size_t recorridos =
		abb_recorrer(abb, ABB_INORDEN, sumar_elementos, &suma);
	pa2m_afirmar(recorridos == 7,
		     "Se recorrieron los 7 elementos del ABB en inorden.");
	pa2m_afirmar(suma == 70,
		     "La suma de los elementos recorridos es correcta (70).");

	abb_destruir(abb);
}
void abb_recorrer_aplica_funcion_a_cada_elemento_en_el_orden_POSTORDEN()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int elementos[] = { 10, 51, 152, 3, 7, 12, 18 };
	for (int i = 0; i < 7; i++)
		abb_insertar(abb, &elementos[i]);

	int suma = 0;
	size_t recorridos =
		abb_recorrer(abb, ABB_POSTORDEN, sumar_elementos, &suma);
	pa2m_afirmar(recorridos == 7,
		     "Se recorrieron los 7 elementos del ABB en postorden.");
	pa2m_afirmar(suma == 253,
		     "La suma de los elementos recorridos es correcta (253).");

	abb_destruir(abb);
}
void abb_recorrer_aplica_funcion_a_cada_elemento_en_el_orden_PREORDEN()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int elementos[] = { 10, 52, 15, 3, 71, 12, 18 };
	for (int i = 0; i < 7; i++)
		abb_insertar(abb, &elementos[i]);

	int suma = 0;
	size_t recorridos =
		abb_recorrer(abb, ABB_PREORDEN, sumar_elementos, &suma);
	pa2m_afirmar(recorridos == 7,
		     "Se recorrieron los 7 elementos del ABB en preorden.");
	pa2m_afirmar(suma == 181,
		     "La suma de los elementos recorridos es correcta (181).");

	abb_destruir(abb);
}

bool restar_elementos_hasta_encontrar_un_15(void *elemento, void *ctx)
{
	if (*(int *)elemento == 15)
		return false;

	int *resta = ctx;
	*resta -= *(int *)elemento;
	return true;
}
void abb_recorrer_aplica_funcion_hasta_devolver_false_en_el_orden_INORDERN()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int elementos[] = { 10, 5, 15, 3, 7, 12, 18 };
	for (int i = 0; i < 7; i++)
		abb_insertar(abb, &elementos[i]);

	int resta = 0;
	size_t recorridos = abb_recorrer(abb, ABB_INORDEN,
					 restar_elementos_hasta_encontrar_un_15,
					 &resta);
	pa2m_afirmar(
		recorridos == 6,
		"Se recorrieron los 6 elementos del ABB en inorden. (valor obtenido: %i)",
		recorridos);
	pa2m_afirmar(
		resta == -37,
		"La resta de los elementos recorridos es correcta (-37). (valor obtenido: %i)",
		resta);

	abb_destruir(abb);
}
void abb_recorrer_aplica_funcion_hasta_devolver_false_en_el_orden_POSTORDEN()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int elementos[] = { 10, 51, 15, 0, 7, 12, 18 };
	for (int i = 0; i < 7; i++)
		abb_insertar(abb, &elementos[i]);

	int resta = 0;
	size_t recorridos = abb_recorrer(abb, ABB_POSTORDEN,
					 restar_elementos_hasta_encontrar_un_15,
					 &resta);
	pa2m_afirmar(
		recorridos == 5,
		"Se recorrieron los 5 elementos del ABB en postorden. (valor obtenido: %i)",
		recorridos);
	pa2m_afirmar(
		resta == -37,
		"La resta de los elementos recorridos es correcta (-37). (valor obtenido: %i)",
		resta);

	abb_destruir(abb);
}
void abb_recorrer_aplica_funcion_hasta_devolver_false_en_el_orden_PREORDEN()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int elementos[] = { 102, 52, 15, 3, 71, 12, 18 };
	for (int i = 0; i < 7; i++)
		abb_insertar(abb, &elementos[i]);

	int resta = 0;
	size_t recorridos = abb_recorrer(abb, ABB_PREORDEN,
					 restar_elementos_hasta_encontrar_un_15,
					 &resta);
	pa2m_afirmar(
		recorridos == 3,
		"Se recorrieron los 3 elementos del ABB en preorden. (valor obtenido: %i)",
		recorridos);
	pa2m_afirmar(
		resta == -154,
		"La resta de los elementos recorridos es correcta (-154). (valor obtenido: %i)",
		resta);

	abb_destruir(abb);
}

void abb_vectorizar_guarda_elementos_en_el_vector_en_el_orden_INORDERN()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int elementos[] = { 10, 5, 15, 3, 7, 12, 18 };
	for (int i = 0; i < 7; i++)
		abb_insertar(abb, &elementos[i]);

	void *vector[7];
	size_t guardados = abb_vectorizar(abb, ABB_INORDEN, vector, 7);

	pa2m_afirmar(
		guardados == 7,
		"Se guardaron los 7 elementos en el vector (modo: INORDEN).");
	pa2m_afirmar(*(int *)vector[0] == 3,
		     "El primer elemento del vector es correcto (3).");
	pa2m_afirmar(*(int *)vector[1] == 5,
		     "El segundo elemento del vector es correcto (5).");
	pa2m_afirmar(*(int *)vector[2] == 7,
		     "El tercer elemento del vector es correcto (7).");
	pa2m_afirmar(*(int *)vector[3] == 10,
		     "El cuarto elemento del vector es correcto (10).");
	pa2m_afirmar(*(int *)vector[4] == 12,
		     "El quinto elemento del vector es correcto (12).");
	pa2m_afirmar(*(int *)vector[5] == 15,
		     "El sexto elemento del vector es correcto (15).");
	pa2m_afirmar(*(int *)vector[6] == 18,
		     "El séptimo elemento del vector es correcto (18).");

	abb_destruir(abb);
}

void abb_vectorizar_guarda_elementos_en_el_vector_en_el_orden_PREORDEN()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int elementos[] = { 10, 5, 15, 3, 7, 12, 18 };
	for (int i = 0; i < 7; i++)
		abb_insertar(abb, &elementos[i]);

	void *vector[7];
	size_t guardados = abb_vectorizar(abb, ABB_PREORDEN, vector, 7);

	pa2m_afirmar(
		guardados == 7,
		"Se guardaron los 7 elementos en el vector (modo: PREORDEN).");
	pa2m_afirmar(*(int *)vector[0] == 10,
		     "El primer elemento del vector es correcto (10).");
	pa2m_afirmar(*(int *)vector[1] == 5,
		     "El segundo elemento del vector es correcto (5).");
	pa2m_afirmar(*(int *)vector[2] == 3,
		     "El tercer elemento del vector es correcto (3).");
	pa2m_afirmar(*(int *)vector[3] == 7,
		     "El cuarto elemento del vector es correcto (7).");
	pa2m_afirmar(*(int *)vector[4] == 15,
		     "El quinto elemento del vector es correcto (15).");
	pa2m_afirmar(*(int *)vector[5] == 12,
		     "El sexto elemento del vector es correcto (12).");
	pa2m_afirmar(*(int *)vector[6] == 18,
		     "El séptimo elemento del vector es correcto (18).");

	abb_destruir(abb);
}

void abb_vectorizar_guarda_elementos_en_el_vector_en_el_orden_POSTORDEN()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int elementos[] = { 10, 5, 15, 3, 7, 12, 18 };
	for (int i = 0; i < 7; i++)
		abb_insertar(abb, &elementos[i]);

	void *vector[7];
	size_t guardados = abb_vectorizar(abb, ABB_POSTORDEN, vector, 7);

	pa2m_afirmar(
		guardados == 7,
		"Se guardaron los 7 elementos en el vector (modo: POSTORDEN).");
	pa2m_afirmar(*(int *)vector[0] == 3,
		     "El primer elemento del vector es correcto (3).");
	pa2m_afirmar(*(int *)vector[1] == 7,
		     "El segundo elemento del vector es correcto (7).");
	pa2m_afirmar(*(int *)vector[2] == 5,
		     "El tercer elemento del vector es correcto (5).");
	pa2m_afirmar(*(int *)vector[3] == 12,
		     "El cuarto elemento del vector es correcto (12).");
	pa2m_afirmar(*(int *)vector[4] == 18,
		     "El quinto elemento del vector es correcto (18).");
	pa2m_afirmar(*(int *)vector[5] == 15,
		     "El sexto elemento del vector es correcto (15).");
	pa2m_afirmar(*(int *)vector[6] == 10,
		     "El séptimo elemento del vector es correcto (10).");

	abb_destruir(abb);
}

void abb_vectorizar_no_excede_la_capacidad_del_vector()
{
	abb_t *abb = abb_crear(comparar_enteros);

	int elementos[] = { 10, 5, 15, 3, 7, 12, 18 };
	for (int i = 0; i < 7; i++)
		abb_insertar(abb, &elementos[i]);

	void *vector[5];
	size_t guardados = abb_vectorizar(abb, ABB_INORDEN, vector, 5);

	pa2m_afirmar(
		guardados == 5,
		"Se guardaron solo 5 elementos en el vector debido a la capacidad.");
	pa2m_afirmar(*(int *)vector[0] == 3,
		     "El primer elemento del vector es correcto (3).");
	pa2m_afirmar(*(int *)vector[1] == 5,
		     "El segundo elemento del vector es correcto (5).");
	pa2m_afirmar(*(int *)vector[2] == 7,
		     "El tercer elemento del vector es correcto (7).");
	pa2m_afirmar(*(int *)vector[3] == 10,
		     "El cuarto elemento del vector es correcto (10).");
	pa2m_afirmar(*(int *)vector[4] == 12,
		     "El quinto elemento del vector es correcto (12).");

	abb_destruir(abb);
}

void sacar_un_nodo_con_hijo_no_rompe_la_estructura()
{
	pa2m_nuevo_grupo(
		"Testeo de sacar un nodo con un hijo no rompe la estructura");
	abb_t *abb = abb_crear(comparar_enteros);

	int elementos[] = { 10, 4, 3 };
	for (int i = 0; i < 3; i++)
		abb_insertar(abb, &elementos[i]);

	void *abb_original[3];
	abb_vectorizar(abb, ABB_PREORDEN, abb_original, 3);
	pa2m_afirmar(*(int *)abb_original[0] == 10,
		     "El primer elemento del vector es correcto (10).");
	pa2m_afirmar(*(int *)abb_original[1] == 4,
		     "El segundo elemento del vector es correcto (4).");
	pa2m_afirmar(*(int *)abb_original[2] == 3,
		     "El tercer elemento del vector es correcto (3).");

	int nodo_a_sacar = 4;
	int *sacado = abb_sacar(abb, &nodo_a_sacar);
	pa2m_afirmar(*sacado == 4, "Se elimina el nodo \"4\"");

	void *abb_modificado[2];
	abb_vectorizar(abb, ABB_PREORDEN, abb_modificado, 2);
	pa2m_afirmar(*(int *)abb_modificado[0] == 10,
		     "El primer elemento del vector es correcto (10).");
	pa2m_afirmar(*(int *)abb_modificado[1] == 3,
		     "El segundo elemento del vector es correcto (3).");

	abb_destruir(abb);
}

void sacar_la_raiz_con_varios_elementos_no_rompe_la_estructura()
{
	pa2m_nuevo_grupo(
		"Testeo de sacar la raíz con varios elementos no rompe la estructura");
	abb_t *abb = abb_crear(comparar_enteros);

	int elementos[] = { 10, 11, 5, 4, 6, 7 };
	for (int i = 0; i < 6; i++)
		abb_insertar(abb, &elementos[i]);

	void *abb_original[6];
	abb_vectorizar(abb, ABB_PREORDEN, abb_original, 6);
	pa2m_afirmar(*(int *)abb_original[0] == 10,
		     "El primer elemento del vector es correcto (10).");
	pa2m_afirmar(*(int *)abb_original[1] == 5,
		     "El segundo elemento del vector es correcto (5).");
	pa2m_afirmar(*(int *)abb_original[2] == 4,
		     "El tercer elemento del vector es correcto (4).");
	pa2m_afirmar(*(int *)abb_original[3] == 6,
		     "El cuarto elemento del vector es correcto (6).");
	pa2m_afirmar(*(int *)abb_original[4] == 7,
		     "El quinto elemento del vector es correcto (7).");
	pa2m_afirmar(*(int *)abb_original[5] == 11,
		     "El sexto elemento del vector es correcto (11).");

	int raiz = 10;
	int *sacado = abb_sacar(abb, &raiz);
	pa2m_afirmar(*sacado == 10, "Se elimina la raíz \"10\"");

	void *abb_modificado[5];
	abb_vectorizar(abb, ABB_PREORDEN, abb_modificado, 5);
	pa2m_afirmar(*(int *)abb_modificado[0] == 7,
		     "El primer elemento del vector es correcto (7).");
	pa2m_afirmar(*(int *)abb_modificado[1] == 5,
		     "El segundo elemento del vector es correcto (5).");
	pa2m_afirmar(*(int *)abb_modificado[2] == 4,
		     "El tercer elemento del vector es correcto (3).");
	pa2m_afirmar(*(int *)abb_modificado[3] == 6,
		     "El cuarto elemento del vector es correcto (15).");
	pa2m_afirmar(*(int *)abb_modificado[4] == 11,
		     "El quinto elemento del vector es correcto (12).");

	abb_destruir(abb);
}

void abb_crear_falla_al_pasar_un_comparador_NULL()
{
	abb_t *abb = abb_crear(NULL);
	pa2m_afirmar(abb == NULL, "El ABB no se crea con un comparador NULL.");
}

void abb_insertar_falla_al_pasar_un_abb_NULL()
{
	abb_t *abb = NULL;
	int elemento = 42;
	pa2m_afirmar(!abb_insertar(abb, &elemento),
		     "No se puede insertar un elemento en un ABB NULL.");
}

void abb_insertar_falla_al_pasar_un_comparador_NULL()
{
	abb_t *abb = abb_crear(NULL);
	int elemento = 42;
	pa2m_afirmar(
		!abb_insertar(abb, &elemento),
		"No se puede insertar un elemento en un ABB con comparador NULL.");

	abb_destruir(abb);
}

void abb_existe_falla_al_pasar_un_abb_NULL()
{
	abb_t *abb = NULL;
	int elemento = 42;
	pa2m_afirmar(!abb_existe(abb, &elemento),
		     "No se puede encontrar un elemento en un ABB NULL.");
}

void abb_existe_falla_al_pasar_un_comparador_NULL()
{
	abb_t *abb = abb_crear(NULL);
	int elemento = 42;
	pa2m_afirmar(
		!abb_existe(abb, &elemento),
		"No se puede encontrar un elemento en un ABB con comparador NULL.");

	abb_destruir(abb);
}

void abb_buscar_falla_al_pasar_un_abb_NULL()
{
	abb_t *abb = NULL;
	int elemento = 42;
	pa2m_afirmar(!abb_buscar(abb, &elemento),
		     "No se puede buscar un elemento en un ABB NULL.");
}

void abb_buscar_falla_al_pasar_un_comparador_NULL()
{
	abb_t *abb = abb_crear(NULL);
	int elemento = 42;
	pa2m_afirmar(
		!abb_buscar(abb, &elemento),
		"No se puede buscar un elemento en un ABB con comparador NULL.");

	abb_destruir(abb);
}

void abb_sacar_falla_al_pasar_un_abb_NULL()
{
	abb_t *abb = NULL;
	int elemento = 42;
	pa2m_afirmar(!abb_sacar(abb, &elemento),
		     "No se puede sacar un elemento en un ABB NULL.");
}

void abb_sacar_falla_al_pasar_un_comparador_NULL()
{
	abb_t *abb = abb_crear(NULL);
	int elemento = 42;
	pa2m_afirmar(
		!abb_sacar(abb, &elemento),
		"No se puede sacar un elemento en un ABB con comparador NULL.");

	abb_destruir(abb);
}

void abb_tamanio_devuelve_cero_y_abb_vacio_indica_que_esta_vacio_al_pasar_un_abb_NULL()
{
	abb_t *abb = NULL;
	pa2m_afirmar(abb_tamanio(abb) == 0,
		     "El tamaño de un ABB NULL es 0 (valor obtenido: %i).",
		     abb_tamanio(abb));
	pa2m_afirmar(abb_vacio(abb) == true, "Un ABB NULL se considera vacío");
}

void abb_recorrer_falla_al_pasar_un_abb_NULL()
{
	abb_t *abb = NULL;
	pa2m_afirmar(abb_recorrer(abb, ABB_INORDEN, sumar_elementos, NULL) == 0,
		     "No se puede recorrer el ABB al pasar un ABB NULL.");
}

void abb_recorrer_falla_al_pasar_una_funcion_NULL()
{
	abb_t *abb = abb_crear(comparar_enteros);
	int elementos[] = { 10, 5, 15, 3, 7, 12, 18 };
	for (int i = 0; i < 7; i++)
		abb_insertar(abb, &elementos[i]);

	pa2m_afirmar(abb_recorrer(abb, ABB_INORDEN, NULL, NULL) == 0,
		     "No se puede recorrer el ABB al pasar una función NULL.");
	abb_destruir(abb);
}

void abb_recorrer_falla_al_pasar_un_orden_de_recorrido_incorrecto()
{
	abb_t *abb = abb_crear(comparar_enteros);
	int elementos[] = { 10, 5, 15, 3, 7, 12, 18 };
	for (int i = 0; i < 7; i++)
		abb_insertar(abb, &elementos[i]);

	pa2m_afirmar(
		abb_recorrer(abb, -1, sumar_elementos, NULL) == 0,
		"No se puede recorrer el ABB al pasar un orden de recorrido incorrecto.");
	abb_destruir(abb);
}

void abb_vectorizar_falla_al_pasar_un_abb_NULL()
{
	abb_t *abb = NULL;
	void *vector[7];
	size_t guardados = abb_vectorizar(abb, ABB_INORDEN, vector, 7);
	pa2m_afirmar(guardados == 0,
		     "No se puede vectorizar el ABB al pasar un ABB NULL.");
}

void abb_vectorizar_falla_al_pasar_un_vector_NULL()
{
	abb_t *abb = abb_crear(comparar_enteros);
	int elementos[] = { 10, 5, 15, 3, 7, 12, 18 };
	for (int i = 0; i < 7; i++)
		abb_insertar(abb, &elementos[i]);

	void **vector = NULL;
	size_t guardados = abb_vectorizar(abb, ABB_INORDEN, vector, 0);
	pa2m_afirmar(guardados == 0,
		     "No se puede vectorizar el ABB al pasar un vector NULL.");
	abb_destruir(abb);
}

void abb_vectorizar_falla_al_pasar_un_orden_de_recorrido_incorrecto()
{
	abb_t *abb = abb_crear(comparar_enteros);
	int elementos[] = { 10, 5, 15, 3, 7, 12, 18 };
	for (int i = 0; i < 7; i++)
		abb_insertar(abb, &elementos[i]);

	void *vector[7];
	size_t guardados = abb_vectorizar(abb, 5, vector, 7);
	pa2m_afirmar(
		guardados == 0,
		"No se puede vectorizar el ABB al pasar un orden de recorrido incorrecto.");
	abb_destruir(abb);
}

void abb_destruir_falla_al_pasar_un_abb_NULL()
{
	abb_t *abb = NULL;
	abb_destruir(abb);
	pa2m_afirmar(true, "No se puede destruir un ABB NULL.");
}

void abb_destruir_todo_falla_al_pasar_un_abb_NULL()
{
	abb_t *abb = NULL;
	abb_destruir_todo(abb, NULL);
	pa2m_afirmar(true, "No se puede destruir un ABB NULL.");
}

int main()
{
	pa2m_nuevo_grupo(
		"Pruebas aisladas de funcionamiento correcto de las funciones principales");
	pa2m_nuevo_grupo(
		"Prueba de abb_crear, abb_destruir y abb_destruir_todo");
	abb_crear_inicializa_y_destruye_correctamente_el_abb();
	abb_destruir_todo_libera_elementos_correctamente();
	pa2m_nuevo_grupo("Pruebas de abb_insertar");
	abb_insertar_inserta_un_elemento_en_el_abb_correctamente();
	abb_insertar_inserta_un_elemento_NULL_en_el_abb_correctamente();
	pa2m_nuevo_grupo("Pruebas de abb_existe");
	abb_existe_encuentra_un_elemento_insertado_previamente();
	abb_existe_encuentra_un_elemento_NULL_insertado_previamente();
	abb_existe_no_encuentra_elemento_porque_no_ha_sido_insertado();
	pa2m_nuevo_grupo("Pruebas de abb_buscar");
	abb_buscar_busca_un_elemento_insertado_previamente_y_lo_devuelve();
	abb_buscar_busca_un_elemento_no_insertado_devuelve_NULL();
	abb_buscar_busca_un_elemento_null_y_lo_devuelve();
	pa2m_nuevo_grupo("Pruebas de abb_tamanio");
	abb_tamanio_de_un_abb_recien_creado_devuelve_0();
	abb_tamanio_devuelve_la_cantidad_correcta_de_elementos_guardados_en_el_abb();
	pa2m_nuevo_grupo("Pruebas de abb_vacio");
	abb_vacio_devuelve_true_al_no_haber_elementos_en_el_abb();
	abb_vacio_devuelve_false_al_haber_elementos_en_el_abb();
	pa2m_nuevo_grupo("Pruebas de abb_sacar");
	abb_sacar_elimina_un_elemento_y_lo_devuelve();
	abb_eliminar_elemento_inexistente();
	pa2m_nuevo_grupo("Pruebas de abb_recorrer");
	abb_recorrer_aplica_funcion_a_cada_elemento_en_el_orden_INORDERN();
	abb_recorrer_aplica_funcion_a_cada_elemento_en_el_orden_POSTORDEN();
	abb_recorrer_aplica_funcion_a_cada_elemento_en_el_orden_PREORDEN();
	abb_recorrer_aplica_funcion_hasta_devolver_false_en_el_orden_INORDERN();
	abb_recorrer_aplica_funcion_hasta_devolver_false_en_el_orden_POSTORDEN();
	abb_recorrer_aplica_funcion_hasta_devolver_false_en_el_orden_PREORDEN();
	pa2m_nuevo_grupo("Pruebas de abb_vectorizar");
	abb_vectorizar_guarda_elementos_en_el_vector_en_el_orden_INORDERN();
	abb_vectorizar_guarda_elementos_en_el_vector_en_el_orden_PREORDEN();
	abb_vectorizar_guarda_elementos_en_el_vector_en_el_orden_POSTORDEN();
	abb_vectorizar_no_excede_la_capacidad_del_vector();

	pa2m_nuevo_grupo("Pruebas de mantenimiento de la estructura");
	sacar_un_nodo_con_hijo_no_rompe_la_estructura();
	sacar_la_raiz_con_varios_elementos_no_rompe_la_estructura();

	pa2m_nuevo_grupo("Pruebas con punteros inválidos");
	abb_crear_falla_al_pasar_un_comparador_NULL();
	abb_insertar_falla_al_pasar_un_abb_NULL();
	abb_insertar_falla_al_pasar_un_comparador_NULL();
	abb_existe_falla_al_pasar_un_abb_NULL();
	abb_existe_falla_al_pasar_un_comparador_NULL();
	abb_buscar_falla_al_pasar_un_abb_NULL();
	abb_buscar_falla_al_pasar_un_comparador_NULL();
	abb_sacar_falla_al_pasar_un_abb_NULL();
	abb_sacar_falla_al_pasar_un_comparador_NULL();
	abb_tamanio_devuelve_cero_y_abb_vacio_indica_que_esta_vacio_al_pasar_un_abb_NULL();
	abb_recorrer_falla_al_pasar_un_abb_NULL();
	abb_recorrer_falla_al_pasar_una_funcion_NULL();
	abb_recorrer_falla_al_pasar_un_orden_de_recorrido_incorrecto();
	abb_vectorizar_falla_al_pasar_un_abb_NULL();
	abb_vectorizar_falla_al_pasar_un_vector_NULL();
	abb_vectorizar_falla_al_pasar_un_orden_de_recorrido_incorrecto();
	abb_destruir_falla_al_pasar_un_abb_NULL();
	abb_destruir_todo_falla_al_pasar_un_abb_NULL();

	return pa2m_mostrar_reporte();
}
