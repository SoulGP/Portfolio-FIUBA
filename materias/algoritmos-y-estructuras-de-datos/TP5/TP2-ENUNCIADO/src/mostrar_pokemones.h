#ifndef MOSTRAR_POKEMONES_H_
#define MOSTRAR_POKEMONES_H_

#include "pokedex.h"
#include "archivo.h"
#include <stdio.h>
#include <string.h>

/**
 *  Muestra a todos los Pokémon de la pokedex.
 */
bool pokemon_mostrar(struct pokemon *pokemon, void *ctx);

/**
 *  Muestra un Pokémon por su NOMBRE.
 */
void buscar_pokemon_por_nombre(pokedex_t *pokedex, const char *nombre);

/**
 *  Muestra un Pokémon por su ID.
 */
void buscar_pokemon_por_id(pokedex_t *pokedex, unsigned id);

#endif // MOSTRAR_POKEMONES_H_