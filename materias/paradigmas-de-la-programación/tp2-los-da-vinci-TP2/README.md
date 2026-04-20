|                   Universidad                        |            Materia              |                Docentes                  | Correctora             |                            Da Vinci                                |
|:----------------------------------------------------:|:-------------------------------:|------------------------------------------|------------------------|------------------------------------------------------------------------|
| Facultad de ingeniería, universidad de Buenos Aires | Paradigmas de la programación | - Essaya Diego <br> - Maraggi Santiago      | - Macarena Vita Sanche | - Fernández Giraldo Diego Jose (112571) <br> - Galeano Galvis Sara Lucia (112120) |

# TP2 los da vinci

## Link videos

Diego: https://youtu.be/_Ix8f03nonk?si=aiUuyt1ZnrU9YZJl
Sara: https://youtu.be/MSbl9QqDCrY?si=RxoB03hzu-Wjq4Pz
Ejecución
- Linux/WSL (por defecto en `project.clj`):
  - `lein run` → abre la app con el editor vacío.
  - `lein run "x y add"` → abre la app con el código precargado.
  - `lein test` → corre los tests.

Lenguaje de la VM (versión mínima)
- `X`/`x`: apila `x` (0..255) en la pila de datos.
- `Y`/`y`: apila `x` (0..255) en la pila de datos.
- `T`/`t`: apila `t` (0..255) en la pila de datos.
- `N`/`n`: apila 0 para iniciar un literal decimal.
- `0`..`9`: reemplaza el tope por `tope*10 + dígito`. Requiere haber iniciado con `N` (o tener un número en el tope).
- `C`/`c`: Si el valor en el tope de DS es negativo, se reemplaza por 0. Si es mayor a 255, se reemplaza por 255.
- `D`/`d`: Apila una copia del valor en el tope de DS.
- `P`/`P`: Desapila un valor de DS y lo descarta.
- `S`/`s`: Intercambia los 2 valores en el tope de DS.
- `R`/`r`: Rota los 3 valores en el tope de DS. [... a b c] -> [... b c a]
- `!`: Desapila a. Si a es 0, apila 1; en caso contrario apila 0.
- `+`: Desapila b. Desapila a. Apila a + b.
- `-`: Desapila b. Desapila a. Apila a - b.
- `*`: Desapila b. Desapila a. Apila a * b.
- `^`: Desapila b. Desapila a. Apila a ^ b. (operación de bits XOR).
- `&`: Desapila b. Desapila a. Apila a & b. (operación de bits AND).
- `|`: Desapila b. Desapila a. Apila a | b. (operación de bits OR).
- `=`: Desapila 2 valores. Apila 1 si son iguales, 0 en caso contrario.
- `<`: Desapila b. Desapila a. Apila 1 si `a < b`, 0 en caso contrario.
- `>`: Desapila b. Desapila a. Apila 1 si `a > b`, 0 en caso contrario.
- `/`: Desapila b. Desapila a. Si `b` es 0:
  - si `M` es 0, produce error.
  - si `M` es 1, finaliza el pixel con RGB `0,0,0`.
  - si `M` es 2, finaliza el pixel con RGB `255,0,0`.
  En caso contrario, apila `a / b` (división entera).
- `%`: Desapila b. Desapila a.
  - Si b es 0: mismo comportamiento que /.
  En caso contrario, apila a % b (resto de la división entera).
- `M`/`m`: Incrementa el modo `M` (0→1→2→0).
- `[` y `]`: Ciclos con contador:
  - `[` desapila `a` de DS. Si `a > 0`, apila en LS `IDX+1` y `a` (con `a` en el tope) e inicia el ciclo. Si `a <= 0`, salta al comando inmediatamente después del `]` correspondiente (respetando anidamientos).
  - `]` decrementa el contador (tope de LS). Si sigue siendo positivo, salta a `IDX+1` guardado (debajo del contador). Si no, desapila ambos y continúa.

- Whitespaces se ignoran. Cualquier otro carácter produce error.
- Límite de código: ≤ 1024 caracteres. Pila de datos (DS) máx 8.

Reglas y límites
- Longitud de código ≤ 1024 caracteres. Si excede, error.
- Pila de datos (DS) máx 8; pila de loops (LS) máx 8.
- Al terminar, los tres valores del tope son `R G B` (faltantes = 0). Si alguno fuera de 0..255 → error.


Estructura
- `logica/davici/vm.clj` → VM pura y funciones de evaluación (sin efectos).
- `vista-clj/davici/main.clj` → `-main` de Leiningen (arranca la UI JavaFX).
- `vista/com/example/davici/App.java` → UI JavaFX, render y concurrencia.
- `vista/com/example/davici/Launcher.java` → puente para lanzar JavaFX.

Notas
- La UI renderiza cada frame en un pool de trabajo y actualiza el `ImageView` en el hilo de UI. Si ocurre un error, se muestra en la etiqueta y se detiene el avance de `t` de ese frame.
- La ventana es no redimensionable.

Dependencias JavaFX
- El proyecto usa artefactos `org.openjfx` con classifier específico por plataforma.
