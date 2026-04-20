#include "mostrar_pokemones.h"

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
	return -1;
}

bool pokemon_mostrar(struct pokemon *pokemon, void *ctx)
{
	printf("%s (%d)  F%d/D%d/I%d   \t%s\n", pokemon->nombre, pokemon->id,
	       pokemon->fuerza, pokemon->destreza, pokemon->inteligencia,
	       tipo_a_string(pokemon->tipo));

	return true;
}

void buscar_pokemon_por_nombre(pokedex_t *pokedex, const char *nombre)
{
	size_t len = strlen(nombre);
	char *nombre_ = (char *)nombre;
	nombre_[len - 1] = 0;
	const struct pokemon *pkm =
		pokedex_buscar_pokemon_nombre(pokedex, nombre_);
	if (pkm == NULL) {
		printf("❌ No se encontró el Pokémon \"%s\" en la Pokédex.\n",
		       nombre_);
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