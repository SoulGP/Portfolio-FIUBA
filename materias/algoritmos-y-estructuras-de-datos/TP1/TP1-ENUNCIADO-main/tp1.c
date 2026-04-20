#include "src/pokedex.h"
#include "src/archivo.h"
#include <stdio.h>
#include <string.h>

const char *LISTA = "list";

const char *MOSTRAR_FUERZA = "mostrar_fuerza";
const char *MOSTRAR = "mostrar";
const char *BUSCAR = "buscar";

const char *NOMBRE = "nombre";
const char *ID = "id";
const char *TIPO = "tipo";

const int TIPO_DESCONOCIDO = 7;

const char *tipo_a_string(tipo_pokemon tipo)
{
	switch (tipo) {
	case TIPO_AGUA:
		return "💧 Agua";
	case TIPO_FUEGO:
		return "🔥 Fuego";
	case TIPO_PLANTA:
		return "🌹 Planta";
	case TIPO_ROCA:
		return "🪨 Roca";
	case TIPO_ELECTRICO:
		return "⚡ Eléctrico";
	case TIPO_NORMAL:
		return "🙉 Normal";
	case TIPO_LUCHA:
		return "🥊 Lucha";
	default:
		return "❓ Desconocido";
	}
}

tipo_pokemon string_a_tipo(const char *str)
{
	if (strcmp(str, "agua") == 0) {
		return TIPO_AGUA;
	}
	if (strcmp(str, "fuego") == 0) {
		return TIPO_FUEGO;
	}
	if (strcmp(str, "planta") == 0) {
		return TIPO_PLANTA;
	}
	if (strcmp(str, "roca") == 0) {
		return TIPO_ROCA;
	}
	if (strcmp(str, "electrico") == 0) {
		return TIPO_ELECTRICO;
	}
	if (strcmp(str, "normal") == 0) {
		return TIPO_NORMAL;
	}
	if (strcmp(str, "lucha") == 0) {
		return TIPO_LUCHA;
	}
	return TIPO_DESCONOCIDO;
}

bool mostrar_pokemon(struct pokemon *pokemon, void *ctx)
{
	printf("%s (%d)  F%d/D%d/I%d   \t%s\n", pokemon->nombre, pokemon->id,
	       pokemon->fuerza, pokemon->destreza, pokemon->inteligencia,
	       tipo_a_string(pokemon->tipo));

	return true;
}

bool mostrar_pokemon_y_sumar_fuerza(struct pokemon *pokemon, void *ctx)
{
	unsigned *fuerza_total = ctx;
	printf("%s (%d)  F%d/D%d/I%d   \t%s\n", pokemon->nombre, pokemon->id,
	       pokemon->fuerza, pokemon->destreza, pokemon->inteligencia,
	       tipo_a_string(pokemon->tipo));

	*fuerza_total += pokemon->fuerza;
	return true;
}

bool mostrar_pokemon_si_tipo(struct pokemon *pkm, void *ctx)
{
	tipo_pokemon tipo_buscado = *(tipo_pokemon *)ctx;
	if (pkm->tipo == tipo_buscado) {
		printf("%s (%d)  F%d/D%d/I%d   \t%s\n", pkm->nombre, pkm->id,
		       pkm->fuerza, pkm->destreza, pkm->inteligencia,
		       tipo_a_string(pkm->tipo));
	}
	return true;
}

void buscar_pokemon_por_nombre(pokedex_t *pokedex, const char *nombre)
{
	const struct pokemon *pkm =
		pokedex_buscar_pokemon_nombre(pokedex, nombre);
	if (pkm == NULL) {
		printf("❌ No se encontró el Pokémon \"%s\" en la Pokédex.\n",
		       nombre);
		return;
	}

	printf("%s (%d)  F%d/D%d/I%d   \t%s\n", pkm->nombre, pkm->id,
	       pkm->fuerza, pkm->destreza, pkm->inteligencia,
	       tipo_a_string(pkm->tipo));
}

void buscar_pokemon_por_id(pokedex_t *pokedex, unsigned id)
{
	const struct pokemon *pkm = pokedex_buscar_pokemon_id(pokedex, id);
	if (pkm == NULL) {
		printf("❌ No se encontró el Pokémon (%u) en la Pokédex.\n",
		       id);
		return;
	}

	printf("%s (%d)  F%d/D%d/I%d   \t%s\n", pkm->nombre, pkm->id,
	       pkm->fuerza, pkm->destreza, pkm->inteligencia,
	       tipo_a_string(pkm->tipo));
}

int main(int argc, char const *argv[])
{
	if (argc == 1) {
		printf("Uso: %s <archivo.csv> <comando> <especificación> <parametro>\n\n",
		       argv[0]);
		printf("Ejemplo de uso:\n\n $ %s <archivo.csv> buscar tipo agua\n\n",
		       argv[0]);
		printf("para saber la lista de comandos y especificaciones use:\n\n $ %s list\n",
		       argv[0]);
		return 0;
	}

	if (argc > 1 && strcmp(argv[1], LISTA) == 0) {
		printf("===============================================================\n");
		printf("| Comandos:\n|\t ■  buscar <especificación> <parametro>\n|\t ■  mostrar <especificación>\n|\t ■  mostrar_fuerza\n");
		printf("===============================================================\n");
		printf("| Especificaciones:\n|\t ■  nombre\n|\t ■  id\n|\t ■  tipo (disponible solo para el comando \"buscar\")\n");
		printf("===============================================================\n");
		return 0;
	}

	const char *archivo = argv[1];
	const char *comando = argc > 2 ? argv[2] : NULL;
	const char *especificacion = argc > 3 ? argv[3] : NULL;
	const char *parametro = argc > 4 ? argv[4] : NULL;
	bool se_ejecuto_comando = false;

	pokedex_t *pokedex = pokedex_abrir(archivo);
	if (!pokedex) {
		printf("Error al abrir el archivo %s\n", archivo);
		return 1;
	}

	if (argc > 3 && comando && especificacion &&
	    strcmp(comando, MOSTRAR) == 0) {
		if (strcmp(especificacion, NOMBRE) == 0) {
			pokedex_iterar_pokemones(pokedex, ITERAR_NOMBRE,
						 mostrar_pokemon, NULL);
			se_ejecuto_comando = true;
		} else if (strcmp(especificacion, ID) == 0) {
			pokedex_iterar_pokemones(pokedex, ITERAR_ID,
						 mostrar_pokemon, NULL);
			se_ejecuto_comando = true;
		}
	} else if (argc > 3 && comando && especificacion &&
		   strcmp(comando, BUSCAR) == 0) {
		if (strcmp(especificacion, NOMBRE) == 0 && parametro) {
			buscar_pokemon_por_nombre(pokedex, parametro);
			se_ejecuto_comando = true;
		} else if (strcmp(especificacion, ID) == 0 && parametro) {
			buscar_pokemon_por_id(pokedex,
					      (unsigned)atoi(parametro));
			se_ejecuto_comando = true;
		} else if (strcmp(especificacion, TIPO) == 0 && parametro) {
			tipo_pokemon tipo = string_a_tipo(parametro);
			if (tipo == TIPO_DESCONOCIDO) {
				printf("Ingresaste un tipo incorrecto.\n");
				pokedex_destruir(pokedex);
				return 1;
			}
			pokedex_iterar_pokemones(pokedex, ITERAR_NOMBRE,
						 mostrar_pokemon_si_tipo,
						 &tipo);
			se_ejecuto_comando = true;
		}
	} else if (argc > 2 && comando &&
		   strcmp(comando, MOSTRAR_FUERZA) == 0) {
		unsigned fuerza_total = 0;
		pokedex_iterar_pokemones(pokedex, ITERAR_NOMBRE,
					 mostrar_pokemon_y_sumar_fuerza,
					 &fuerza_total);
		printf("\nSi sumamos todas las fuerzas, el resultado es: %d\n",
		       fuerza_total);
		se_ejecuto_comando = true;
	} else {
		printf("❌ Argumentos incorrectos. Use el siguiente comando para ver las funcionalidades:\n\n $ %s list\n",
		       argv[0]);
		pokedex_destruir(pokedex);
		return 1;
	}

	if (!se_ejecuto_comando) {
		printf("❌ Argumentos incorrectos. Use el siguiente comando para ver las funcionalidades:\n\n $ %s list\n",
		       argv[0]);
	}

	pokedex_destruir(pokedex);

	return 0;
}
