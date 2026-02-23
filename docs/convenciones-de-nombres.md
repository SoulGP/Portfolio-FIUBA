# Convenciones de nombres

Este documento define las reglas de nombrado de archivos y carpetas dentro del repositorio para mantener consistencia y legibilidad.

---

## Reglas generales

1. **Minúsculas siempre.** No usar mayúsculas en nombres de archivos ni carpetas.
2. **Guiones en lugar de espacios o guiones bajos.** Usar `kebab-case` para separar palabras.
   - ✅ `trabajo-practico-1`
   - ❌ `Trabajo Practico 1`
   - ❌ `trabajo_practico_1`
3. **Sin caracteres especiales ni tildes en nombres de carpetas/archivos.**
   - ✅ `analisis-matematico`
   - ❌ `análisis-matemático`
4. **Sin espacios.** Los espacios en nombres de archivo generan problemas en terminales y scripts.

---

## Estructura de carpetas

```
materias/
└── nombre-de-materia/
    └── tp-numero-nombre/
        ├── src/
        ├── docs/
        ├── tests/
        └── README.md
```

Ejemplo:
```
materias/
└── algoritmos/
    └── tp1-lista-enlazada/
        ├── src/
        └── README.md
```

---

## Nombrado de archivos de código

| Lenguaje | Convención          | Ejemplo              |
|----------|---------------------|----------------------|
| C        | `snake_case`        | `lista_enlazada.c`   |
| C++      | `snake_case`        | `arbol_binario.cpp`  |
| Python   | `snake_case`        | `ordenamiento.py`    |
| Java     | `PascalCase`        | `ListaEnlazada.java` |
| Markdown | `kebab-case`        | `informe-final.md`   |

---

## Nombrado de commits

Usar mensajes de commit claros y concisos en infinitivo o imperativo:

- ✅ `Agregar implementación de lista enlazada`
- ✅ `Corregir error en función de búsqueda`
- ❌ `cambios`
- ❌ `arreglé cosas`

---

## Nombrado de ramas

Si se trabaja con ramas:

```
materia/nombre-del-tp
```

Ejemplo:
```
algoritmos/tp1-lista-enlazada
```

---

## Archivos obligatorios por proyecto

Cada proyecto dentro del portafolio debe incluir:

- `README.md` – Descripción, instrucciones y contexto.
- `.gitignore` – Archivos a excluir (si aplica localmente).
- `Makefile` o script equivalente – Si el proyecto requiere compilación.
