# IntroDex

![Python](https://img.shields.io/badge/Python-100%25-blue)

## Descripción
IntroDex es un proyecto desarrollado en Python.
Este proyecto utiliza [FastAPI](https://fastapi.tiangolo.com/) como framework de API y [Alembic](https://alembic.sqlalchemy.org/) para la gestión de migraciones de base de datos.

Consiste en implementar un sitio web para obtener información sobre Pokemones, sus movimientos y habilidades así como también la posibilidad de creación de equipos de combate (similar a [PokemonDB](https://pokemondb.net/)).

## Requisitos

- Python 3.8 o superior
- [pip](https://pip.pypa.io/en/stable/installation/) para gestionar dependencias
- Opcional: [Virtualenv](https://docs.python.org/3/library/venv.html) para crear un entorno virtual

## Instalación
1. **Clona** el repositorio:

    ```bash
    git clone https://github.com/Intro-Equipo-Rocket/IntroDex.git
    ```

2. **Crea un entorno virtual** (opcional pero recomendado):

    ```bash
    python3 -m venv venv
    source venv/bin/activate 
    ```
3. **Instala las dependencias** necesarias:

    ```bash
    pip install -r requirements.txt
    ```

## Uso
### Ejecutar el Servidor
#### FastAPI

> **❗IMPORTANT:** Se adjunta aquí un repositorio con los archivos .csv con la información necesaria para soportar la funcionalidad del proyecto: [Veekun](https://github.com/veekun/pokedex)


1. **Inicia el servidor de desarrollo** usando el comando de FastAPI `uvicorn`:

    ```bash
    uvicorn app.main:app --reload
    ```

    - `app.main:app` es el path hacia tu aplicación FastAPI.
    - `--reload` reinicia el servidor automáticamente al detectar cambios en el código (útil para desarrollo).

2. **Accede a la API** en tu navegador o cliente HTTP:

    - La API estará disponible en: [http://127.0.0.1:8000](http://127.0.0.1:8000)
    - La documentación interactiva (Swagger UI) estará en: [http://127.0.0.1:8000/docs](http://127.0.0.1:8000/docs)

#### Docker

1. **Abre una terminal y navega al directorio raíz del proyecto** donde se    encuentra el archivo Dockerfile.

2. **Construye la imagen Docker** con el siguiente comando:

    ```bash
        docker build -t <Nombre-de-la-imagen> .
    ```

    - `-t nombre-de-la-imagen`: Etiqueta tu imagen con un nombre amigable.
    - `.`: Especifica el contexto de construcción (directorio actual).

3. **Ejecuta un contenedor** basado en la imagen que acabas de construir:

    ```bash
        docker run --rm -it -p 8000:8000 <Nombre-de-la-imagen>
    ```

    - `-d`: Opcional, Ejecuta el contenedor en segundo plano.
    - `-it`: ver salida en la terminal.
    - `--rm`: contenedor descartable, borrarlo al terminar.
    - `-p 8000:8000`: Mapea el puerto 8000 del contenedor al puerto 8000 de tu máquina.

4. **Accede a la API** en tu navegador o cliente HTTP:

    - La API estará disponible en: [http://127.0.0.1:8000](http://127.0.0.1:8000)
    - La documentación interactiva (Swagger UI) estará en: [http://127.0.0.1:8000/docs](http://127.0.0.1:8000/docs)

### Crear Migraciones

Para hacer una migración cuando cambies el modelo de datos en el proyecto:

1. **Crear un archivo de migración**:

    ```bash
    alembic revision --autogenerate -m "Descripción de los cambios"
    ```

    Esto genera un nuevo archivo de migración en `alembic/versions/` que describe los cambios.

2. **Aplicar la migración**:

    ```bash
    alembic upgrade head
    ```

    Este comando aplica los cambios en la base de datos.

3. **Revertir una migración** (opcional):

    Para deshacer la última migración aplicada:

    ```bash
    alembic downgrade -1
    ```


## Contribuyendo
1. **Haz un fork del proyecto.**
2. **Crea una nueva rama** para tu feature o fix:

    ```bash
    git checkout -b feature/nueva-feature
    ```

3. Realiza tus cambios y **haz commit**:

    ```bash
    git commit -m 'Añadir nueva feature'
    ```

4. **Sube tus cambios** a tu fork:

    ```bash
    git push origin feature/nueva-feature
    ```

5. **Crea un Pull Request** en GitHub.

## Extras

>**ℹ️ NOTE:** Si experimentas problemas de importación al ejecutar el proyecto, intenta utilizar el siguiente comando para establecer el `PYTHONPATH`:
>
> ```bash
> export PYTHONPATH=$PYTHONPATH:/ruta_de_su_dispositivo/IntroDex
> ```
>
> Reemplaza `/ruta_de_su_dispositivo/IntroDex` con la ruta absoluta a tu proyecto.

> **⚠️ WARNING:** NO modificar los modelos de clases existentes ni alterar cualquier columna de la database, en caso de hacerlo tenga en cuenta que MUY probablemente rompera el funcionamiento de todos los endpoints o la mayoria de ellos. 


