#ifndef JUEGO_H_
#define JUEGO_H_

#include "cola.h"
#include "pila.h"
#include "lista.h"
#include "hash.h"
#include "pokedex.h"
#include "mostrar_pokemones.h"
#include "../extra/engine.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#define CANT_JUGADORES 2
#define CANT_POKEMONES 4

#define MAX_FILAS 12
#define MAX_COLUMNAS 12

#define PUNTAJE_INICIAL 10

#define CARACTER_J1 '@'
#define CARACTER_J2 '#'

enum num_jugadores { J1, J2 };

typedef struct coordenada {
	int fil;
	int col;
} coordenada_t;

typedef struct celda {
	char inicial;
	tipo_pokemon tipo;
} celda_t;

typedef struct {
	celda_t (*terreno[2])[MAX_COLUMNAS];
	size_t contador[CANT_JUGADORES];
	size_t limite;
} terreno_t;

typedef struct jugador {
	coordenada_t posicion;
	pila_t *pkm_capturados;
	pila_t *pkm_por_capturar;
	size_t puntaje;
} jugador_t;

typedef struct pokemones {
	coordenada_t posicion;
	bool capturado[CANT_JUGADORES];
	struct pokemon pokemon;
} pokemones_t;

typedef struct pokemones_salvajes {
	lista_t *pokemones_salvajes;
	cola_t *siguiente_pkm_salvaje;
} pokemones_salvajes_t;

typedef struct juego {
	pokedex_t *pokedex;
	jugador_t *jugadores;
	pokemones_salvajes_t pokemones_salvajes[CANT_JUGADORES];
	hash_t *hash_acciones;
	unsigned iteraciones_restantes;
	unsigned int semilla;
} juego_t;

typedef void (*accion_t)(jugador_t *);

/**
 * Inicializará el juego , cargando toda la información inicial de los jugadores,
 * los pokemones y el terreno.
 */
void juego_inicializar(juego_t *juego);

/**
 * Realizará la acción recibida por parámetro.
 */
void realizar_jugada(juego_t *juego, int accion);

/**
 * Muestra el estado actual del juego.
 * 
 * Imprime el estado del juego, incluyendo
 * información relevante como el puntaje, el tiempo restante, etc.
 */
void juego_mostrar(juego_t juego);

/**
 * El juego se dará por terminado cuando se acabe el tiempo o alguno de los jugadores
 * se quede sin puntos.
 * 
 * Devuelve 0 si el juego sigue en curso, 1 si se ha terminado. 
 */
int juego_estado(juego_t juego);

/**
 * Libera la memoria reservada para el juego.
 */
void juego_destruir(juego_t *juego);

#endif // JUEGO_H_