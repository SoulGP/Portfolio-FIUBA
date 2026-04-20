#include "juego_imprimir.h"
#include "../extra/ansi.h"

const char *obtener_color_por_tipo(int tipo)
{
	switch (tipo) {
	case TIPO_FUEGO:
		return ANSI_COLOR_RED;
	case TIPO_AGUA:
		return ANSI_COLOR_BLUE;
	case TIPO_PLANTA:
		return ANSI_COLOR_GREEN;
	case TIPO_ELECTRICO:
		return ANSI_COLOR_YELLOW;
	case TIPO_ROCA:
		return ANSI_COLOR_MAGENTA;
	case TIPO_NORMAL:
		return ANSI_COLOR_BOLD;
	case TIPO_LUCHA:
		return ANSI_COLOR_CYAN;
	default:
		return ANSI_COLOR_WHITE;
	}
}

void imprimir_terreno(terreno_t ctx, unsigned int semilla,
		      unsigned int iteraciones_restantes)
{
	printf("\tW A S D\t\t\t\t\t      ↑ ←  ↓ → \n");
	for (int i = 0; i < MAX_FILAS; i++) {
		for (int j = 0; j < MAX_COLUMNAS; j++) {
			celda_t cel = ctx.terreno[J1][i][j];
			printf("%s%c " ANSI_COLOR_RESET,
			       obtener_color_por_tipo(cel.tipo), cel.inicial);
		}
		if (i == (MAX_FILAS / 2) - 1)
			printf(" Semilla:  %-10u ", semilla);
		else if (i == MAX_FILAS / 2)
			printf(" Tiempo:   %-10u ", iteraciones_restantes);
		else
			printf("                      ");

		for (int j = 0; j < MAX_COLUMNAS; j++) {
			celda_t cel = ctx.terreno[J2][i][j];
			printf("%s%c " ANSI_COLOR_RESET,
			       obtener_color_por_tipo(cel.tipo), cel.inicial);
		}
		putchar('\n');
	}
}

void imprimir_pkm_por_capturar(pokemones_t *a_capturar_j1,
			       pokemones_t *a_capturar_j2)
{
	if (a_capturar_j1)
		printf(" A capturar: %s (%c)\t\t\t" ANSI_COLOR_RESET,
		       obtener_color_por_tipo(a_capturar_j1->pokemon.tipo),
		       a_capturar_j1->pokemon.nombre[0]);
	else
		printf(" A capturar: \t\t\t\t");
	if (a_capturar_j2)
		printf("       A capturar: %s (%c)\n" ANSI_COLOR_RESET,
		       obtener_color_por_tipo(a_capturar_j2->pokemon.tipo),
		       a_capturar_j2->pokemon.nombre[0]);
	else
		printf("       A capturar: \n");
}

void imprimir_ultimos_pkm_capturados(pokemones_t *ultimo_cap_j1,
				     pokemones_t *ultimo_cap_j2)
{
	if (ultimo_cap_j1)
		printf(" Ultimo capturado:%s %-20s " ANSI_COLOR_RESET,
		       obtener_color_por_tipo(ultimo_cap_j1->pokemon.tipo),
		       ultimo_cap_j1->pokemon.nombre);
	else
		printf(" Ultimo capturado: \t\t\t");

	if (ultimo_cap_j2)
		printf("       Ultimo capturado:%s %-20s\n" ANSI_COLOR_RESET,
		       obtener_color_por_tipo(ultimo_cap_j2->pokemon.tipo),
		       ultimo_cap_j2->pokemon.nombre);
	else
		printf("       Ultimo capturado: \n");
}

void imprimir_puntajes(jugador_t *jugadores)
{
	printf(" Puntaje J1: %zu\t\t\t\t", jugadores[J1].puntaje);
	printf("       Puntaje J2: %zu\n", jugadores[J2].puntaje);
}

void imprimir_estado_de_juego(juego_t *juego)
{
	pokemones_t *ultimo_cap_j1 =
		pila_tope(juego->jugadores[J1].pkm_capturados);
	pokemones_t *ultimo_cap_j2 =
		pila_tope(juego->jugadores[J2].pkm_capturados);
	pokemones_t *a_capturar_j1 =
		pila_tope(juego->jugadores[J1].pkm_por_capturar);
	pokemones_t *a_capturar_j2 =
		pila_tope(juego->jugadores[J2].pkm_por_capturar);

	imprimir_pkm_por_capturar(a_capturar_j1, a_capturar_j2);
	imprimir_ultimos_pkm_capturados(ultimo_cap_j1, ultimo_cap_j2);
	imprimir_puntajes(juego->jugadores);
	printf("\nPulse Q para salir...\n");
}