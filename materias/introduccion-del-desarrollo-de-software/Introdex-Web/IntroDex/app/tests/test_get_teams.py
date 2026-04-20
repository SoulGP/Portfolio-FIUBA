from fastapi.testclient import TestClient
from app.modelos import Equipo
from app.main import app
from app.database import get_session
from sqlmodel import SQLModel, Session, create_engine
import pytest

DATABASE_URL = "sqlite:///./test.db"
engine = create_engine(DATABASE_URL, echo=True)


@pytest.fixture(scope="module")
def setup_db():
    SQLModel.metadata.create_all(engine)
    with Session(engine) as session:
        session.add_all(
            [
                Equipo(id=1, nombre="Equipo Rocket", generacion=1),
                Equipo(id=2, nombre="Equipo Aqua", generacion=3),
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


def test_obtener_equipos_pagina_1(client, setup_db):
    pagina = 1
    cantidad_equipos = 10
    response = client.get(
        f"/equipos/pagina/{pagina}?cantidad_equipos={cantidad_equipos}"
    )

    assert response.status_code == 200
    data = response.json()
    assert len(data) <= cantidad_equipos
    assert data[0]["nombre"] == "Equipo Rocket"


def test_obtener_equipos_pagina_sin_equipos(client, setup_db):
    pagina = 5
    cantidad_equipos = 10
    response = client.get(
        f"/equipos/pagina/{pagina}?cantidad_equipos={cantidad_equipos}"
    )

    assert response.status_code == 404
    assert response.json() == {"detail": "No se encontraron equipos para esta página"}


def test_pagina_invalida(client):
    pagina = 0
    cantidad_equipos = 10
    response = client.get(
        f"/equipos/pagina/{pagina}?cantidad_equipos={cantidad_equipos}"
    )

    assert response.status_code == 404
    assert response.json() == {
        "detail": "Algunos de los parámetros están siendo mal introducidas"
    }


def test_cantidad_invalida(client):
    pagina = 1
    cantidad_equipos = 0
    response = client.get(
        f"/equipos/pagina/{pagina}?cantidad_equipos={cantidad_equipos}"
    )

    assert response.status_code == 404
    assert response.json() == {
        "detail": "Algunos de los parámetros están siendo mal introducidas"
    }
