#ifndef JUEGO_IMPRIMIR_H_
#define JUEGO_IMPRIMIR_H_

#include "juego.h"

#include <stdio.h>

/*
 * Imprime el terreno del juego.
 */
void imprimir_terreno(terreno_t ctx, unsigned int semilla,
		      unsigned int iteraciones_restantes);

/*
 * Imprime el estado del juego.
 */
void imprimir_estado_de_juego(juego_t juego);

#endif // JUEGO_IMPRIMIR_H_