from fastapi.testclient import TestClient
from app.main import app
from app.database import get_session
from sqlmodel import SQLModel, Session, create_engine
import pytest
from app.modelos import (
    Equipo,
    Movimientos,
    Pokemon,
    MovimientosPokemon,
)

DATABASE_URL = "sqlite:///./test_1.db"
engine = create_engine(DATABASE_URL, echo=True)


@pytest.fixture(scope="module")
def setup_db():
    SQLModel.metadata.create_all(engine)
    with Session(engine) as session:
        equipo_1 = Equipo(id=1, nombre="Equipo Rocket", generacion=1)
        equipo_2 = Equipo(id=2, nombre="Equipo Aqua", generacion=3)

        pokemon_1 = Pokemon(
            id=1,
            nombre="Pikachu",
            imagen="p.png",
            altura=0,
            peso=0,
            especie=1,
            generacion=1,
        )
        pokemon_2 = Pokemon(
            id=2,
            nombre="Squirtle",
            imagen="r.png",
            altura=0,
            peso=0,
            especie=3,
            generacion=1,
        )

        movimiento_1 = Movimientos(
            id=1,
            nombre="Thunderbolt",
            tipo=1,
            categoria=1,
            potencia=90,
            precision=100,
            usos=15,
            generacion=1,
            efecto=1,
        )
        movimiento_2 = Movimientos(
            id=2,
            nombre="Water Gun",
            tipo=2,
            categoria=2,
            potencia=40,
            precision=100,
            usos=20,
            generacion=1,
            efecto=2,
        )

        relacion = MovimientosPokemon(pokemon_id=2, move_id=1, id_metodo=1, nivel=0)

        session.add_all(
            [
                equipo_1,
                equipo_2,
                pokemon_1,
                pokemon_2,
                movimiento_1,
                movimiento_2,
                relacion,
            ]
        )
        session.commit()
    yield
    SQLModel.metadata.drop_all(engine)


@pytest.fixture
def client():
    def override_get_session():
        with Session(engine) as session:
            yield session

    app.dependency_overrides[get_session] = override_get_session
    yield TestClient(app)
    app.dependency_overrides.clear()


def test_editar_equipo_exitoso(client, setup_db):
    id_equipo_nuevo = 1
    generacion = 4
    response = client.put(
        f"equipos/editar/{id_equipo_nuevo}?nombre_equipo=dedwe&generacion_equipo={generacion}"
    )

    assert response.status_code == 200
    assert response.json()["nombre"] == "dedwe"


def test_editar_equipo_no_encontrado(client, setup_db):
    equipo_nuevo_id = 7
    generacion_nueva = 4
    response = client.put(
        f"/equipos/editar/{equipo_nuevo_id}?nombre_equipo=djbldasl&generacion_equipo={generacion_nueva}"
    )

    assert response.status_code == 404
    assert response.json() == {
        "detail": f"El equipo con id {equipo_nuevo_id} no ha sido encontrado"
    }


def test_editar_equipo_movimientos_incompatibles(client, setup_db):
    pokemon_id = 1
    naturaleza_id = 1
    movimientos = 2

    id = 1
    generacion = 4

    response = client.put(
        f"/equipos/editar{id}?nombre_equipo=kjfds&generacion_equipo={generacion}&id_pokemon_1={pokemon_id}&id_naturaleza_1={naturaleza_id}"
    )

    assert response.status_code == 404
