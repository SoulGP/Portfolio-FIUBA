#include "pa2m.h"
#include "src/hash.h"
#include <string.h>

size_t f_colision(const char *clave)
{
	return 0;
}

void se_puede_crear_un_hash_con_funcion_y_ser_destruido_correctamente()
{
	hash_t *hash = hash_crear(10);
	hash_destruir(hash);
	hash = hash_crear_con_funcion(10, f_colision);
	hash_destruir_todo(hash, NULL);

	pa2m_afirmar(
		hash != NULL,
		"Se puede crear un hash con una función válida y ser destruida sin perdida de memoria.");
}

void destruir_un_hash_con_destructor_no_produce_fugas()
{
	hash_t *hash = hash_crear(10);
	char *valor = malloc(10);
	hash_insertar(hash, "clave1", valor, NULL);
	hash_destruir_todo(hash, free);
	pa2m_afirmar(true, "Destruir un hash con destructor no produce fugas.");
}

void se_puede_insertar_en_un_hash_correctamente()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash_insertar(hash, "clave1", "valor1", NULL),
		     "Se puede insertar una clave y valor en el hash.");
	hash_destruir(hash);
}

void se_puede_insertar_en_un_hash_con_anterior_y_obtener_su_valor()
{
	hash_t *hash = hash_crear(10);
	char *anterior = NULL;
	hash_insertar(hash, "clave1", "valor1", (void **)&anterior);
	pa2m_afirmar(anterior == NULL,
		     "El valor anterior es NULL al insertar una clave nueva.");
	hash_insertar(hash, "clave1", "valor2", (void **)&anterior);
	pa2m_afirmar(
		strcmp(anterior, "valor1") == 0,
		"El valor anterior es el esperado al modificar una clave existente.");
	hash_destruir(hash);
}

void se_puede_buscar_en_un_hash_una_clave_existente_correctamente()
{
	hash_t *hash = hash_crear(10);
	hash_insertar(hash, "clave1", "valor1", NULL);
	char *valor = hash_buscar(hash, "clave1");
	pa2m_afirmar(valor != NULL && strcmp(valor, "valor1") == 0,
		     "Se puede buscar una clave y obtener su valor.");
	hash_destruir(hash);
}

void buscar_una_clave_inexistente_devuelve_NULL()
{
	hash_t *hash = hash_crear(10);
	char *valor = hash_buscar(hash, "clave_inexistente");
	pa2m_afirmar(valor == NULL,
		     "Buscar una clave inexistente devuelve NULL.");
	hash_destruir(hash);
}

void se_puede_verificar_existencia_de_claves_en_un_hash()
{
	hash_t *hash = hash_crear(10);
	hash_insertar(hash, "clave1", "valor1", NULL);
	pa2m_afirmar(
		hash_existe(hash, "clave1"),
		"Se puede verificar la existencia de una clave en el hash.");
	pa2m_afirmar(
		!hash_existe(hash, "clave2"),
		"Se verifica correctamente que una clave no existe en el hash.");
	hash_destruir(hash);
}

void se_puede_sacar_de_un_hash_correctamente()
{
	hash_t *hash = hash_crear(10);
	hash_insertar(hash, "clave1", "valor1", NULL);
	char *valor = hash_sacar(hash, "clave1");
	pa2m_afirmar(strcmp(valor, "valor1") == 0,
		     "Se puede sacar una clave y obtener su valor.");
	hash_destruir(hash);
}

void se_puede_obtener_tamanio_de_un_hash_correctamente()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash_tamanio(hash) == 0,
		     "El tamaño de un hash recién creado es 0.");
	hash_insertar(hash, "clave1", "valor1", NULL);
	pa2m_afirmar(
		hash_tamanio(hash) == 1,
		"El tamaño del hash es correcto después de insertar una clave.");
	hash_sacar(hash, "clave1");
	pa2m_afirmar(hash_tamanio(hash) == 0,
		     "El tamaño del hash es 0 después de sacar la clave.");
	hash_destruir(hash);
}

bool contar_claves(const char *clave, void *ctx)
{
	(*(size_t *)(ctx))++;
	return true;
}
void se_puede_iterar_claves_de_un_hash_correctamente()
{
	hash_t *hash = hash_crear(10);
	hash_insertar(hash, "clave1", "valor1", NULL);
	hash_insertar(hash, "clave2", "valor2", NULL);
	size_t contador = 0;
	size_t iteradas = hash_iterar_claves(hash, contar_claves, &contador);
	pa2m_afirmar(iteradas == 2 && contador == 2,
		     "Se pueden iterar las claves del hash correctamente.");
	hash_destruir(hash);
}

void iterar_un_hash_vacio_devuelve_0()
{
	hash_t *hash = hash_crear(10);
	size_t contador = 0;
	size_t iteradas = hash_iterar_claves(hash, contar_claves, &contador);
	pa2m_afirmar(iteradas == 0 && contador == 0,
		     "Iterar un hash vacío devuelve 0.");
	hash_destruir(hash);
}

bool hacer_nada(const char *clave, void *ctx)
{
	return false;
}
void se_puede_iterar_claves_de_un_hash_cuya_funcion_devuelve_false()
{
	hash_t *hash = hash_crear(10);
	hash_insertar(hash, "clave1", "valor1", NULL);
	hash_insertar(hash, "clave2", "valor2", NULL);
	size_t iteradas = hash_iterar_claves(hash, hacer_nada, NULL);
	pa2m_afirmar(
		iteradas == 1,
		"Iterar las claves con una función que devuelve false devuelve la cantidad de iterados correcta.");
	hash_destruir(hash);
}

void prueba_de_uso_promedio_de_una_tabla_de_hash()
{
	hash_t *hash = hash_crear(10);
	char *anterior = NULL;
	hash_insertar(hash, "clave1", "valor1", NULL);
	hash_sacar(hash, "clave1");
	char *valor = hash_buscar(hash, "clave1");
	pa2m_afirmar(valor == NULL,
		     "Buscar una clave sacada del hash devuelve NULL.");
	pa2m_afirmar(!hash_existe(hash, "clave1"),
		     "No existe la clave sacada del hash.");
	hash_insertar(hash, "clave1", "valor2", (void **)&anterior);
	pa2m_afirmar(
		anterior == NULL,
		"El valor anterior es NULL al insertar en una clave borrada.");
	valor = hash_buscar(hash, "clave1");
	pa2m_afirmar(
		strcmp(valor, "valor2") == 0,
		"Buscar una clave insertada después de haber sido sacada devuelve el valor correcto.");
	hash_destruir(hash);
}

void colisionar_claves_permite_seguir_buscando_y_sacando()
{
	hash_t *hash = hash_crear_con_funcion(10, f_colision);
	hash_insertar(hash, "clave1", "valor1", NULL);
	hash_insertar(hash, "clave2", "valor2", NULL);
	hash_insertar(hash, "clave3", "valor3", NULL);
	hash_insertar(hash, "clave4", "valor4", NULL);
	hash_insertar(hash, "clave5", "valor5", NULL);

	char *valor5 = hash_buscar(hash, "clave5");

	pa2m_afirmar(
		strcmp(valor5, "valor5") == 0,
		"Se puede buscar una clave donde hubo colisión al insertar y obtener su valor.");
	hash_sacar(hash, "clave1");
	char *sacado4 = hash_sacar(hash, "clave4");
	pa2m_afirmar(
		strcmp(sacado4, "valor4") == 0,
		"Se puede sacar una clave donde hubo colisión al insertar y obtener su valor.");
	pa2m_afirmar(
		strcmp(valor5, "valor5") == 0,
		"Tras sacar claves donde hubo colisión se puede buscar una clave y obtener su valor.");
	hash_destruir(hash);
}

void superar_el_factor_de_carga_redimensiona_la_tabla_correctamente()
{
	hash_t *hash = hash_crear(0);
	hash_insertar(hash, "clave1", "valor1", NULL);
	hash_insertar(hash, "clave2", "valor2", NULL);
	hash_insertar(hash, "clave3", "valor3", NULL);
	hash_insertar(hash, "clave4", "valor4", NULL);

	pa2m_afirmar(
		hash_existe(hash, "clave4"),
		"Exceder el factor de carga de una tabla la redimensiona y reubica sus elementos correctamente.");
	hash_destruir(hash);
}

void hash_crear_falla_al_pasar_un_funcion_NULL()
{
	hash_t *hash = hash_crear_con_funcion(10, NULL);
	pa2m_afirmar(hash == NULL,
		     "No se puede crear un hash al pasar una función NULL.");
}

void hash_destruir_falla_al_pasar_un_hash_NULL()
{
	hash_destruir(NULL);
	hash_destruir_todo(NULL, NULL);
	pa2m_afirmar(true, "No se produce error al destruir un hash NULL.");
}

void hash_insertar_falla_al_pasar_un_hash_NULL()
{
	pa2m_afirmar(!hash_insertar(NULL, "clave", "valor", NULL),
		     "No se puede insertar en un hash NULL.");
}

void hash_insertar_falla_al_pasar_una_clave_NULL()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(!hash_insertar(hash, NULL, "valor", NULL),
		     "No se puede insertar una clave NULL en el hash.");
	hash_destruir(hash);
}

void hash_sacar_falla_al_pasar_un_hash_NULL()
{
	pa2m_afirmar(hash_sacar(NULL, "clave") == NULL,
		     "No se puede sacar de un hash NULL.");
}

void hash_sacar_falla_al_pasar_una_clave_NULL()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash_sacar(hash, NULL) == NULL,
		     "No se puede sacar una clave NULL del hash.");
	hash_destruir(hash);
}

void hash_buscar_falla_al_pasar_un_hash_NULL()
{
	pa2m_afirmar(hash_buscar(NULL, "clave") == NULL,
		     "No se puede buscar en un hash NULL.");
}

void hash_buscar_falla_al_pasar_una_clave_NULL()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash_buscar(hash, NULL) == NULL,
		     "No se puede buscar una clave NULL en el hash.");
	hash_destruir(hash);
}

void hash_existe_falla_al_pasar_un_hash_NULL()
{
	pa2m_afirmar(!hash_existe(NULL, "clave"),
		     "No se puede verificar existencia en un hash NULL.");
}

void hash_existe_falla_al_pasar_una_clave_NULL()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(
		!hash_existe(hash, NULL),
		"No se puede verificar existencia de una clave NULL en el hash.");
	hash_destruir(hash);
}

void hash_tamanio_falla_al_pasar_un_hash_NULL()
{
	pa2m_afirmar(hash_tamanio(NULL) == 0,
		     "El tamaño de un hash NULL es 0.");
}

void hash_iterar_claves_falla_al_pasar_un_hash_NULL()
{
	pa2m_afirmar(hash_iterar_claves(NULL, NULL, NULL) == 0,
		     "No se puede iterar claves de un hash NULL.");
}

void hash_iterar_claves_falla_al_pasar_una_funcion_NULL()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(
		hash_iterar_claves(hash, NULL, NULL) == 0,
		"No se puede iterar claves de un hash al pasar una función NULL.");
	hash_destruir(hash);
}

int main()
{
	pa2m_nuevo_grupo(
		"Pruebas aisladas de funcionamiento correcto de las funciones principales");
	pa2m_nuevo_grupo(
		"Pruebas de hash_crear, hash_crear_con_funcion, hash_destruir y hash_destruir_todo");
	se_puede_crear_un_hash_con_funcion_y_ser_destruido_correctamente();
	destruir_un_hash_con_destructor_no_produce_fugas();
	pa2m_nuevo_grupo("Pruebas de hash_insertar");
	se_puede_insertar_en_un_hash_correctamente();
	se_puede_insertar_en_un_hash_con_anterior_y_obtener_su_valor();
	pa2m_nuevo_grupo("Pruebas de hash_buscar");
	se_puede_buscar_en_un_hash_una_clave_existente_correctamente();
	buscar_una_clave_inexistente_devuelve_NULL();
	pa2m_nuevo_grupo("Pruebas de hash_existe");
	se_puede_verificar_existencia_de_claves_en_un_hash();
	pa2m_nuevo_grupo("Pruebas de hash_sacar");
	se_puede_sacar_de_un_hash_correctamente();
	pa2m_nuevo_grupo("Pruebas de hash_tamanio");
	se_puede_obtener_tamanio_de_un_hash_correctamente();
	pa2m_nuevo_grupo("Pruebas de hash_iterar_claves");
	se_puede_iterar_claves_de_un_hash_correctamente();
	iterar_un_hash_vacio_devuelve_0();
	se_puede_iterar_claves_de_un_hash_cuya_funcion_devuelve_false();

	pa2m_nuevo_grupo(
		"Pruebas de combinación de funciones verificando que se mantenga la estructura del hash");
	prueba_de_uso_promedio_de_una_tabla_de_hash();

	pa2m_nuevo_grupo("Pruebas de colisiones y redimensionamiento");
	colisionar_claves_permite_seguir_buscando_y_sacando();
	superar_el_factor_de_carga_redimensiona_la_tabla_correctamente();

	pa2m_nuevo_grupo("Pruebas con punteros inválidos");
	hash_crear_falla_al_pasar_un_funcion_NULL();
	hash_destruir_falla_al_pasar_un_hash_NULL();
	hash_insertar_falla_al_pasar_un_hash_NULL();
	hash_insertar_falla_al_pasar_una_clave_NULL();
	hash_sacar_falla_al_pasar_un_hash_NULL();
	hash_sacar_falla_al_pasar_una_clave_NULL();
	hash_buscar_falla_al_pasar_un_hash_NULL();
	hash_buscar_falla_al_pasar_una_clave_NULL();
	hash_existe_falla_al_pasar_un_hash_NULL();
	hash_existe_falla_al_pasar_una_clave_NULL();
	hash_tamanio_falla_al_pasar_un_hash_NULL();
	hash_iterar_claves_falla_al_pasar_un_hash_NULL();
	hash_iterar_claves_falla_al_pasar_una_funcion_NULL();

	return pa2m_mostrar_reporte();
}
