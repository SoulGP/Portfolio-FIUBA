#include "juego.h"
#include "juego_imprimir.h"

/*
 * Mueve al jugador J1 hacia arriba si no está en el borde superior del terreno.
 */
void mover_arriba_j1(jugador_t *jugador)
{
	if (jugador[J1].posicion.fil > 0) {
		jugador[J1].posicion.fil--;
	}
}

/**
 * Mueve al jugador J1 hacia abajo si no está en el borde inferior del terreno.
 */
void mover_abajo_j1(jugador_t *jugador)
{
	if (jugador[J1].posicion.fil < MAX_FILAS - 1) {
		jugador[J1].posicion.fil++;
	}
}

/**
 * Mueve al jugador J1 hacia la izquierda si no está en el borde izquierdo del terreno.
 */
void mover_izquierda_j1(jugador_t *jugador)
{
	if (jugador[J1].posicion.col > 0) {
		jugador[J1].posicion.col--;
	}
}

/**
 * Mueve al jugador J1 hacia la derecha si no está en el borde derecho del terreno.
 */
void mover_derecha_j1(jugador_t *jugador)
{
	if (jugador[J1].posicion.col < MAX_COLUMNAS - 1) {
		jugador[J1].posicion.col++;
	}
}

/**
 * Mueve al jugador J2 hacia arriba si no está en el borde superior del terreno.
 */
void mover_arriba_j2(jugador_t *jugador)
{
	if (jugador[J2].posicion.fil > 0) {
		jugador[J2].posicion.fil--;
	}
}

/**
 * Mueve al jugador J2 hacia abajo si no está en el borde inferior del terreno.
 */
void mover_abajo_j2(jugador_t *jugador)
{
	if (jugador[J2].posicion.fil < MAX_FILAS - 1) {
		jugador[J2].posicion.fil++;
	}
}

/**
 * Mueve al jugador J2 a la izquierda si no está en el borde izquierdo del terreno.
 */
void mover_izquierda_j2(jugador_t *jugador)
{
	if (jugador[J2].posicion.col > 0) {
		jugador[J2].posicion.col--;
	}
}

/**
 * Mueve al jugador J2 a la derecha si no está en el borde derecho del terreno.
 */
void mover_derecha_j2(jugador_t *jugador)
{
	if (jugador[J2].posicion.col < MAX_COLUMNAS - 1) {
		jugador[J2].posicion.col++;
	}
}

struct accion_jugador {
	accion_t fn;
};

/**
 * Crea una acción del jugador y la inicializa con la función indicada.
 * Devuelve un puntero a la acción creada o NULL en caso de error.
 */
struct accion_jugador *crear_accion(accion_t accion)
{
	struct accion_jugador *accion_jugador =
		malloc(sizeof(struct accion_jugador));
	if (!accion_jugador)
		return NULL;
	accion_jugador->fn = accion;
	return accion_jugador;
}

/**
 * Inicializa las acciones del juego y las inserta en el hash.
 * Devuelve true si se inicializaron correctamente, false en caso contrario.
 */
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

/**
 * Inicializa los jugadores con posiciones aleatorias y recursos necesarios.
 * Devuelve true si se inicializaron correctamente, false en caso contrario.
 */
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

/**
 * Verifica si las listas de Pokémon salvajes de ambos jugadores han alcanzado
 * el límite establecido.
 * Si ambas listas están llenas, establece salir a true.
 */
bool listas_al_limite(struct pokedex_iterar_ctx *ctx)
{
	bool j1_lleno = lista_tamanio(ctx->juego->pokemones_salvajes[J1]
					      .pokemones_salvajes) >=
			ctx->limite;

	bool j2_lleno = lista_tamanio(ctx->juego->pokemones_salvajes[J2]
					      .pokemones_salvajes) >=
			ctx->limite;

	if (j1_lleno && j2_lleno) {
		ctx->salir = true;
		return true;
	}
	return false;
}

/**
 * Inserta un Pokémon salvaje en las listas de ambos jugadores.
 */
bool insertar_en_listas(juego_t *juego, pokemones_t *s)
{
	bool ok_j1 = lista_insertar(
		juego->pokemones_salvajes[J1].pokemones_salvajes, s);

	bool ok_j2 = lista_insertar(
		juego->pokemones_salvajes[J2].pokemones_salvajes, s);

	return ok_j1 && ok_j2;
}

/**
 * Agrega un Pokémon salvaje a la lista de Pokémon salvajes del jugador.
 */
bool lista_agregar_pkm_salvaje(struct pokemon *poke, void *ctx_)
{
	struct pokedex_iterar_ctx *ctx = ctx_;
	if (listas_al_limite(ctx))
		return false;

	if (ctx->contador != ctx->indice_ramdom) {
		ctx->contador++;
		return true;
	}

	pokemones_t *salvaje = malloc(sizeof(pokemones_t));
	if (!salvaje)
		return false;

	*salvaje = (pokemones_t){ .pokemon = *poke,
				  .posicion = coord_aleatoria(),
				  .capturado = { false, false } };

	if (!insertar_en_listas(ctx->juego, salvaje)) {
		free(salvaje);
		return false;
	}

	ctx->indice_ramdom =
		rand() % (int)(pokedex_cantidad_pokemones(ctx->juego->pokedex));
	ctx->contador = 0;
	return false;
}

/**
 * Crea los recursos necesarios para los Pokémon salvajes de ambos jugadores.
 * Devuelve true si se crearon correctamente, false en caso contrario.
 */
bool crear_recursos_salvajes(juego_t *juego)
{
	for (size_t i = 0; i < CANT_JUGADORES; ++i) {
		juego->pokemones_salvajes[i].pokemones_salvajes = lista_crear();
		juego->pokemones_salvajes[i].siguiente_pkm_salvaje =
			cola_crear();
	}

	if (!juego->pokemones_salvajes[J1].pokemones_salvajes ||
	    !juego->pokemones_salvajes[J1].siguiente_pkm_salvaje) {
		printf("Error al crear los recursos del jugador J1.\n");
		return false;
	}
	if (!juego->pokemones_salvajes[J2].pokemones_salvajes ||
	    !juego->pokemones_salvajes[J2].siguiente_pkm_salvaje) {
		printf("Error al crear los recursos del jugador J2.\n");
		return false;
	}
	return true;
}

void juego_inicializar(juego_t *juego)
{
	if (!juego || !juego->pokedex)
		return;

	if (!inicializar_acciones(juego))
		return;

	juego->jugadores = malloc(CANT_JUGADORES * sizeof(jugador_t));
	if (!juego->jugadores) {
		printf("Error al crear los jugadores.\n");
		return;
	}

	if (!inicializar_jugadores(juego->jugadores))
		return;

	if (!crear_recursos_salvajes(juego))
		return;

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

/**
 * Compara dos coordenadas.
 * Devuelve true si son iguales, false en caso contrario.
 */
bool comparar_coordenadas(coordenada_t a, coordenada_t b)
{
	return a.fil == b.fil && a.col == b.col;
}

/**
 * Mueve a los jugadores según la acción indicada.
 * Si la acción es una tecla de movimiento, se llama a la función correspondiente
 * para mover al jugador J1 o J2.
 */
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

/**
 * Desapila el Pokémon por capturar del jugador indicado si coincide con el
 * último Pokémon capturado.
 * Si coincide, aumenta el puntaje del jugador.
 */
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

/**
 * Desapila el Pokémon por capturar del jugador J1 si es que coincide con el
 * último Pokémon capturado.
 */
void desapilar_pokemon_por_capturar_j1(jugador_t *jugadores)
{
	desapilar_pokemon_por_capturar_idx(jugadores, J1);
}

/**
 * Desapila el Pokémon por capturar del jugador J2 si es que coincide con el
 * último Pokémon capturado.
 */
void desapilar_pokemon_por_capturar_j2(jugador_t *jugadores)
{
	desapilar_pokemon_por_capturar_idx(jugadores, J2);
}

/**
 * Si la posición del Pokémon coincide con la del jugador,
 * captura el Pokémon y lo agrega a la pila de Pokémon capturados del jugador.
 *
 * Además, lo agrega a la pila de Pokémon por capturar del rival, y disminuye
 * su puntaje.
 */
bool jugador_captura_pokemon(void *dato, void *ctx, int jugador, int rival)
{
	pokemones_t *pkm = dato;
	jugador_t *jugadores = ctx;

	if (!pkm->capturado[jugador] &&
	    comparar_coordenadas(pkm->posicion, jugadores[jugador].posicion)) {
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

/**
 * permite al jugador J1 capturar Pokémon salvajes.
 */
bool jugador_captura_pokemon_j1(void *dato, void *ctx)
{
	return jugador_captura_pokemon(dato, ctx, J1, J2);
}

/**
 * permite al jugador J2 capturar Pokémon salvajes.
 */
bool jugador_captura_pokemon_j2(void *dato, void *ctx)
{
	return jugador_captura_pokemon(dato, ctx, J2, J1);
}

void realizar_jugada(juego_t *juego, int accion)
{
	if (!juego || !juego->hash_acciones || !juego->jugadores ||
	    !juego->pokemones_salvajes[J1].pokemones_salvajes ||
	    !juego->pokemones_salvajes[J2].pokemones_salvajes) {
		printf("Juego no inicializado correctamente.\n");
		return;
	}
	movimiento_jugadores(juego, accion);
	lista_iterar(juego->pokemones_salvajes[J1].pokemones_salvajes,
		     jugador_captura_pokemon_j1, juego->jugadores);
	lista_iterar(juego->pokemones_salvajes[J2].pokemones_salvajes,
		     jugador_captura_pokemon_j2, juego->jugadores);
}

/**
 * Rellena la lista de Pokémon salvajes del jugador con el siguiente Pokémon
 * de la cola. Si la cola está vacía, no hace nada y devuelve false.
 * Si se agrega un Pokémon, devuelve true.
 */
bool rellenar_con_cola(juego_t *juego, int jugador)
{
	cola_t *cola = juego->pokemones_salvajes[jugador].siguiente_pkm_salvaje;

	if (!cola_vacia(cola)) {
		pokemones_t *salvaje = cola_desencolar(cola);
		lista_insertar(
			juego->pokemones_salvajes[jugador].pokemones_salvajes,
			salvaje);
		return true;
	}
	return false;
}

/**
 * Crea un Pokémon salvaje y lo inserta en la lista de Pokémon salvajes del
 * jugador y en la cola del rival.
 * Si no se puede crear o insertar, libera la memoria y devuelve false.
 */
bool crear_e_insertar_pkm_salvaje(juego_t *juego, int jugador, int rival,
				  struct pokemon *poke,
				  struct pokedex_iterar_ctx *ctx)
{
	pokemones_t *salvaje = malloc(sizeof *salvaje);
	if (!salvaje)
		return false;

	*salvaje = (pokemones_t){
		.pokemon = *poke,
		.posicion = coord_aleatoria(),
		.capturado = { false, false },
	};

	bool ok_lista = lista_insertar(
		juego->pokemones_salvajes[jugador].pokemones_salvajes, salvaje);

	bool ok_cola = cola_encolar(
		juego->pokemones_salvajes[rival].siguiente_pkm_salvaje,
		salvaje);

	if (!ok_lista || !ok_cola) {
		free(salvaje);
		return false;
	}

	ctx->indice_ramdom =
		rand() % (int)pokedex_cantidad_pokemones(juego->pokedex);
	ctx->contador = 0;
	ctx->no_capturados[jugador]++;
	return false;
}

/**
 * Rellena la lista de Pokémon salvajes de un jugador.
 * Si el número de Pokémon capturados alcanza el límite, se detiene el proceso.
 * Si no hay más Pokémon en la cola, se crea uno nuevo y se agrega a la lista
 * de Pokémon salvajes del jugador y a la cola del rival.
 */
bool rellenar_pkm_salvaje(int jugador, int rival, struct pokemon *poke,
			  struct pokedex_iterar_ctx *ctx)
{
	if (ctx->no_capturados[jugador] >= ctx->limite) {
		ctx->salir = true;
		return false;
	}

	if (rellenar_con_cola(ctx->juego, jugador))
		return false;

	if (ctx->contador != ctx->indice_ramdom) {
		ctx->contador++;
		return true;
	}

	return crear_e_insertar_pkm_salvaje(ctx->juego, jugador, rival, poke,
					    ctx);
}

/**
 * Rellena la lista de Pokémon salvajes del jugador J1.
 */
bool lista_rellenar_pkm_salvaje_j1(struct pokemon *poke, void *ctx_)
{
	return rellenar_pkm_salvaje(J1, J2, poke, ctx_);
}

/**
 * Rellena la lista de Pokémon salvajes del jugador J2.
 */
bool lista_rellenar_pkm_salvaje_j2(struct pokemon *poke, void *ctx_)
{
	return rellenar_pkm_salvaje(J2, J1, poke, ctx_);
}

/**
 * Coloca un Pokémon salvaje en el terreno.
 * Si el Pokémon ya ha sido capturado o se ha alcanzado el límite de Pokémon
 * salvajes, no se coloca nada en la celda.
 */
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
		ctx->contador[j]++;
	} else {
		ctx->terreno[j][p.fil][p.col].inicial = '.';
	}
	return true;
}

/**
 * Coloca los Pokémon salvajes del jugador J1 en el terreno.
 */
bool pokemon_a_terreno_j1(void *dato, void *ctx_)
{
	return pokemon_a_terreno(J1, dato, ctx_);
}

/**
 * Coloca los Pokémon salvajes del jugador J2 en el terreno.
 */
bool pokemon_a_terreno_j2(void *dato, void *ctx_)
{
	return pokemon_a_terreno(J2, dato, ctx_);
}

typedef struct contar {
	size_t *contador;
	int jugador;
} contar_ctx_t;

/**
 * Cuenta los Pokémon salvajes que no han sido capturados.
 */
bool contar_no_capturados(void *dato, void *ctx_)
{
	pokemones_t *salvaje = dato;
	contar_ctx_t *ctx = ctx_;

	if (!salvaje->capturado[ctx->jugador])
		(*ctx->contador)++;

	return true;
}

/**
 * Cuenta los Pokémon salvajes que no han sido capturados por un jugador.
 */
size_t contar_no_capturados_jugador(const juego_t *juego, int jugador)
{
	size_t sin_capturar = 0;
	contar_ctx_t ctx = { .contador = &sin_capturar, .jugador = jugador };

	lista_iterar(juego->pokemones_salvajes[jugador].pokemones_salvajes,
		     contar_no_capturados, &ctx);

	return sin_capturar;
}

/**
 * Rellena el terreno con Pokémon salvajes de ambos jugadores.
 * Utiliza la pokedex para obtener los Pokémon y los coloca en las colas
 * correspondientes.
 */
void rellenar_terreno_de_pokemones(juego_t *juego, terreno_t terreno)
{
	size_t sin_capturar[J2 + 1];
	sin_capturar[J1] = contar_no_capturados_jugador(juego, J1);
	sin_capturar[J2] = contar_no_capturados_jugador(juego, J2);

	struct pokedex_iterar_ctx info_pkm = {
		.juego = juego,
		.indice_ramdom = rand() % (int)pokedex_cantidad_pokemones(
						  juego->pokedex),
		.no_capturados = { sin_capturar[J1], sin_capturar[J2] },
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

/**
 * Rellena el terreno con las celdas iniciales y los Pokémon salvajes.
 * Devuelve un contexto que contiene el terreno.
 */
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

void juego_mostrar(juego_t juego)
{
	if (!juego.pokedex || !juego.jugadores) {
		printf("Juego no inicializado correctamente.\n");
		return;
	}
	celda_t celdas[CANT_JUGADORES][MAX_FILAS][MAX_COLUMNAS];
	terreno_t ctx = rellenar_terreno(&juego, celdas);
	imprimir_terreno(ctx, juego.semilla, juego.iteraciones_restantes);
	imprimir_estado_de_juego(juego);
}

int juego_estado(juego_t juego)
{
	if (!juego.jugadores) {
		printf("Juego no inicializado correctamente.\n");
		return 1;
	}
	if (juego.jugadores[J1].puntaje == 0 ||
	    juego.jugadores[J2].puntaje == 0)
		return 1;
	return juego.iteraciones_restantes == 0;
}

/**
 * destruye un Pokémon salvaje.
 */
bool destruir_pokemon(void *dato, void *ctx)
{
	pokemones_t *salvaje = dato;
	free(salvaje);
	return true;
}

/**
 * Destruye una acción del jugador.
 */
void destruir_accion(void *dato)
{
	if (!dato)
		return;
	struct accion_jugador *accion = dato;
	free(accion);
}

/**
 * Destruye los recursos de los jugadores.
 */
void destruir_jugadores(juego_t *juego)
{
	for (size_t i = 0; i < CANT_JUGADORES; ++i) {
		pila_destruir(juego->jugadores[i].pkm_capturados);
		pila_destruir(juego->jugadores[i].pkm_por_capturar);
	}
	free(juego->jugadores);
}

/**
 * Vacía las colas de Pokémon salvajes de ambos jugadores.
 */
void vaciar_colas_pkm(juego_t *juego)
{
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
}

/**
 * Destruye todos los Pokémon salvajes para un jugador.
 */
void destruir_pkm_salvajes_de_jugador(struct pokemones_salvajes *salvajes)
{
	lista_destruir(salvajes->pokemones_salvajes);
	cola_destruir(salvajes->siguiente_pkm_salvaje);
}

void juego_destruir(juego_t *juego)
{
	if (!juego || !juego->pokedex)
		return;

	destruir_jugadores(juego);

	hash_destruir_todo(juego->hash_acciones, destruir_accion);

	vaciar_colas_pkm(juego);

	lista_iterar(juego->pokemones_salvajes[J1].pokemones_salvajes,
		     destruir_pokemon, NULL);
	for (size_t i = 0; i < CANT_JUGADORES; ++i)
		destruir_pkm_salvajes_de_jugador(&juego->pokemones_salvajes[i]);
}