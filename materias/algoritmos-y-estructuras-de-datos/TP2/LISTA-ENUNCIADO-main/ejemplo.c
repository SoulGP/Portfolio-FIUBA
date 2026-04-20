#include <stdlib.h>
#include <stdio.h>
#include "src/lista.h"
#include "src/cola.h"
#include "src/pila.h"

bool mostrar_int(void *elemento, void *contexto)
{
	int *posicion = contexto;

	int *n = (int *)elemento;
	printf("Elemento en posicion %i: %i\n", *posicion, *n);
	(*posicion)++;
	return true;
}

bool es_igual_a_contexto(void *elemento, void *contexto)
{
	if (!elemento || !contexto)
		return false;

	int *valor = elemento;
	int *objetivo = contexto;

	return *valor == *objetivo;
}

int main(int argc, char *argv[])
{
	//Este es el ejemplo que armamos en clase
	//Notar que no incluye ejemplos de cola y pila
	//Si la implementación es correcta, se deberían mostrar correctamente 3 veces
	//todos los elementos de la lista en el orden en que fueron insertados

	int uno = 1;
	int dos = 2;
	int tres = 3;

	lista_t *l = lista_crear();

	lista_insertar(l, &uno);
	lista_insertar(l, &dos);
	lista_insertar(l, &tres);

	int objetivo = 2;
	printf("Busco si hay un \"2\" en la lista\n");
	int *resultado = lista_buscar(l, es_igual_a_contexto, &objetivo);
	printf("se encontró: %i\n\n", *resultado);

	printf("Muestro los elementos usando el iterador interno:\n");
	int posicion = 0;
	int cantidad_iterados = lista_iterar(l, mostrar_int, &posicion);
	printf("cantidad de iterados: %i\n", cantidad_iterados);

	printf("\nMuestro los elementos usando lista_obtener_elemento:\n");
	for (int i = 0; i < lista_tamanio(l); i++) {
		int *n = lista_obtener_elemento(l, i);
		printf("Elemento en posicion %i: %i\n", i, *n);
	}

	int posicion_a_sacar = 2;
	printf("\nSaco el elemento %i:\n", posicion_a_sacar);
	int *n = lista_sacar_de_posicion(l, posicion_a_sacar);
	if (n == NULL) {
		printf("no hay nada.\n");
		lista_destruir(l);
		return -1;
	}
	printf("\nMuestro el elemento sacado: %i\n", *n);

	printf("\nMuestro los elementos usando lista_obtener_elemento tras sacar un elemento:\n");
	for (int i = 0; i < lista_tamanio(l); i++) {
		n = lista_obtener_elemento(l, i);
		printf("Elemento en posicion %i: %i\n", i, *n);
	}

	int cuatro = 4;
	int posicion_a_insertar = 1;
	if (!lista_insertar_en_posicion(l, posicion_a_insertar, &cuatro)) {
		printf("No se pudo insertar el elemento.\n");
	}
	printf("\nMuestro los elementos usando lista_obtener_elemento tras insertar un elemento:\n");
	for (int i = 0; i < lista_tamanio(l); i++) {
		n = lista_obtener_elemento(l, i);
		printf("Elemento en posicion %i: %i\n", i, *n);
	}

	printf("el elemento 4 esta en la posición: %i\n",
	       lista_buscar_posicion(l, &cuatro));

	int elemento_a_sacar = 4;
	printf("\nSaco el elemento %i:\n", elemento_a_sacar);
	n = lista_sacar_elemento(l, &elemento_a_sacar);
	if (n == NULL) {
		printf("no hay nada.\n");
		lista_destruir(l);
		return -1;
	}
	printf("\nMuestro el elemento sacado: %i\n", *n);

	printf("\nMuestro los elementos usando lista_obtener_elemento tras sacar un elemento:\n");
	for (int i = 0; i < lista_tamanio(l); i++) {
		n = lista_obtener_elemento(l, i);
		printf("Elemento en posicion %i: %i\n", i, *n);
	}

	printf("\nMuestro los elementos usando el iterador externo:\n");
	posicion = 0;
	lista_iterador_t *li;
	for (li = lista_iterador_crear(l);
	     lista_iterador_quedan_elementos_por_recorrer(li);
	     lista_iterador_proxima_iteracion(li)) {
		int *n = lista_iterador_obtener_elemento(li);
		printf("Elemento en posicion %i: %i\n", posicion, *n);
		posicion++;
	}
	lista_iterador_destruir(li);

	lista_destruir(l);
	return 0;
}
