<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TDA LISTA/PILA/COLA

## Alumno: Diego Jose Fernandez Giraldo - 112571 - diego.j.fernandez.g@gmail.com

- Para compilar:

```bash
$ make all
```

- Para ejecutar el ejemplo:

```bash
$ ./ejemplo 
```

- Para ejecutar los test:

```bash
$ ./test 
```

- Para ejecutar con valgrind:
```bash
$ valgrind -s <ejecutable>
```
---
##  Detalles generales

En __ejemplo.c__ se hizo uso de las primitivas de lista.h con ejemplos básicos de funcionamiento. Para la implementación de la estructura de la lista simplemente enlazada aparte del nodo que apunta al primer elemento de la lista la pensé con un nodo extra que apunte al final de la lista y asi mejorar su complejidad computacional en ciertas operaciones como buscar e insertar al final de misma, pasando de tener que recorrer secuencialmente nodo a nodo (O(n)) a simplemente acceder a través del puntero extra (O(1)). 

Una de las problematicas que tuve a la hora de hacer el tp fue el implementar la función __pila_desapilar()__ con una complejidad O(1). Esto era porque pensaba a la pila como una lista convencional cuyo tope se encontraba en su elemento final, lo que hacía que a la hora de desapilar la complejidad de la operación fuera O(n), pero después llegue a la solución de que no hacía falta insertar al final sino que era más que suficiente el solo insertar siempre al principio y tratar este como el tope logrando una mejor eficiencia y cumpliendo que pila_desapilar() fuere O(1). 

No demostré el funcionamiento básico de las primitivas de cola.h y pila.h en ejemplo.c porque reutilizan las primitivas de lista.h. De todas formas hice uso de estos tda´s a la hora de implementar los tests. 

---

## Respuestas a las preguntas teóricas
Explicar teóricamente los siguientes puntos (no necesariamente en orden, pero por favor usando diagramas):

   1. __Explicar qué es una lista, lista enlazada y lista doblemente enlazada.__
      - Explicar las características de cada una.
      - Explicar las diferencias internas de implementación.
      - Explicar ventajas y desventajas de cada una, si existen.

      <div align="center">
      <img width="70%" src="img/li.drawio.svg">
      </div>

      Una lista se puede implementar con un vector dinámicamico o estático, en el caso de ser estático va a estar limitado por un tamaño máximo arbitrario no modificable y si es dinámico, permite expandir el tamaño de la lista siempre y cuando halla suficiente memoria contigua disponible.

      Ambos se manejan con topes para evitar accesos a memoria no válida y solo puden almacener el tipo de dato con el que fue definido. Realizar ```lista[0]``` permite el acceso al primer elemento de la lista, ```lista[i]``` al elemento en la "i" posición y ```lista[tope]``` al último elemento de la misma.

      Ventajas que pueden ofrecer este tipo de listas es la comodidad de manejar los elementos almacenados, ya que para acceder a un elemento de una lista o puedes recorrerla hasta encontrar el elemento buscado o, si de antemano sabes su posición puedes acceder directamente a ella sin tener que recorrerla aumentando la eficiencia.

      Las desventajas tienen que ver con que solo pueden almacenar el tipo de dato con el cual se definió, en el caso de ser estática, de llegar a necesitar más espacios de almacenamiento no puede ser expandida y de ser dinámica al momento de no haber más memoria contigua disponible tampoco puede ser expandida la lista. 

      La complejidad de sus operaciones son:
      - __O(1)__ para la inserción y acceso por índice.
      - __O(1)__ para la inserción y acceso al primer o último elemento.
      - __O(1)__ para la inserción y acceso en el medio.
      - __O(1)__ para borrar el último elemento.
      - __O(n)__ para borrar el primer elemento.
      - __O(n)__ para borrar algún dato del medio.
      - __O(1)__ para liberar memoria reservada (_solo para la lista de vector dinámico_).
      - __O(n)__ para redimensionar (_solo para la lista de vector dinámico_).

      <div align="center">
      <img width="70%" src="img/lie.drawio.svg">
      </div>

      Una lista simplemente enlazada se implementa mediante múltiples nodos, cada uno de los cuales almacena un puntero a un dato y apunta al siguiente nodo de la lista.

      La lista mantiene un contador de elementos para evitar accesos a memoria no válida. Para acceder a un elemento, se debe recorrer la lista nodo a nodo hasta encontrar el deseado (_se puede añadir un puntero adicional al último nodo para acceder rápidamente al final_).

      Frente a las listas basadas en vectores estáticos o dinámicos, la lista simplemente enlazada ofrece mayor flexibilidad al expandir su capacidad, pues sus nodos no requieren memoria contigua y pueden almacenar distintos tipos de datos.

      Su principal desventaja es que a excepción del primer elemento (y último si se implento con un nodo a la cola) todos los demás requieren un recorrido secuencial nodo a nodo antes de poder operar sobre ellos.

      La complejidad de sus operaciones son:
      - __✗__ para la inserción y acceso por índice.
      - __O(1)__ para la inserción y acceso al primer y último elemento.
      - __O(n)__ para la inserción y acceso en el medio.
      - __O(n)__ para borrar el último elemento.
      - __O(1)__ para borrar el primer elemento.
      - __O(n)__ para borrar algún dato del medio.
      - __O(n)__ para liberar memoria reservada.
      - __✗__ para redimensionar.

      <div align="center">
      <img width="70%" src="img/lide.drawio.svg">
      </div>

      Una lista doblemente enlazada es muy similar a la simplemente enlazada, pero cada nodo incorpora un puntero adicional al nodo anterior (_si existe_), permitiendo un recorrido bidireccional.

      Esto aporta ventaja en la eficiencia de ciertas operaciones: por ejemplo, si cuentas con un puntero directo al nodo a eliminar, tanto la eliminación como la inserción en esa posición se llevan a cabo en __O(1)__, ya que tienes acceso inmediato al nodo anterior y al siguiente, y puedes reconectar ambas referencias sin recorrer la lista.

      Como desventaja, incurres en un mayor consumo de memoria (_cada nodo almacena un puntero extra_).

      La complejidad de sus operaciones son:
      - __✗__ para la inserción y acceso por índice.
      - __O(1)__ para la inserción y acceso al primer y último elemento.
      - __O(n)__ para la inserción y acceso en el medio (_O(1) si se tiene un puntero al nodo_).
      - __O(1)__ para borrar el último elemento.
      - __O(1)__ para borrar el primer elemento.
      - __O(n)__ para borrar algún dato del medio (_O(1) si se tiene un puntero al nodo_).
      - __O(n)__ para liberar memoria reservada.
      - __✗__ para redimensionar.

   2. __Explicar qué es una lista circular y de qué maneras se puede implementar.__
  <div align="center">
  <img width="70%" src="img/lic.drawio.svg">
  </div>

  Una lista circular es una lista enlazada con multiples nodos que tiene de caracteristica, que el último elemento, el campo siguiente del nodo no apuntará a __NULL__ sino de vuelta al primer nodo, formando un recorrido cíclico sin fin o hasta que se cumpla cierta condición dada por el usuario.

  Esta lista se puede implementar tanto con la estructura de una lista simplemente enlazada o una doblemenete enlazada la diferencia es que en la doblemenete enlazada el campo anterior del primer nodo apuntará al último nodo de la lista dando un recorrido cíclico bidireccional.

  3. __Explicar la diferencia de funcionamiento entre cola y pila.__

  <div align="center">
  <img width="70%" src="img/p_y_c.drawio.svg">
  </div>

   Alguna de las diferencias son:
   - Una pila se rige por el principio LIFO donde todos los elementos se apilan por el tope de la pila y se desapilan por el mismo.
   - Una cola se rige por el principio FIFO donde los elementos se encolan por el final de la cola y se desencolan por el frente de la cola.
   - En una pila solo se puede observar el elemento del tope, mientras que en una cola se puede observar los elementos del frente y la cola.

   4. __Explicar la diferencia entre un iterador interno y uno externo.__
   
   Un iterador interno maneja el ciclo de iteración de un tda a través de una función y recibe tres parámetros:
   - El tda.
   - Un puntero a una función (_recibe el contenido del nodo y un puntero extra en caso de ser necesario_).
   - Un puntero a void extra que sirve de memoria común para todos los nodos visitados.

   Un iterador externo nos permite manejar el ciclo de iteración de un tda a través de otro tda (_tda_iterador_) que provee un set de primitivas especiales para recorrer la estructura. Este nuevo tda debe tener un minimo de operaciones, las cuales son: 

   - crear el iterador.
   - verificar si quedan elementos por recorrer.
   - avanzar al siguiente elemento del tda.
   - obtener el elemento actual.
   - destruir el iterador.

