#ifndef MENU_H
#define MENU_H

#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef void (*opcion)(void *ctx);

typedef struct opcion_menu {
	const char *descripcion;
	opcion funcion;
} opcion_menu_t;

typedef struct menu menu_t;

/**
 * Crea un menú con una capacidad inicial.
 * 
 * Si la capacidad inicial es menor a 3, se usa una capacidad mínima.
 * 
 * Devuelve un puntero al menú creado o NULL en caso de error.
 */
menu_t *menu_crear(size_t capacidad_inicial);

/**
 * Agrega una opción al menú.
 * 
 * Devuelve true si se pudo agregar la opción, false en caso de error.
 */
bool menu_agregar_opcion(menu_t *menu, const char *inicial,
			 const char *descripcion, opcion funcion);

/**
 * Muestra el menú por stdout.
 * 
 * Si el menú está vacío, muestra un mensaje indicando que no hay opciones.
 */
void menu_mostrar(menu_t *menu);

/**
 * Ejecuta la opción del menú seleccionada por el usuario.
 */
void menu_ejecutar(menu_t *menu, void *ctx);

/**
 * Destruye el menú y libera la memoria reservada.
 */
void menu_destruir(menu_t *menu);

#endif // MENU_H
