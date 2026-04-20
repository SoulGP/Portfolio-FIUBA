<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TDA HASH

## Alumno: Diego Jose Fernandez Giraldo - 112571 - diego.j.fernandez.g@gmail.com

- Para compilar:

```bash
make all
```

- Para ejecutar los tests __sin__ valgrind:

```bash
make run
```

- Para ejecutar los tests __con__ valgrind:

```bash
make valgrind
```

## Mi estructura 

Mi struct ```hash_t``` se compone de 3 atributos y una struct auxiliar: 
- ```capacidad```: Indica el tamaño del vector ```elemento_t``` y evitar accesos a basura.
- ```cant_claves```: Indica cuantas claves hay almacenadas en el vector  ```elemento_t```, sirve para calcular el factor de carga para así poder expandir el vector.
- ```func_hash```: Almacena la función de hash usada para calcular el numero asociado a una clave.
- ```elemento_t```: Es una struct auxiliar que contiene los atributos ```clave```, ```valor``` y un atributo extra ```borrado``` utilizado para la implementación de _hash_sacar()_ ya que la facilita al solo tener que marcar ese espacio como "eliminado" y no tener que redistribuir el vector cada que se saca una clave a no ser que haya que rehashearla por superar el factor de carga.

<div align="center">
<img src="img/struct.drawio.svg">
</div>

---

## Complejidad funciones del hash.h

__hash_crear_con_funcion()__ ---> O(1), inicializa los campos del struct.

__hash_crear()__ ---> O(1), inicializa los campos del struct.

__hash_insertar()__ ---> O(n), en el peor de los casos que se tenga una mala función de hash habrá "n" colisiones hasta encontrar un espacio disponible para insertar una clave. En caso de tener una buena función de hash su complejidad tiende lo más cercano a O(1).

__hash_sacar()__ ---> O(n), en el peor de los casos que se tenga una mala función de hash se recorrerán "n" claves hasta llegar a la buscada y poder eliminarla. En caso de tener una buena función de hash su complejidad tiende lo más cercano a O(1).

__hash_buscar()__ ---> O(n), en el peor de los casos que se tenga una mala función de hash se recorrerán "n" claves hasta llegar a la buscada. En caso de tener una buena función de hash su complejidad tiende lo más cercano a O(1).

__hash_existe()__ ---> O(n), en el peor de los casos que se tenga una mala función de hash se recorrerán "n" claves hasta llegar a la buscada. En caso de tener una buena función de hash su complejidad tiende lo más cercano a O(1).

__size_t hash_tamanio()__ ---> O(1), inmediato acceso a través del atributo ```cant_claves``` del struct.

__void hash_destruir()__ ---> O(n), antes de liberar la tabla, se recorre toda la tabla liberando las claves almacenadas.

__void hash_destruir_todo()__ ---> O(n), antes de liberar la tabla, se recorre toda la tabla liberando las claves almacenadas y si hay un destructor libera los valores también.

__size_t hash_iterar_claves()__ ---> O(n), recorre toda la tabla aplicando una función a todas las claves almacenadas.

---

##  Preguntas del TP

### Explicar teóricamente los siguientes puntos (no necesariamente en orden, pero por favor usando diagramas):

### - Qué es un diccionario - Explicar 3 formas diferentes de implementar un diccionario (tabla de hash cuenta como 1). Justificar ventajas y desventajas.

Un diccionario es un conjunto de __pares claves/valor__, en donde cada __clave es única__ y se le asocia un determinado valor. Un diccionario se puede implementar con:

1. __TABLA DE HASH__

    La principal ventaja al implementarlo con una tabla de hash es que sus operaciones de búsqueda/inserción/eliminación tienden muy cercano a O(1) y una desventaja es que si no se tiene una buena función de hash aumenta las probabilidades de colisiones perdiendo su principal ventaja.

<div align="center">
<img src="img/cerrado.svg">
</div>

2. __LISTA__

    La principal ventaja es que implementarlo con una lista requiere menos lógica (_comparado con un hash o un abb_) y una desventaja es que dependiendo de la implementación de la lista se puede mejorar la complejidad de ciertas operaciones pero en general todas sus operaciones de búsqueda/inserción/eliminación tienden a O(n).

<div align="center">
<img src="img/lista.svg">
</div>

3. __ABB__

    La principal ventaja al implementarlo con un abb es que permite insertar y acceder las claves ordenadamente por poseer un comparador y una desventaja es que si no esta bien balanceado empeora el rendimiento llegando a poder degenerarse a una lista donde sus recorridos tienden a O(n).

<div align="center">
<img src="img/abb.svg">
</div>

### - Qué es una función de hash y qué características debe tener para nuestro problema en particular
Una función de hash es una función que al pasarle una clave nos retornara un número asociado que luego al aplicarle la operación módulo (_%_) contra la capacidad de la tabla de hash, obtendremos una posición fija en la tabla que le asignamos a nuestra clave y es de utilidad para luego realizar operaciones de búsqueda/inserción/eliminación.
- __Caracteristicas para nuestra función de hash__

    - __Uniforme__ ---> Queremos que las colisiones que se produzcan, se distribuyan lo más parejo posible a medida de que el tamaño de la tabla crezca, asi evitamos un mayor número seguido de colisiones al insertar una nueva clave y mantener una complejidad lo más cercano posible a O(1) en las operaciones de búsqueda/inserción/eliminación (_En caso de que no hubiese una distribución pareja estás operaciones tenderian a O(n)_). 

    <div align="center">
    <img src="img/uniforme.svg">
    </div>

    - __Rapida__ ---> Queremos que el número asociado a la clave que devuelve la función de hash sea un algoritmos que se resuelva lo más rápido posible. Esto es porque esta función es llamada constantemente al realizar la búsqueda de una clave y no queremos perder rendimiento.

    - __Deterministica__ ---> Necesitamos que el número asociado a la clave que devuelve la función de hash sea siempre la misma para esa determinada clave. Porque en caso contrario, perderiamos la referencia de su posición en la tabla al buscarla tras ser insertada.

    <div align="center">
    <img src="img/deterministica.svg">
    </div>

### - Qué es una tabla de Hash y los diferentes métodos de resolución de colisiones vistos.

Una tabla de hash __es una tabla que contiene valores__ a los cuáles se pueden acceder mediante una función de hash cuyo parámetro principal es una clave asociada a ese valor. Existen dos tipos de hash:

-__Abierto__: Este tipo de hash tiene un direccionamiento cerrado donde la tabla funciona de "indice" y mediante la función de hash le asignamos el indice correspondiente a la clave/valor, estos elementos son guardados fuera de la tabla mediante otro __TDA__ (_por ejemplo, una lista_) y las colisiones que se producen se solucionan "encadenando" cada clave/valor entre ellas mismas teniendo que recorrer el TDA para acceder a alguna de ellas.  

<div align="center">
<img src="img/abierto.svg">
</div>

-__Cerrado__: Este tipo de hash tiene un direccionamiento abierto donde las claves/valores son almacenados dentro de la misma tabla y la función de hash determina en que posición será ubicada. A la hora que se produzca una colisión al insertar se seguira recorriendo la tabla hasta encontrar el próximo espacio disponible.

<div align="center">
<img src="img/cerrado.svg">
</div>

### - Explicar por qué es importante el tamaño de la tabla (tanto para tablas abiertas como cerradas)

- Dado que en una tabla abierta se pueden encadenar colisiones sin importar el tamaño de la tabla, ¿Realmente importa el tamaño?

    El tamaño de una tabla es importante para poder saber cuando expandirla cuando insertemos cierta cantidad de claves. Ya que a medida de que la tabla se llena aumenta la probabilidad de que se ocasione una colisión. En el caso de un hash abierto a pesar de que se puedan encadenar las colisiones, es importante el tamaño porque si no la expandimos a medida de que vayamos insertando claves la tabla se irá degenerando en una lista. Donde para acceder a una clave habrá que recorrer "n" posiciones perdiendo el proposito principal de la tabla que es el acceso a los valores lo más cercano posible a O(1). 

    <div align="center">
    <img src="img/tamanio.svg">
    </div>