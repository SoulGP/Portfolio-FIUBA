# Estructura recomendada para proyectos

Esta plantilla describe la organización de carpetas y archivos recomendada para cada proyecto dentro del portafolio.

---

## Estructura base

```
nombre-del-proyecto/
├── src/              # Código fuente principal
│   ├── main.c        # Punto de entrada (adaptar según el lenguaje)
│   └── ...
├── include/          # Archivos de cabecera (C/C++)
│   └── ...
├── docs/             # Documentación adicional (informes, diagramas)
│   └── informe.pdf
├── tests/            # Pruebas unitarias o de integración
│   └── test_main.c
├── data/             # Archivos de datos de entrada/salida (si aplica)
│   └── entrada.txt
├── Makefile          # Sistema de compilación (C/C++) – usar el que corresponda al lenguaje
├── requirements.txt  # Dependencias (Python) – alternativa a Makefile según el lenguaje
├── .gitignore        # Archivos ignorados por Git
└── README.md         # Documentación del proyecto
```

---

## Notas por lenguaje

### C / C++
- Usar `Makefile` para compilación.
- Separar encabezados en `include/` y fuentes en `src/`.
- Agregar `*.o`, `*.out`, y binarios al `.gitignore`.

### Python
- Usar `requirements.txt` o `pyproject.toml` para dependencias.
- Organizar módulos dentro de `src/` con `__init__.py` si corresponde.
- Agregar `__pycache__/`, `*.pyc`, y entornos virtuales al `.gitignore`.

### Java
- Separar fuentes en `src/` y clases compiladas en `bin/` o `out/`.
- Usar `Makefile` o scripts de compilación simples.
- Agregar `*.class` y carpetas de salida al `.gitignore`.

---

## Convenciones

- Los nombres de carpetas y archivos deben estar en **minúsculas con guiones** (`kebab-case`).
- Cada proyecto debe contener al menos un `README.md`.
- Evitar subir archivos binarios o compilados al repositorio.
- Ver [`docs/convenciones-de-nombres.md`](../docs/convenciones-de-nombres.md) para más detalles.
