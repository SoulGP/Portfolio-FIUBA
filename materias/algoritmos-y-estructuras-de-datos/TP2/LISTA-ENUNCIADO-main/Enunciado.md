# TDA Lista/Pila/Cola

Se pide implementar una lista simplementa enlazada, una pila y una cola según las interfaces definidas en clase.

Los TDAs entregados deberan compilar y pasar las pruebas dispuestas por la cátedra sin errores, adicionalmente estas pruebas deberán ser ejecutadas **sin pérdida de memoria**.

Se facilita al alumno un archivo simple de _ejemplo_ (**ejemplo.c**). **Este archivo no es mas que un programa mínimo de ejemplo de utilización del TDA a implementar y es provisto sólo a fines ilustrativos** como una ayuda extra para entender el funcionamiento del mismo. No es necesario modificar ni entregar el archivo de ejemplo, pero **si la implementación es correcta, debería correr con valgrind sin errores de memoria**.

Recordar que es obligatorio entregar un informe que explique cómo se implementaron los TDAs, cómo funcionan, la complejidad de las operaciones y cualquier aclaración que sea necesaria para el corrector.

Recordar también que es obligatorio entregar **pruebas_alumno.c** con las pruebas realizadas para verificar 
el correcto funcionamiento de los TDA.

## Restricciones

  Para que el trabajo se considere aprobado, la implementación debe cumplir los siguientes requisitos:

  - Las lista debe ser simplemente enlazada
  - Las operaciones **apilar**, **desapilar** y **tope** deben ser **O(1)**
  - Las operaciones **encolar**, **desencolar** y **frente** deben ser **O(1)**
  - Las implementaciones de pila y cola deben reutilizar la implementación de lista

## Parte teórica

Explicar teóricamente los siguientes puntos (no necesariamente en orden, pero por favor usando diagramas):

   - Explicar qué es una lista, lista enlazada y lista doblemente enlazada.
     - Explicar las características de cada una.
     - Explicar las diferencias internas de implementación.
     - Explicar ventajas y desventajas de cada una, si existen.
   - Explicar qué es una lista circular y de qué maneras se puede implementar.
   - Explicar la diferencia de funcionamiento entre cola y pila.
   - Explicar la diferencia entre un iterador interno y uno externo.
