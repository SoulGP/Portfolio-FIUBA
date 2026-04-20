<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP2

## Alumno: Diego Jose Fernandez Giraldo - 112571 - diego.j.fernandez.g@gmail.com

- Para compilar el TP:

```bash
make tp2
```

- Para compilar los tests:

```bash
make pruebas_alumno
```

- Para ejecutar el TP __con__ valgrind:

```bash
make valgrind-tp2
```

- Para ejecutar los tests __con__ valgrind:

```bash
make valgrind-alumno
```
---


---

##  Preguntas del TP

- __Explicar el TDA menú. Justifique las operaciones incluídas en el TDA.__

Yo pensé al TDA menú como un hash por ende reutilicé el TDA hash implementado anteriormente para el desarrollo del TDA menú. Esto porque pensaba a la inicial de referencia de la opción como una "clave" y la acción de la opción como su "valor" asociado.

Después las operaciones que incluí para el TDA menú aparte de las basicas de ```crear()``` y ```destruir()``` son las tres siguientes:

1. __```menu_agregar_opcion()```:__ Mediante una estructura auxiliar "opcion_menu_t" guardo toda la información sobre la opción, es decir, su descripción y acción (_la función que se va a ejecutar al ser seleccionada_) y lo almaceno en el hash asociando esta información a la inicial correspondiente.

2. __```menu_mostrar()```:__ Itera las claves existentes para mostrarle al usuario las distintas opciones disponibles y saber cuál ejecutar.

3. __```menu_ejecutar()```:__ Si la opción existe ejecutara la acción asociada.

<div align="center">
<img src="img/operaciones_menu.svg">
</div>

- __¿Qué TDAs fueron utilizados para implementar las funcionalidades pedidas? ¿Por qué fue elegido ese TDA y no otro?__

    - __Para el movimiento de los jugadores hice uso del "TDA hash"__. Ya que a _x_ tecla se le asocia una función de movimiento y lo vi más adecuado que una lista o abb donde los recorridos son más costosos cuando yo necesito que tienda lo más posible a O(1) porque son acciones que se repiten constantemente. 

    - __Para manejar los pokemones capturados por los jugadores usé el "TDA pila"__. Ya que solo me interesa los pokemones que se encuentran en el tope para mostrarlos por pantalla o para las mecanicas de modificación de puntajes. No use otros TDA´s porque o me rompen el orden de inserción o hay que iterar n elementos para llegar al tope, cosa que en una pila es O(1).
    
    <div align="center">
    <img src="img/manejo_pkm_pila.svg">
    </div>

    - __Para rellenar el terreno con pokemones salvajes utilicé el "TDA lista" y el "TDA cola" para cada jugador__. Donde se tiene una lista donde se van almacenando los pokemones ya capturados más los cuatro disponibles por capturar, y a medida de que se vayan necesitando más pokemones para capturar se irán desencolando de la cola de los siguientes pokemones salvajes disponibles. No use el TDA hash porque me rompe el orden de inserción especialmente el de la cola, donde sino se respeta no se asegura que los jugadores mantengan el mismo orden de aparición de pokemones salvajes, y tampoco use el TDA abb porque necesito contabilizar cuantos pokemones no capturados hay en el terreno para saber si hay que agregar más o no y eso es recorrer n elementos, así que no puedo aprovechar sus ventajas de búsqueda binaria.

    <div align="center">
    <img src="img/manejo_pkm_salvajes.svg">
    </div>

- __Explicar cualquier otro TDA implementado.__

Para el desarrollo del juego implemente un __"TDA juego"__ en donde se maneja toda su inicialización, jugabilidad, visualización, control de estado y finalmente su liberación de toda memoria reservada.

<div align="center">
<img src="img/operaciones_juego.svg">
</div>

- __Explique qué dificultades encontró al implementar el TP2 y cómo pudo solucionarlas.__

    - Me costo a la hora de implementar el "TDA menú" el como deben ser las firmas de las funciones almacenadas que hacen de la acción de una opción, ya que cada opción puede depender de _x_ parametros y la solución a ello es que todas las funciones almacenadas reciban como parametro un contexto y ya dependa del usuario que guardar en el para la funcionalidad de sus opciones.

    - Se me dificulto manejar el flujo de añadición de nuevos pokemones salvajes ya que en un principio lo manejaba con dos listas, una donde tenía los _n_ pokemones de la pokedex con toda su información (_posición, pokemon, si fue capturado_) y la otra que elejía aleatoriamente uno de estos. Esto producía que no se admitieran pokemones repetidos y en el caso de haberlo lo que hacía era modificar la posición del único existente rompiendo las reglas de juego. Esto lo solucione primero cambiando el como guardo los pokemones para que se pudieran admitir repetidos y sean independientes y además hice uso del "TDA cola" para poder ir añadiendolos a la lista a medida de que se vayan necesitando.


