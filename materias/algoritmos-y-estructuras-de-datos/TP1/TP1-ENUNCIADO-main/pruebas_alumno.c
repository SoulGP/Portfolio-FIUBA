#include "pa2m.h"
#include "src/pokedex.h"

#define ARCHIVO_PRUEBA "ejemplos/pokedex.csv"
#define ARCHIVO_PRUEBA_INVALIDA "ejemplos/pokedex_test.csv"
#define ARCHIVO_PRUEBA_FORMATO "ejemplos/pokedex_formato.csv"
#define ARCHIVO_INEXISTENTE "dadafadalakjhakh"

void pokedex_abrir_lee_todos_los_pokemon_y_devuelve_la_cantidad_correcta()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	pa2m_afirmar(pokedex != NULL, "La pokedex no es nula");
	unsigned cantidad = pokedex_cantidad_pokemones(pokedex);
	pa2m_afirmar(cantidad == 9,
		     "La pokedex tiene 9 pokemones (valor obtenido: %d)",
		     cantidad);
	pokedex_destruir(pokedex);
}

void pokedex_abrir_lee_archivo_invalido_y_devuelve_cero()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA_INVALIDA);
	pa2m_afirmar(
		pokedex == NULL,
		"El archivo SI existe, pero no tiene un formato válido. La pokedex es nula");
	unsigned cantidad = pokedex_cantidad_pokemones(pokedex);
	pa2m_afirmar(cantidad == 0,
		     "La pokedex NO tiene pokemones (valor obtenido: %d)",
		     cantidad);
	pokedex_destruir(pokedex);
}

void pokedex_abrir_no_encuentra_archivo_y_devuelve_null()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_INEXISTENTE);
	pa2m_afirmar(pokedex == NULL,
		     "El archivo NO existe, la pokedex es nula");
	pokedex_destruir(pokedex);
}

void pokedex_abrir_lee_archivo_hasta_fallo_de_formato_y_devuelve_la_cantidad_correcta()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA_FORMATO);
	pa2m_afirmar(pokedex != NULL, "La pokedex no es nula");
	unsigned cantidad = pokedex_cantidad_pokemones(pokedex);
	pa2m_afirmar(cantidad == 6,
		     "La pokedex tiene 6 pokemones (valor obtenido: %d)",
		     cantidad);
	pokedex_destruir(pokedex);
}

void pokedex_buscar_pokemon_nombre_devuelve_un_pokemon_válido()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	const char *nombre = "Cacnea";
	const struct pokemon *pkm =
		pokedex_buscar_pokemon_nombre(pokedex, nombre);
	pa2m_afirmar(pkm->id == 331,
		     "pokedex_buscar_pokemon_nombre devuelve a Cacnea",
		     pkm->id);
	pokedex_destruir(pokedex);
}

void pokedex_buscar_pokemon_id_devuelve_un_pokemon_válido()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	unsigned id = 419;
	const struct pokemon *pkm = pokedex_buscar_pokemon_id(pokedex, id);
	pa2m_afirmar(pkm->id == 419,
		     "pokedex_buscar_pokemon_id devuelve a Floatzel", pkm->id);
	pokedex_destruir(pokedex);
}

void pokedex_buscar_pokemon_nombre_devuelve_null()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	const char *nombre = "easter_egg";
	const struct pokemon *pkm =
		pokedex_buscar_pokemon_nombre(pokedex, nombre);
	pa2m_afirmar(
		pkm == NULL,
		"pokedex_buscar_pokemon_nombre devuelve NULL (valor obtenido: %c)",
		pkm);
	pokedex_destruir(pokedex);
}

void pokedex_buscar_pokemon_id_devuelve_null()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	const struct pokemon *pkm = pokedex_buscar_pokemon_id(pokedex, 246323);
	pa2m_afirmar(
		pkm == NULL,
		"pokedex_buscar_pokemon_id devuelve NULL (valor obtenido: %c)",
		pkm);
	pokedex_destruir(pokedex);
}

bool contar_pokemon(struct pokemon *pokemon, void *ctx)
{
	if (pokemon) {
		(*(unsigned *)ctx)++;
	}

	return true;
}

bool contar_pokemones_debiles(struct pokemon *pokemon, void *ctx)
{
	if (pokemon->fuerza <= 10) {
		(*(unsigned *)ctx)++;
	}

	return true;
}

bool contar_pokemones_excesivamente_fuertes(struct pokemon *pokemon, void *ctx)
{
	if (pokemon->fuerza >= 10000000) {
		(*(unsigned *)ctx)++;
	} else {
		return false;
	}

	return true;
}

void pokedex_iterar_pokemones_en_pokedex_inválida_devuelve_la_cantidad_de_iterados_correcta()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_INEXISTENTE);
	unsigned cantidad_iterados = 0;
	pokedex_iterar_pokemones(pokedex, ITERAR_NOMBRE, contar_pokemon,
				 &cantidad_iterados);
	pa2m_afirmar(
		cantidad_iterados == 0,
		"pokedex_iterar_pokemones en pokedex inválida devuelve 0 pokemones iterados (valor obtenido: %u)",
		cantidad_iterados);
	pokedex_destruir(pokedex);
}

void pokedex_iterar_pokemones_en_pokedex_valida_devuelve_la_cantidad_de_iterados_y_pokemones_correcta()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	unsigned cantidad_iterados = 0;
	pokedex_iterar_pokemones(pokedex, ITERAR_NOMBRE, contar_pokemon,
				 &cantidad_iterados);
	pa2m_afirmar(
		cantidad_iterados == 9,
		"pokedex_iterar_pokemones por nombre devuelve 9 pokemones contados (valor obtenido: %u)",
		cantidad_iterados);
	cantidad_iterados = 0;
	pokedex_iterar_pokemones(pokedex, ITERAR_ID, contar_pokemon,
				 &cantidad_iterados);
	pa2m_afirmar(
		cantidad_iterados == 9,
		"pokedex_iterar_pokemones por id devuelve 9 pokemones contados (valor obtenido: %u)",
		cantidad_iterados);
	cantidad_iterados = 0;
	pokedex_iterar_pokemones(pokedex, ITERAR_ID, contar_pokemones_debiles,
				 &cantidad_iterados);
	pa2m_afirmar(
		cantidad_iterados == 3,
		"pokedex_iterar_pokemones con una funcion x devuelve 3 pokemones contados (valor obtenido: %u)",
		cantidad_iterados);
	cantidad_iterados = pokedex_iterar_pokemones(
		pokedex, ITERAR_NOMBRE, contar_pokemones_excesivamente_fuertes,
		&cantidad_iterados);
	pa2m_afirmar(
		cantidad_iterados == 1,
		"pokedex_iterar_pokemones devuelve 1 iteraciones xq' ctx es False (valor obtenido: %u)",
		cantidad_iterados);
	pokedex_destruir(pokedex);
}

bool sumar_fuerza(struct pokemon *pokemon, void *ctx)
{
	unsigned *fuerza_total = ctx;
	*fuerza_total += pokemon->fuerza;
	return true;
}

bool sumar_inteligencia(struct pokemon *pokemon, void *ctx)
{
	unsigned *inteligencia_total = ctx;
	*inteligencia_total += pokemon->inteligencia;
	return true;
}

void pokedex_iterar_pokemones_en_pokedex_valida_devuelve_la_suma_total_de_estadisticas_correcta()
{
	pokedex_t *pokedex = pokedex_abrir(ARCHIVO_PRUEBA);
	unsigned fuerza_total = 0;
	pokedex_iterar_pokemones(pokedex, ITERAR_NOMBRE, sumar_fuerza,
				 &fuerza_total);
	pa2m_afirmar(
		fuerza_total == 148,
		"pokedex_iterar_pokemones por nombre con funcion x devuelve la suma total de la Fuerza de todos los pokemones (valor obtenido: %u)",
		fuerza_total);
	unsigned inteligencia_total = 0;
	pokedex_iterar_pokemones(pokedex, ITERAR_ID, sumar_inteligencia,
				 &inteligencia_total);
	pa2m_afirmar(
		inteligencia_total == 146,
		"pokedex_iterar_pokemones por id con funcion x devuelve la suma total de la Inteligencia de todos los pokemones (valor obtenido: %u)",
		inteligencia_total);

	pokedex_destruir(pokedex);
}

void pokedex_buscar_pokemon_nombre_devuelve_un_pokemon_y_sus_datos_guardados_son_correctos()
{
	pokedex_t *pokedex =
		pokedex_abrir(ARCHIVO_PRUEBA); //25;Pikachu;E;20;15;17
	const char *nombre = "Pikachu";
	const struct pokemon *pkm =
		pokedex_buscar_pokemon_nombre(pokedex, nombre);
	pa2m_afirmar(*((unsigned *)(pkm->nombre)) == *((unsigned *)nombre),
		     "pokedex_buscar_pokemon_nombre devuelve a Pikachu");
	pa2m_afirmar(
		pkm->id == 25,
		"pokedex_buscar_pokemon_nombre devuelve su ID -> 25 (valor obtenido: %d)",
		pkm->id);
	pa2m_afirmar(
		pkm->tipo == TIPO_ELECTRICO,
		"pokedex_buscar_pokemon_nombre devuelve su Tipo -> Eléctrico (valor obtenido: %d)",
		pkm->tipo);
	pa2m_afirmar(
		pkm->fuerza == 20,
		"pokedex_buscar_pokemon_nombre devuelve su Fuerza -> 20 (valor obtenido: %d)",
		pkm->fuerza);
	pa2m_afirmar(
		pkm->destreza == 15,
		"pokedex_buscar_pokemon_nombre devuelve su Destreza -> 15 (valor obtenido: %d)",
		pkm->destreza);
	pa2m_afirmar(
		pkm->inteligencia == 17,
		"pokedex_buscar_pokemon_nombre devuelve su Inteligencia -> 17 (valor obtenido: %d)",
		pkm->inteligencia);
	pokedex_destruir(pokedex);
}

void pokedex_buscar_pokemon_id_devuelve_un_pokemon_y_sus_datos_guardados_son_correctos()
{
	pokedex_t *pokedex =
		pokedex_abrir(ARCHIVO_PRUEBA); //246;Larvitar;R;30;1;40
	unsigned id = 246;
	const char *nombre = "Larvitar";
	const struct pokemon *pkm = pokedex_buscar_pokemon_id(pokedex, id);
	pa2m_afirmar(*((unsigned *)(pkm->nombre)) == *((unsigned *)nombre),
		     "pokedex_buscar_pokemon_id devuelve a Larvitar");
	pa2m_afirmar(
		pkm->id == 246,
		"pokedex_buscar_pokemon_id devuelve su ID -> 246 (valor obtenido: %d)",
		pkm->id);
	pa2m_afirmar(
		pkm->tipo == TIPO_ROCA,
		"pokedex_buscar_pokemon_id devuelve su Tipo -> Roca (valor obtenido: %d)",
		pkm->tipo);
	pa2m_afirmar(
		pkm->fuerza == 30,
		"pokedex_buscar_pokemon_id devuelve su Fuerza -> 30 (valor obtenido: %d)",
		pkm->fuerza);
	pa2m_afirmar(
		pkm->destreza == 1,
		"pokedex_buscar_pokemon_id devuelve su Destreza -> 1 (valor obtenido: %d)",
		pkm->destreza);
	pa2m_afirmar(
		pkm->inteligencia == 40,
		"pokedex_buscar_pokemon_id devuelve su Inteligencia -> 40 (valor obtenido: %d)",
		pkm->inteligencia);
	pokedex_destruir(pokedex);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas con archivo válido");
	pokedex_abrir_lee_todos_los_pokemon_y_devuelve_la_cantidad_correcta();
	pokedex_buscar_pokemon_nombre_devuelve_un_pokemon_válido();
	pokedex_buscar_pokemon_id_devuelve_un_pokemon_válido();

	pa2m_nuevo_grupo("Pruebas con archivo inválido/inexistente");
	pokedex_abrir_no_encuentra_archivo_y_devuelve_null();
	pokedex_abrir_lee_archivo_invalido_y_devuelve_cero();
	pokedex_buscar_pokemon_nombre_devuelve_null();
	pokedex_buscar_pokemon_id_devuelve_null();

	pa2m_nuevo_grupo(
		"Pruebas con archivo válido, pero falla formato en cierto punto");
	pokedex_abrir_lee_archivo_hasta_fallo_de_formato_y_devuelve_la_cantidad_correcta();

	pa2m_nuevo_grupo("Pruebas validando datos de un pokemon recibido");
	pokedex_buscar_pokemon_nombre_devuelve_un_pokemon_y_sus_datos_guardados_son_correctos();
	pokedex_buscar_pokemon_id_devuelve_un_pokemon_y_sus_datos_guardados_son_correctos();

	pa2m_nuevo_grupo("Pruebas con pokedex_iterar_pokemones()");
	pokedex_iterar_pokemones_en_pokedex_valida_devuelve_la_cantidad_de_iterados_y_pokemones_correcta();
	pokedex_iterar_pokemones_en_pokedex_inválida_devuelve_la_cantidad_de_iterados_correcta();
	pokedex_iterar_pokemones_en_pokedex_valida_devuelve_la_suma_total_de_estadisticas_correcta();

	return pa2m_mostrar_reporte();
}
