#include "pa2m.h"
#include "src/menu.h"
#include "src/juego.h"

#define POKEDEX_CSV "datos/pokedex.csv"

void opcion_prueba(void *ctx)
{
	pa2m_afirmar(ctx == NULL,
		     "La opción de prueba se ejecutó correctamente.");
}

void se_puede_crear_un_menu_y_ser_destruido_correctamente()
{
	menu_t *menu = menu_crear(10);
	pa2m_afirmar(
		menu != NULL,
		"Se puede crear un menú correctamente y ser destruida sin perdida de memoria.");
	menu_destruir(menu);
}

void se_puede_agregar_opciones_a_un_menu()
{
	menu_t *menu = menu_crear(10);
	pa2m_afirmar(menu_agregar_opcion(menu, "A", "Opción A", opcion_prueba),
		     "Se puede agregar una opción al menú.");
	pa2m_afirmar(menu_agregar_opcion(menu, "B", "Opción B", opcion_prueba),
		     "Se puede agregar otra opción al menú.");
	menu_destruir(menu);
}

void se_puede_mostrar_un_menu_con_opciones()
{
	menu_t *menu = menu_crear(10);
	int acerto = 0;
	acerto += menu_agregar_opcion(menu, "A", "Opción A", opcion_prueba);
	acerto += menu_agregar_opcion(menu, "B", "Opción B", opcion_prueba);
	menu_mostrar(menu);
	pa2m_afirmar(acerto == 2,
		     "Se puede mostrar un menú con opciones correctamente.");
	menu_destruir(menu);
}

void se_puede_ejecutar_opcion_de_un_menu()
{
	menu_t *menu = menu_crear(10);
	menu_agregar_opcion(menu, "A", "Opción A", opcion_prueba);
	opcion_menu_t *opcion = hash_buscar((hash_t *)menu, "A");
	opcion->funcion(NULL);
	menu_destruir(menu);
}

void menu_agregar_opcion_falla_al_pasar_parametros_NULL()
{
	menu_t *menu = menu_crear(10);
	pa2m_afirmar(!menu_agregar_opcion(NULL, "A", "Opción A", opcion_prueba),
		     "No se puede agregar una opción a un menú NULL.");
	pa2m_afirmar(!menu_agregar_opcion(menu, NULL, "Opción A",
					  opcion_prueba),
		     "No se puede agregar una opción con una inicial NULL.");
	pa2m_afirmar(
		!menu_agregar_opcion(menu, "A", NULL, opcion_prueba),
		"No se puede agregar una opción con una descripción NULL.");
	pa2m_afirmar(!menu_agregar_opcion(menu, "A", "Opción A", NULL),
		     "No se puede agregar una opción con una función NULL.");
	menu_destruir(menu);
}

void menu_agregar_opcion_falla_al_pasar_inicial_o_descripcion_vacia()
{
	menu_t *menu = menu_crear(10);
	pa2m_afirmar(!menu_agregar_opcion(menu, "", "Opción A", opcion_prueba),
		     "No se puede agregar una opción con una inicial vacía.");
	pa2m_afirmar(
		!menu_agregar_opcion(menu, "A", "", opcion_prueba),
		"No se puede agregar una opción con una descripción vacía.");
	menu_destruir(menu);
}

void menu_agregar_opcion_falla_al_pasar_un_string_muy_largo_como_inicial()
{
	menu_t *menu = menu_crear(10);
	char inicial_largo[100] =
		"Te pedi una inicial y me pasaste un string -_-";
	pa2m_afirmar(
		!menu_agregar_opcion(menu, inicial_largo, "Opción A",
				     opcion_prueba),
		"No se puede agregar una opción con una inicial demasiado larga.");
	menu_destruir(menu);
}

void menu_mostrar_falla_al_pasar_un_menu_NULL()
{
	menu_t *menu = NULL;
	menu_mostrar(menu);
	pa2m_afirmar(menu == NULL, "No se puede mostrar un menú NULL.");
}

void menu_mostrar_falla_al_pasar_un_menu_vacio()
{
	menu_t *menu = menu_crear(10);
	menu_mostrar(menu);
	pa2m_afirmar(menu != NULL, "No se puede mostrar un menú vacío.");
	menu_destruir(menu);
}

void menu_destruir_falla_al_pasar_un_menu_NULL()
{
	menu_t *menu = NULL;
	menu_destruir(menu);
	pa2m_afirmar(menu == NULL, "No se puede destruir un menú NULL.");
}

void se_puede_crear_un_juego_y_ser_destruido_correctamente()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;
	juego_inicializar(&juego);
	pa2m_afirmar(true,
		     "Se puede crear un juego y ser destruido correctamente.");
	pokedex_destruir(juego.pokedex);
	juego_destruir(&juego);
}

void realizar_jugada_puede_mover_al_j1()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;
	juego_inicializar(&juego);

	juego.jugadores[J1].posicion = (coordenada_t){ 0, 0 };
	realizar_jugada(&juego, 's');
	pa2m_afirmar(juego.jugadores[J1].posicion.fil == 1,
		     "El jugador J1 se mueve hacia abajo.");
	realizar_jugada(&juego, 'w');
	pa2m_afirmar(juego.jugadores[J1].posicion.fil == 0,
		     "El jugador J1 se mueve hacia arriba.");
	realizar_jugada(&juego, 'd');
	pa2m_afirmar(juego.jugadores[J1].posicion.col == 1,
		     "El jugador J1 se mueve hacia la derecha.");
	realizar_jugada(&juego, 'a');
	pa2m_afirmar(juego.jugadores[J1].posicion.col == 0,
		     "El jugador J1 se mueve hacia la izquierda.");
	pokedex_destruir(juego.pokedex);
	juego_destruir(&juego);
}

void realizar_jugada_puede_mover_al_j2()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;
	juego_inicializar(&juego);

	juego.jugadores[J2].posicion = (coordenada_t){ 0, 0 };
	realizar_jugada(&juego, TECLA_ABAJO);
	pa2m_afirmar(juego.jugadores[J2].posicion.fil == 1,
		     "El jugador J2 se mueve hacia abajo.");
	realizar_jugada(&juego, TECLA_ARRIBA);
	pa2m_afirmar(juego.jugadores[J2].posicion.fil == 0,
		     "El jugador J2 se mueve hacia arriba.");
	realizar_jugada(&juego, TECLA_DERECHA);
	pa2m_afirmar(juego.jugadores[J2].posicion.col == 1,
		     "El jugador J2 se mueve hacia la derecha.");
	realizar_jugada(&juego, TECLA_IZQUIERDA);
	pa2m_afirmar(juego.jugadores[J2].posicion.col == 0,
		     "El jugador J2 se mueve hacia la izquierda.");
	pokedex_destruir(juego.pokedex);
	juego_destruir(&juego);
}

void realizar_jugada_puede_capturar_pokemon_j1_y_penalizar_a_j2()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;
	juego_inicializar(&juego);

	juego.jugadores[J1].posicion = (coordenada_t){ 0, 0 };
	pokemones_t *pkm = lista_obtener_elemento(
		juego.pokemones_salvajes[J1].pokemones_salvajes, 0);
	pkm->posicion = (coordenada_t){ 1, 0 };
	realizar_jugada(&juego, 's');

	pa2m_afirmar(!pila_vacia(juego.jugadores[J1].pkm_capturados),
		     "El jugador J1 captura un Pokémon.");

	pa2m_afirmar(
		juego.jugadores[J2].puntaje == PUNTAJE_INICIAL - 1,
		"El puntaje del jugador J2 se reduce al capturar un Pokémon.");

	pa2m_afirmar(!pila_vacia(juego.jugadores[J2].pkm_por_capturar),
		     "El jugador J2 tiene un Pokémon por capturar.");

	pokedex_destruir(juego.pokedex);
	juego_destruir(&juego);
}

void realizar_jugada_puede_capturar_pokemon_j2_y_penalizar_a_j1()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;
	juego_inicializar(&juego);

	juego.jugadores[J2].posicion = (coordenada_t){ 0, 0 };
	pokemones_t *pkm = lista_obtener_elemento(
		juego.pokemones_salvajes[J2].pokemones_salvajes, 0);
	pkm->posicion = (coordenada_t){ 1, 0 };
	realizar_jugada(&juego, TECLA_ABAJO);

	pa2m_afirmar(!pila_vacia(juego.jugadores[J2].pkm_capturados),
		     "El jugador J2 captura un Pokémon.");

	pa2m_afirmar(
		juego.jugadores[J1].puntaje == PUNTAJE_INICIAL - 1,
		"El puntaje del jugador J1 se reduce al capturar un Pokémon.");

	pa2m_afirmar(!pila_vacia(juego.jugadores[J1].pkm_por_capturar),
		     "El jugador J1 tiene un Pokémon por capturar.");

	pokedex_destruir(juego.pokedex);
	juego_destruir(&juego);
}

void capturar_un_pokemon_el_cual_coincide_el_tipo_con_el_del_por_capturar_bonifica_a_j1()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;
	juego_inicializar(&juego);

	juego.jugadores[J1].posicion = (coordenada_t){ 0, 0 };
	pokemones_t *pkm_j1 = lista_obtener_elemento(
		juego.pokemones_salvajes[J1].pokemones_salvajes, 0);
	pkm_j1->posicion = (coordenada_t){ 1, 0 };
	pokemones_t *pkm_j2 = lista_obtener_elemento(
		juego.pokemones_salvajes[J2].pokemones_salvajes, 0);
	pkm_j2->posicion = (coordenada_t){ 1, 0 };
	realizar_jugada(&juego, TECLA_ABAJO);
	realizar_jugada(&juego, 's');

	pa2m_afirmar(
		juego.jugadores[J1].puntaje == PUNTAJE_INICIAL,
		"El puntaje del jugador J1 se incrementa al capturar un Pokémon del mismo tipo que el del Pokémon por capturar.");

	pokedex_destruir(juego.pokedex);
	juego_destruir(&juego);
}

void capturar_un_pokemon_el_cual_coincide_el_tipo_con_el_del_por_capturar_bonifica_a_j2()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;
	juego_inicializar(&juego);

	juego.jugadores[J2].posicion = (coordenada_t){ 0, 0 };
	pokemones_t *pkm_j1 = lista_obtener_elemento(
		juego.pokemones_salvajes[J1].pokemones_salvajes, 0);
	pkm_j1->posicion = (coordenada_t){ 1, 0 };
	pokemones_t *pkm_j2 = lista_obtener_elemento(
		juego.pokemones_salvajes[J2].pokemones_salvajes, 0);
	pkm_j2->posicion = (coordenada_t){ 1, 0 };
	realizar_jugada(&juego, 's');
	realizar_jugada(&juego, TECLA_ABAJO);

	pa2m_afirmar(
		juego.jugadores[J2].puntaje == PUNTAJE_INICIAL,
		"El puntaje del jugador J2 se incrementa al capturar un Pokémon del mismo tipo que el del Pokémon por capturar.");

	pokedex_destruir(juego.pokedex);
	juego_destruir(&juego);
}

void juego_mostrar_se_puede_mostrar_el_terreno_con_pokemones()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;
	juego_inicializar(&juego);

	juego_mostrar(juego);

	pa2m_afirmar(true, "El terreno con Pokémon se muestra correctamente.");

	pokedex_destruir(juego.pokedex);
	juego_destruir(&juego);
}

void juego_estado_devuelve_0_si_el_juego_no_termino()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 1;
	juego.semilla = 0;
	juego_inicializar(&juego);

	int estado = juego_estado(juego);
	pa2m_afirmar(estado == 0, "El juego no ha terminado.");

	pokedex_destruir(juego.pokedex);
	juego_destruir(&juego);
}

void juego_estado_devuelve_1_si_el_juego_termino_por_puntaje_j1_igual_a_cero()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 1;
	juego.semilla = 0;
	juego_inicializar(&juego);

	juego.jugadores[J1].puntaje = 0;
	int estado = juego_estado(juego);
	pa2m_afirmar(estado == 1,
		     "El juego ha terminado porque perdio el jugador 1.");

	pokedex_destruir(juego.pokedex);
	juego_destruir(&juego);
}

void juego_estado_devuelve_1_si_el_juego_termino_por_puntaje_j2_igual_a_cero()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 1;
	juego.semilla = 0;
	juego_inicializar(&juego);

	juego.jugadores[J2].puntaje = 0;
	int estado = juego_estado(juego);
	pa2m_afirmar(estado == 1,
		     "El juego ha terminado porque perdio el jugador 2.");

	pokedex_destruir(juego.pokedex);
	juego_destruir(&juego);
}

void juego_estado_devuelve_1_si_el_juego_termino_por_tiempo()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;
	juego_inicializar(&juego);

	int estado = juego_estado(juego);
	pa2m_afirmar(estado == 1, "El juego ha terminado por tiempo.");

	pokedex_destruir(juego.pokedex);
	juego_destruir(&juego);
}

void juego_inicializar_falla_al_pasar_juego_NULL()
{
	juego_t *juego = NULL;
	juego_inicializar(juego);
	pa2m_afirmar(juego == NULL, "No se puede inicializar un juego NULL.");
}

void juego_inicializar_falla_al_pasar_pokedex_NULL()
{
	juego_t juego;
	juego.pokedex = NULL;
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;
	juego_inicializar(&juego);
	pa2m_afirmar(juego.pokedex == NULL,
		     "No se puede inicializar un juego con un pokedex NULL.");
}

void realizar_jugada_falla_al_pasar_juego_NULL()
{
	juego_t *juego = NULL;
	realizar_jugada(juego, 's');
	pa2m_afirmar(juego == NULL,
		     "No se puede realizar una jugada en un juego NULL.");
}

void realizar_jugada_falla_al_pasar_hash_de_acciones_NULL()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;
	juego.hash_acciones = NULL;
	juego.jugadores = malloc(sizeof(jugador_t) * CANT_JUGADORES);
	juego.pokemones_salvajes[J1].pokemones_salvajes = lista_crear();
	juego.pokemones_salvajes[J2].pokemones_salvajes = lista_crear();

	realizar_jugada(&juego, 's');
	pa2m_afirmar(
		juego.hash_acciones == NULL,
		"No se puede realizar una jugada con un hash de acciones NULL.");

	free(juego.jugadores);
	lista_destruir(juego.pokemones_salvajes[J1].pokemones_salvajes);
	lista_destruir(juego.pokemones_salvajes[J2].pokemones_salvajes);
	pokedex_destruir(juego.pokedex);
}

void realizar_jugada_falla_al_pasar_jugadores_NULL()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;
	juego.jugadores = NULL;
	juego.pokemones_salvajes[J1].pokemones_salvajes = lista_crear();
	juego.pokemones_salvajes[J2].pokemones_salvajes = lista_crear();
	juego.hash_acciones = hash_crear(3);

	realizar_jugada(&juego, 's');
	pa2m_afirmar(juego.jugadores == NULL,
		     "No se puede realizar una jugada con jugadores NULL.");

	lista_destruir(juego.pokemones_salvajes[J1].pokemones_salvajes);
	lista_destruir(juego.pokemones_salvajes[J2].pokemones_salvajes);
	hash_destruir(juego.hash_acciones);
	pokedex_destruir(juego.pokedex);
}

void realizar_jugada_falla_al_pasar_pokemones_salvajes_NULL()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;
	juego.pokemones_salvajes[J1].pokemones_salvajes = NULL;
	juego.pokemones_salvajes[J2].pokemones_salvajes = NULL;
	juego.jugadores = malloc(sizeof(jugador_t) * CANT_JUGADORES);
	juego.hash_acciones = hash_crear(3);

	realizar_jugada(&juego, 's');
	pa2m_afirmar(
		juego.pokemones_salvajes[J1].pokemones_salvajes == NULL &&
			juego.pokemones_salvajes[J2].pokemones_salvajes == NULL,
		"No se puede realizar una jugada con pokemones salvajes NULL.");

	free(juego.jugadores);
	hash_destruir(juego.hash_acciones);
	pokedex_destruir(juego.pokedex);
}

void juego_mostrar_falla_al_pasar_jugadores_NULL()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;
	juego.jugadores = NULL;

	juego_mostrar(juego);
	pa2m_afirmar(juego.jugadores == NULL,
		     "No se puede mostrar un juego con jugadores NULL.");

	pokedex_destruir(juego.pokedex);
}

void juego_mostrar_falla_al_pasar_una_pokedex_NULL()
{
	juego_t juego;
	juego.pokedex = NULL;
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;

	juego_mostrar(juego);
	pa2m_afirmar(juego.pokedex == NULL,
		     "No se puede mostrar un juego con una pokedex NULL.");
}

void juego_estado_falla_al_pasar_jugadores_NULL()
{
	juego_t juego;
	juego.pokedex = pokedex_abrir(POKEDEX_CSV);
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;
	juego.jugadores = NULL;

	int estado = juego_estado(juego);
	pa2m_afirmar(
		estado == 1,
		"No se puede obtener el estado de un juego con jugadores NULL.");

	pokedex_destruir(juego.pokedex);
}

void juego_destruir_falla_al_pasar_un_juego_NULL()
{
	juego_t *juego = NULL;
	juego_destruir(juego);
	pa2m_afirmar(juego == NULL, "No se puede destruir un juego NULL.");
}

void juego_destruir_falla_al_pasar_pokedex_NULL()
{
	juego_t juego;
	juego.pokedex = NULL;
	juego.iteraciones_restantes = 0;
	juego.semilla = 0;

	juego_destruir(&juego);
	pa2m_afirmar(juego.pokedex == NULL,
		     "No se puede destruir un juego con una pokedex NULL.");
}

int main()
{
	pa2m_nuevo_grupo("Pruebas TDA Menu");
	pa2m_nuevo_grupo(
		"Pruebas aisladas de funcionamiento correcto de las funciones principales");
	pa2m_nuevo_grupo("creación y destrucción");
	se_puede_crear_un_menu_y_ser_destruido_correctamente();
	pa2m_nuevo_grupo("agregar opciones al menú");
	se_puede_agregar_opciones_a_un_menu();
	pa2m_nuevo_grupo("mostrar un menú con opciones");
	se_puede_mostrar_un_menu_con_opciones();
	pa2m_nuevo_grupo("ejecutar una opción de un menú");
	se_puede_ejecutar_opcion_de_un_menu();

	pa2m_nuevo_grupo("Pruebas con punteros inválidos y atajo de errores");
	menu_agregar_opcion_falla_al_pasar_parametros_NULL();
	menu_agregar_opcion_falla_al_pasar_inicial_o_descripcion_vacia();
	menu_agregar_opcion_falla_al_pasar_un_string_muy_largo_como_inicial();
	menu_mostrar_falla_al_pasar_un_menu_NULL();
	menu_mostrar_falla_al_pasar_un_menu_vacio();
	menu_destruir_falla_al_pasar_un_menu_NULL();

	pa2m_nuevo_grupo("Pruebas TDA Juego");
	se_puede_crear_un_juego_y_ser_destruido_correctamente();

	pa2m_nuevo_grupo("Pruebas movimientos de jugadores");
	realizar_jugada_puede_mover_al_j1();
	realizar_jugada_puede_mover_al_j2();

	pa2m_nuevo_grupo("pruebas captura de pokemones");
	realizar_jugada_puede_capturar_pokemon_j1_y_penalizar_a_j2();
	realizar_jugada_puede_capturar_pokemon_j2_y_penalizar_a_j1();
	capturar_un_pokemon_el_cual_coincide_el_tipo_con_el_del_por_capturar_bonifica_a_j1();
	capturar_un_pokemon_el_cual_coincide_el_tipo_con_el_del_por_capturar_bonifica_a_j2();

	pa2m_nuevo_grupo("Pruebas de mostrar el juego");
	juego_mostrar_se_puede_mostrar_el_terreno_con_pokemones();

	pa2m_nuevo_grupo("Pruebas de estado del juego");
	juego_estado_devuelve_0_si_el_juego_no_termino();
	juego_estado_devuelve_1_si_el_juego_termino_por_puntaje_j1_igual_a_cero();
	juego_estado_devuelve_1_si_el_juego_termino_por_puntaje_j2_igual_a_cero();
	juego_estado_devuelve_1_si_el_juego_termino_por_tiempo();

	pa2m_nuevo_grupo("Pruebas de atajo de errores");
	juego_inicializar_falla_al_pasar_juego_NULL();
	juego_inicializar_falla_al_pasar_pokedex_NULL();
	realizar_jugada_falla_al_pasar_juego_NULL();
	realizar_jugada_falla_al_pasar_hash_de_acciones_NULL();
	realizar_jugada_falla_al_pasar_jugadores_NULL();
	realizar_jugada_falla_al_pasar_pokemones_salvajes_NULL();
	juego_mostrar_falla_al_pasar_jugadores_NULL();
	juego_mostrar_falla_al_pasar_una_pokedex_NULL();
	juego_estado_falla_al_pasar_jugadores_NULL();
	juego_destruir_falla_al_pasar_un_juego_NULL();
	juego_destruir_falla_al_pasar_pokedex_NULL();

	return pa2m_mostrar_reporte();
}
