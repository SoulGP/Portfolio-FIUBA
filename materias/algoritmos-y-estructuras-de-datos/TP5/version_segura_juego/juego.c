#include "juego.h"
#include "juego_imprimir.h"

void mover_arriba_j1(jugador_t *jugador)
{
	if (jugador[J1].posicion.fil > 0) {
		jugador[J1].posicion.fil--;
	}
}

void mover_abajo_j1(jugador_t *jugador)
{
	if (jugador[J1].posicion.fil < MAX_FILAS - 1) {
		jugador[J1].posicion.fil++;
	}
}

void mover_izquierda_j1(jugador_t *jugador)
{
	if (jugador[J1].posicion.col > 0) {
		jugador[J1].posicion.col--;
	}
}

void mover_derecha_j1(jugador_t *jugador)
{
	if (jugador[J1].posicion.col < MAX_COLUMNAS - 1) {
		jugador[J1].posicion.col++;
	}
}

void mover_arriba_j2(jugador_t *jugador)
{
	if (jugador[J2].posicion.fil > 0) {
		jugador[J2].posicion.fil--;
	}
}

void mover_abajo_j2(jugador_t *jugador)
{
	if (jugador[J2].posicion.fil < MAX_FILAS - 1) {
		jugador[J2].posicion.fil++;
	}
}

void mover_izquierda_j2(jugador_t *jugador)
{
	if (jugador[J2].posicion.col > 0) {
		jugador[J2].posicion.col--;
	}
}

void mover_derecha_j2(jugador_t *jugador)
{
	if (jugador[J2].posicion.col < MAX_COLUMNAS - 1) {
		jugador[J2].posicion.col++;
	}
}

struct accion_jugador {
	accion_t fn;
};
struct accion_jugador *crear_accion(accion_t accion)
{
	struct accion_jugador *accion_jugador =
		malloc(sizeof(struct accion_jugador));
	if (!accion_jugador)
		return NULL;
	accion_jugador->fn = accion;
	return accion_jugador;
}

bool inicializar_acciones(juego_t *juego)
{
	juego->hash_acciones = hash_crear(10);
	if (!juego->hash_acciones) {
		printf("Error al crear la lista de acciones.\n");
		return false;
	}

	int aciertos = 0;
	aciertos += hash_insertar(juego->hash_acciones, "w",
				  crear_accion(mover_arriba_j1), NULL);
	aciertos += hash_insertar(juego->hash_acciones, "s",
				  crear_accion(mover_abajo_j1), NULL);
	aciertos += hash_insertar(juego->hash_acciones, "a",
				  crear_accion(mover_izquierda_j1), NULL);
	aciertos += hash_insertar(juego->hash_acciones, "d",
				  crear_accion(mover_derecha_j1), NULL);
	aciertos += hash_insertar(juego->hash_acciones, "arriba",
				  crear_accion(mover_arriba_j2), NULL);
	aciertos += hash_insertar(juego->hash_acciones, "abajo",
				  crear_accion(mover_abajo_j2), NULL);
	aciertos += hash_insertar(juego->hash_acciones, "izquierda",
				  crear_accion(mover_izquierda_j2), NULL);
	aciertos += hash_insertar(juego->hash_acciones, "derecha",
				  crear_accion(mover_derecha_j2), NULL);

	if (aciertos != 8) {
		printf("Error al inicializar las acciones del juego.\n");
		return false;
	}

	return true;
}

/* 
 * Devuelve una coordenada dentro de los límites definidos
*/
coordenada_t coord_aleatoria(void)
{
	coordenada_t c = { .fil = rand() % MAX_FILAS,
			   .col = rand() % MAX_COLUMNAS };
	return c;
}

bool inicializar_jugadores(jugador_t jugadores[CANT_JUGADORES])
{
	bool acerto = true;
	for (size_t i = 0; i < CANT_JUGADORES; i++) {
		jugadores[i].posicion = coord_aleatoria();
		jugadores[i].pkm_capturados = pila_crear();
		jugadores[i].pkm_por_capturar = pila_crear();
		jugadores[i].puntaje = PUNTAJE_INICIAL;
		if (!jugadores[i].pkm_capturados ||
		    !jugadores[i].pkm_por_capturar) {
			printf("Error al crear los recursos del jugador %zu.\n",
			       i + 1);
			acerto = false;
		}
	}
	return acerto;
}

struct pokedex_iterar_ctx {
	juego_t *juego;
	int indice_ramdom;
	size_t no_capturados[CANT_JUGADORES];
	size_t contador;
	size_t limite;
	bool salir;
};

bool lista_agregar_pkm_salvaje(struct pokemon *poke, void *ctx_)
{
	struct pokedex_iterar_ctx *ctx = ctx_;
	if (lista_tamanio(
		    ctx->juego->pokemones_salvajes[J1].pokemones_salvajes) >=
		    ctx->limite &&
	    lista_tamanio(
		    ctx->juego->pokemones_salvajes[J2].pokemones_salvajes) >=
		    ctx->limite) {
		ctx->salir = true;
		return false;
	}

	if (ctx->contador != ctx->indice_ramdom) {
		ctx->contador++;
		return true;
	}

	pokemones_t *salvaje = malloc(sizeof(pokemones_t));
	if (!salvaje)
		return false;

	*salvaje = (pokemones_t){ .pokemon = *poke,
				  .posicion = coord_aleatoria(),
				  .capturado = { false, false },
				  .oculto = { true, true } };

	if (!lista_insertar(
		    ctx->juego->pokemones_salvajes[J1].pokemones_salvajes,
		    salvaje) ||
	    !lista_insertar(
		    ctx->juego->pokemones_salvajes[J2].pokemones_salvajes,
		    salvaje)) {
		free(salvaje);
		return false;
	}

	ctx->indice_ramdom =
		rand() % (int)(pokedex_cantidad_pokemones(ctx->juego->pokedex));
	ctx->contador = 0;
	return false;
}

void iniciar_juego(juego_t *juego)
{
	if (!inicializar_acciones(juego))
		return;

	juego->jugadores = malloc(CANT_JUGADORES * sizeof(jugador_t));
	if (!juego->jugadores) {
		printf("Error al crear los jugadores.\n");
		return;
	}

	if (!inicializar_jugadores(juego->jugadores))
		return;

	for (size_t i = 0; i < CANT_JUGADORES; i++) {
		juego->pokemones_salvajes[i].pokemones_salvajes = lista_crear();
		juego->pokemones_salvajes[i].siguiente_pkm_salvaje =
			cola_crear();
	}

	if (!juego->pokemones_salvajes[J1].pokemones_salvajes ||
	    !juego->pokemones_salvajes[J1].siguiente_pkm_salvaje) {
		printf("Error al crear los recursos del jugador J1.\n");
		return;
	}

	if (!juego->pokemones_salvajes[J2].pokemones_salvajes ||
	    !juego->pokemones_salvajes[J2].siguiente_pkm_salvaje) {
		printf("Error al crear los recursos del jugador J2.\n");
		return;
	}

	struct pokedex_iterar_ctx ctx = {
		.juego = juego,
		.indice_ramdom = rand() % (int)pokedex_cantidad_pokemones(
						  juego->pokedex),
		.no_capturados = { 0, 0 },
		.contador = 0,
		.limite = CANT_POKEMONES,
		.salir = false
	};

	while (!ctx.salir)
		pokedex_iterar_pokemones(juego->pokedex, ITERAR_ID,
					 lista_agregar_pkm_salvaje, &ctx);
}

bool comparar_coordenadas(coordenada_t a, coordenada_t b)
{
	return a.fil == b.fil && a.col == b.col;
}

void movimiento_jugadores(juego_t *juego, int accion)
{
	const char *accion_str = (const char *)&accion;
	if (accion >= TECLA_ARRIBA && accion <= TECLA_DERECHA) {
		switch (accion) {
		case TECLA_ARRIBA:
			accion_str = "arriba";
			break;
		case TECLA_ABAJO:
			accion_str = "abajo";
			break;
		case TECLA_IZQUIERDA:
			accion_str = "izquierda";
			break;
		case TECLA_DERECHA:
			accion_str = "derecha";
			break;
		}
	}
	struct accion_jugador *dato =
		hash_buscar(juego->hash_acciones, accion_str);
	if (dato)
		dato->fn(juego->jugadores);
}

void desapilar_pokemon_por_capturar_idx(jugador_t *jugadores, int jugador)
{
	pokemones_t *por_capturar =
		pila_tope(jugadores[jugador].pkm_por_capturar);

	pokemones_t *ultimo_cap = pila_tope(jugadores[jugador].pkm_capturados);

	if (ultimo_cap && por_capturar &&
	    ultimo_cap->pokemon.tipo == por_capturar->pokemon.tipo) {
		pila_desapilar(jugadores[jugador].pkm_por_capturar);
		jugadores[jugador].puntaje++;
	}
}

void desapilar_pokemon_por_capturar_j1(jugador_t *jugadores)
{
	desapilar_pokemon_por_capturar_idx(jugadores, J1);
}

void desapilar_pokemon_por_capturar_j2(jugador_t *jugadores)
{
	desapilar_pokemon_por_capturar_idx(jugadores, J2);
}

bool jugador_captura_pokemon(void *dato, void *ctx, int jugador, int rival)
{
	pokemones_t *pkm = dato;
	jugador_t *jugadores = ctx;

	if (!pkm->capturado[jugador] &&
	    comparar_coordenadas(pkm->posicion, jugadores[jugador].posicion) &&
	    !pkm->oculto[jugador]) {
		pkm->capturado[jugador] = true;
		pila_apilar(jugadores[jugador].pkm_capturados, pkm);
		pila_apilar(jugadores[rival].pkm_por_capturar, pkm);

		jugadores[rival].puntaje--;

		if (jugador == J1) {
			desapilar_pokemon_por_capturar_j1(jugadores);
		} else {
			desapilar_pokemon_por_capturar_j2(jugadores);
		}
	}
	return true;
}

bool jugador_captura_pokemon_j1(void *dato, void *ctx)
{
	return jugador_captura_pokemon(dato, ctx, J1, J2);
}

bool jugador_captura_pokemon_j2(void *dato, void *ctx)
{
	return jugador_captura_pokemon(dato, ctx, J2, J1);
}

void realizar_jugada(juego_t *juego, int accion)
{
	movimiento_jugadores(juego, accion);
	lista_iterar(juego->pokemones_salvajes[J1].pokemones_salvajes,
		     jugador_captura_pokemon_j1, juego->jugadores);
	lista_iterar(juego->pokemones_salvajes[J2].pokemones_salvajes,
		     jugador_captura_pokemon_j2, juego->jugadores);
}

bool rellenar_pkm_salvaje(int jugador, int rival, struct pokemon *poke,
			  struct pokedex_iterar_ctx *ctx)
{
	if (ctx->no_capturados[jugador] >= ctx->limite) {
		ctx->salir = true;
		return false;
	}

	if (!cola_vacia(ctx->juego->pokemones_salvajes[jugador]
				.siguiente_pkm_salvaje)) {
		pokemones_t *salvaje =
			cola_desencolar(ctx->juego->pokemones_salvajes[jugador]
						.siguiente_pkm_salvaje);
		lista_insertar(ctx->juego->pokemones_salvajes[jugador]
				       .pokemones_salvajes,
			       salvaje);
		return true;
	}

	if (ctx->contador != ctx->indice_ramdom) {
		ctx->contador++;
		return true;
	}

	pokemones_t *salvaje = malloc(sizeof *salvaje);
	if (!salvaje)
		return false;

	*salvaje = (pokemones_t){
		.pokemon = *poke,
		.posicion = coord_aleatoria(),
		.capturado = { false, false },
	};
	salvaje->oculto[jugador] = false;
	salvaje->oculto[rival] = true;

	if (!lista_insertar(
		    ctx->juego->pokemones_salvajes[jugador].pokemones_salvajes,
		    salvaje) ||
	    !cola_encolar(
		    ctx->juego->pokemones_salvajes[rival].siguiente_pkm_salvaje,
		    salvaje)) {
		free(salvaje);
		return false;
	}

	ctx->indice_ramdom =
		rand() % (int)pokedex_cantidad_pokemones(ctx->juego->pokedex);
	ctx->contador = 0;
	ctx->no_capturados[jugador]++;
	return false;
}

bool lista_rellenar_pkm_salvaje_j1(struct pokemon *poke, void *ctx_)
{
	return rellenar_pkm_salvaje(J1, J2, poke, ctx_);
}

bool lista_rellenar_pkm_salvaje_j2(struct pokemon *poke, void *ctx_)
{
	return rellenar_pkm_salvaje(J2, J1, poke, ctx_);
}

bool pokemon_a_terreno(int j, pokemones_t *salvaje, terreno_t *ctx)
{
	if (ctx->contador[j] == ctx->limite) {
		ctx->contador[j] = 0;
		return false;
	}

	coordenada_t p = salvaje->posicion;

	if (!salvaje->capturado[j] && ctx->contador[j] < ctx->limite) {
		ctx->terreno[j][p.fil][p.col].inicial =
			salvaje->pokemon.nombre[0];
		ctx->terreno[j][p.fil][p.col].tipo = salvaje->pokemon.tipo;
		salvaje->oculto[j] = false;
		ctx->contador[j]++;
	} else {
		ctx->terreno[j][p.fil][p.col].inicial = '.';
		salvaje->oculto[j] = true;
	}
	return true;
}

bool pokemon_a_terreno_j1(void *dato, void *ctx_)
{
	return pokemon_a_terreno(J1, dato, ctx_);
}

bool pokemon_a_terreno_j2(void *dato, void *ctx_)
{
	return pokemon_a_terreno(J2, dato, ctx_);
}

typedef struct contar {
	size_t *contador;
	int jugador;
} contar_ctx_t;

bool contar_no_capturados(void *dato, void *ctx_)
{
	pokemones_t *salvaje = dato;
	contar_ctx_t *ctx = ctx_;

	if (!salvaje->capturado[ctx->jugador])
		(*ctx->contador)++;

	return true;
}

void rellenar_terreno_de_pokemones(juego_t *juego, terreno_t terreno)
{
	size_t sin_capturar_j1 = 0;
	contar_ctx_t ctx_j1 = { .contador = &sin_capturar_j1, .jugador = J1 };
	lista_iterar(juego->pokemones_salvajes[J1].pokemones_salvajes,
		     contar_no_capturados, &ctx_j1);

	size_t sin_capturar_j2 = 0;
	contar_ctx_t ctx_j2 = { .contador = &sin_capturar_j2, .jugador = J2 };
	lista_iterar(juego->pokemones_salvajes[J2].pokemones_salvajes,
		     contar_no_capturados, &ctx_j2);

	struct pokedex_iterar_ctx info_pkm = {
		.juego = juego,
		.indice_ramdom = rand() % (int)pokedex_cantidad_pokemones(
						  juego->pokedex),
		.no_capturados = { sin_capturar_j1, sin_capturar_j2 },
		.contador = 0,
		.limite = CANT_POKEMONES,
		.salir = false
	};

	while (!info_pkm.salir)
		pokedex_iterar_pokemones(juego->pokedex, ITERAR_ID,
					 lista_rellenar_pkm_salvaje_j1,
					 &info_pkm);

	info_pkm.salir = false;
	while (!info_pkm.salir)
		pokedex_iterar_pokemones(juego->pokedex, ITERAR_ID,
					 lista_rellenar_pkm_salvaje_j2,
					 &info_pkm);

	lista_iterar(juego->pokemones_salvajes[J1].pokemones_salvajes,
		     pokemon_a_terreno_j1, &terreno);
	lista_iterar(juego->pokemones_salvajes[J2].pokemones_salvajes,
		     pokemon_a_terreno_j2, &terreno);
}

terreno_t
rellenar_terreno(juego_t *juego,
		 celda_t celdas[CANT_JUGADORES][MAX_FILAS][MAX_COLUMNAS])
{
	for (int i = 0; i < MAX_FILAS; i++) {
		for (int j = 0; j < MAX_COLUMNAS; j++) {
			celdas[J1][i][j].inicial = '.';
			celdas[J1][i][j].tipo = -1;
			celdas[J2][i][j].inicial = '.';
			celdas[J2][i][j].tipo = -1;
		}
	}

	terreno_t ctx = { .terreno[J1] = (celda_t(*)[MAX_COLUMNAS])celdas[J1],
			  .terreno[J2] = (celda_t(*)[MAX_COLUMNAS])celdas[J2],
			  .contador = { 0, 0 },
			  .limite = CANT_POKEMONES };

	rellenar_terreno_de_pokemones(juego, ctx);

	ctx.terreno[J1][juego->jugadores[J1].posicion.fil]
		   [juego->jugadores[J1].posicion.col]
			   .inicial = CARACTER_J1;
	ctx.terreno[J2][juego->jugadores[J2].posicion.fil]
		   [juego->jugadores[J2].posicion.col]
			   .inicial = CARACTER_J2;

	return ctx;
}

void mostrar_juego(juego_t *juego)
{
	celda_t celdas[CANT_JUGADORES][MAX_FILAS][MAX_COLUMNAS];
	terreno_t ctx = rellenar_terreno(juego, celdas);
	imprimir_terreno(ctx, juego->semilla, juego->iteraciones_restantes);
	imprimir_estado_de_juego(juego);
}

bool liberar_pokemon(void *dato, void *ctx)
{
	pokemones_t *salvaje = dato;
	free(salvaje);
	return true;
}

void destruir_accion(void *dato)
{
	if (!dato)
		return;
	struct accion_jugador *accion = dato;
	free(accion);
}

void destruir_juego(juego_t *juego)
{
	if (!juego)
		return;

	for (size_t i = 0; i < CANT_JUGADORES; i++) {
		pila_destruir(juego->jugadores[i].pkm_capturados);
		pila_destruir(juego->jugadores[i].pkm_por_capturar);
	}
	free(juego->jugadores);

	hash_destruir_todo(juego->hash_acciones, destruir_accion);

	bool colas_vacias = false;
	while (!colas_vacias) {
		pokemones_t *pkm_j1 = cola_desencolar(
			juego->pokemones_salvajes[J1].siguiente_pkm_salvaje);
		pokemones_t *pkm_j2 = cola_desencolar(
			juego->pokemones_salvajes[J2].siguiente_pkm_salvaje);
		if (pkm_j1)
			lista_insertar(
				juego->pokemones_salvajes[J1].pokemones_salvajes,
				pkm_j1);
		if (pkm_j2)
			lista_insertar(
				juego->pokemones_salvajes[J2].pokemones_salvajes,
				pkm_j2);
		if (!pkm_j1 && !pkm_j2)
			colas_vacias = true;
	}

	lista_iterar(juego->pokemones_salvajes[J1].pokemones_salvajes,
		     liberar_pokemon, NULL);

	for (size_t i = 0; i < CANT_JUGADORES; i++) {
		lista_destruir(juego->pokemones_salvajes[i].pokemones_salvajes);
		cola_destruir(
			juego->pokemones_salvajes[i].siguiente_pkm_salvaje);
	}
}