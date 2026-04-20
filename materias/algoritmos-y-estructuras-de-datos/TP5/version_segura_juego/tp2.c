#include "extra/engine.h"
#include "extra/ansi.h"
#include "src/menu.h"
#include "src/juego.h"
struct info_menu {
	pokedex_t *pokedex;
	unsigned int semilla;
	int salir;
};

void pausa_entre_menus()
{
	printf("Presione \"Enter\" para volver al menú.\n");
	mostrar_cursor();
	getchar();
	esconder_cursor();
}

void salir(void *ctx)
{
	struct info_menu *info = ctx;
	info->salir = 1;
}

int logica_del_juego(int tecla, void *ctx)
{
	tecla = tolower(tecla);
	if (tecla == 'q')
		return 1;
	borrar_pantalla();
	esconder_cursor();

	juego_t *juego = ctx;
	realizar_jugada(juego, tecla);

	mostrar_juego(juego);

	if (juego->jugadores[J1].puntaje == 0 ||
	    juego->jugadores[J2].puntaje == 0)
		return 1;
	juego->iteraciones_restantes--;
	return juego->iteraciones_restantes == 0;
}

void jugar(void *ctx)
{
	juego_t juego;
	struct info_menu *info = ctx;
	juego.semilla = info->semilla;
	srand(juego.semilla);
	juego.iteraciones_restantes = 1 * 60 * 5;
	juego.pokedex = info->pokedex;
	if (!juego.pokedex)
		return;

	iniciar_juego(&juego);

	game_loop(logica_del_juego, &juego);

	borrar_pantalla();

	if (juego.jugadores[J1].puntaje > juego.jugadores[J2].puntaje)
		printf("Jugador 1 gana con %zu puntos!\n",
		       juego.jugadores[J1].puntaje);
	else if (juego.jugadores[J2].puntaje > juego.jugadores[J1].puntaje)
		printf("Jugador 2 gana con %zu puntos!\n",
		       juego.jugadores[J2].puntaje);
	else
		printf("Empate! Ambos jugadores tienen %zu puntos.\n",
		       juego.jugadores[J1].puntaje);

	destruir_juego(&juego);
	printf("Semilla utilizada: %u\n", info->semilla);
	pausa_entre_menus();
}

void jugar_con_semilla(void *ctx)
{
	struct info_menu *info = ctx;
	printf("Ingrese una semilla (número entero): ");
	char semilla[100];
	if (!fgets(semilla, sizeof(semilla), stdin))
		return;
	info->semilla = (unsigned)atoi(semilla);
	jugar(ctx);
}

void jugar_con_semilla_aleatoria(void *ctx)
{
	struct info_menu *info = ctx;
	info->semilla = (unsigned)time(NULL);
	jugar(ctx);
}

void buscar_por_id(void *ctx)
{
	struct info_menu *info = ctx;
	char id[100];
	printf("Ingrese el ID del Pokémon: ");
	if (!fgets(id, sizeof(id), stdin))
		return;
	buscar_pokemon_por_id(info->pokedex, (unsigned)atoi(id));
	pausa_entre_menus();
}
void buscar_por_nombre(void *ctx)
{
	struct info_menu *info = ctx;
	char nombre[100];
	printf("Ingrese el nombre del Pokémon: ");
	if (!fgets(nombre, sizeof(nombre), stdin))
		return;
	buscar_pokemon_por_nombre(info->pokedex, nombre);
	pausa_entre_menus();
}

void menu_buscar_pokemon(void *ctx)
{
	struct info_menu *info = ctx;
	menu_t *menu = menu_crear(8);
	if (!menu) {
		printf("Error al crear el menú.\n");
		return;
	}
	menu_agregar_opcion(menu, "N", "Buscar por nombre", buscar_por_nombre);
	menu_agregar_opcion(menu, "I", "Buscar por ID", buscar_por_id);
	menu_agregar_opcion(menu, "A", "Menú Anterior", salir);

	while (!info->salir) {
		borrar_pantalla();
		menu_ejecutar(menu, info);
	}

	info->salir = 0;

	menu_destruir(menu);
}

void mostrar_pokemon_nombre(void *ctx)
{
	struct info_menu *info = ctx;
	borrar_pantalla();
	pokedex_iterar_pokemones(info->pokedex, ITERAR_NOMBRE, pokemon_mostrar,
				 NULL);
	pausa_entre_menus();
}

void mostrar_pokemon_id(void *ctx)
{
	struct info_menu *info = ctx;
	borrar_pantalla();
	pokedex_iterar_pokemones(info->pokedex, ITERAR_ID, pokemon_mostrar,
				 NULL);
	pausa_entre_menus();
}

void menu_mostrar_pokemon(void *ctx)
{
	struct info_menu *info = ctx;
	menu_t *menu = menu_crear(8);
	if (!menu) {
		printf("Error al crear el menú.\n");
		return;
	}
	menu_agregar_opcion(menu, "N", "Mostrar por nombre",
			    mostrar_pokemon_nombre);
	menu_agregar_opcion(menu, "I", "Mostrar por ID", mostrar_pokemon_id);
	menu_agregar_opcion(menu, "A", "Menú Anterior", salir);

	while (!info->salir) {
		borrar_pantalla();
		menu_ejecutar(menu, info);
	}

	info->salir = 0;

	menu_destruir(menu);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Uso: %s <archivo_pokedex>\n", argv[0]);
		return 1;
	}

	struct info_menu info = { .pokedex = pokedex_abrir(argv[1]),
				  .semilla = 0,
				  .salir = 0 };

	if (!info.pokedex) {
		printf("Error al abrir el archivo de la Pokédex.\n");
		return 1;
	}

	menu_t *menu = menu_crear(4);
	if (!menu) {
		printf("Error al crear el menú.\n");
		return 1;
	}

	int acerto = 0;
	acerto += menu_agregar_opcion(menu, "B", "Buscar Pokémon",
				      menu_buscar_pokemon);
	acerto += menu_agregar_opcion(menu, "M", "Mostrar Pokémon",
				      menu_mostrar_pokemon);
	acerto += menu_agregar_opcion(menu, "Q", "Salir", salir);
	acerto += menu_agregar_opcion(menu, "J", "Iniciar juego",
				      jugar_con_semilla_aleatoria);
	acerto += menu_agregar_opcion(menu, "S", "Iniciar juego con semilla",
				      jugar_con_semilla);
	if (!acerto) {
		printf("Error al agregar opciones al menú.\n");
		pokedex_destruir(info.pokedex);
		menu_destruir(menu);
		return 1;
	}

	while (!info.salir) {
		menu_ejecutar(menu, &info);
		borrar_pantalla();
	}

	pokedex_destruir(info.pokedex);

	menu_destruir(menu);

	return 0;
}