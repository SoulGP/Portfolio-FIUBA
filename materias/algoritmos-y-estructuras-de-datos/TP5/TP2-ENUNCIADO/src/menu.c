#include "menu.h"

const char *duplicarstr(const char **destino, const char *clave)
{
	char *copia = malloc(strlen(clave) + 1);
	if (!copia)
		return NULL;

	strcpy(copia, clave);
	*destino = copia;
	return copia;
}

menu_t *menu_crear(size_t capacidad_inicial)
{
	hash_t *menu = hash_crear(capacidad_inicial);
	if (!menu) {
		return NULL;
	}

	return (menu_t *)menu;
}

bool menu_agregar_opcion(menu_t *menu, const char *inicial,
			 const char *descripcion, opcion funcion)
{
	if (!menu || !inicial || !descripcion || !funcion)
		return false;

	if (inicial[0] == '\0' || descripcion[0] == '\0' || strlen(inicial) > 1)
		return false;

	const char *cpy_descripcion =
		duplicarstr(&cpy_descripcion, descripcion);
	if (!cpy_descripcion)
		return false;

	opcion_menu_t *opcion = malloc(sizeof(*opcion));
	if (!opcion) {
		free((char *)cpy_descripcion);
		return false;
	}

	*opcion = (opcion_menu_t){ .descripcion = cpy_descripcion,
				   .funcion = funcion };

	return hash_insertar((hash_t *)menu, inicial, opcion, NULL);
}

/*
* Imprime las opciones disponibles del menú.
*/
bool printear_opciones(const char *clave, void *ctx)
{
	if (!clave || !ctx)
		return false;
	hash_t *hash = ctx;
	opcion_menu_t *opcion = hash_buscar(hash, clave);
	if (!opcion)
		return false;

	printf("%s - %s\n", clave, opcion->descripcion);
	return true;
}

void menu_mostrar(menu_t *menu)
{
	if (!menu || hash_tamanio((hash_t *)menu) == 0) {
		printf("No hay opciones disponibles en el menú.\n");
		return;
	}

	printf("Menú:\n");
	hash_iterar_claves((hash_t *)menu, printear_opciones, (hash_t *)menu);
}

void menu_ejecutar(menu_t *menu, void *ctx)
{
	menu_mostrar(menu);
	char buffer[100];
	printf("Seleccione una opción: ");
	if (!fgets(buffer, sizeof(buffer), stdin))
		return;

	char clave[2] = { (char)toupper(buffer[0]), '\0' };

	opcion_menu_t *op = hash_buscar((hash_t *)menu, clave);
	if (op && op->funcion)
		op->funcion(ctx);
}

/**
 * Libera la memoria reservada por una opción del menú.
 */
void liberar_opcion(void *opcion_)
{
	opcion_menu_t *opcion = (opcion_menu_t *)opcion_;
	if (!opcion)
		return;

	free((char *)opcion->descripcion);
	free(opcion);
}

void menu_destruir(menu_t *menu)
{
	if (!menu)
		return;

	hash_destruir_todo((hash_t *)menu, liberar_opcion);
}