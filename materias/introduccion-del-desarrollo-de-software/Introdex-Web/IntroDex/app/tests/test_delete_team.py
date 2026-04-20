from fastapi.testclient import TestClient
from app.modelos import Equipo, IntegrantesEquipo, Estadisticas
from app.main import app
from app.database import get_session
from sqlmodel import SQLModel, Session, create_engine, select
import pytest

DATABASE_URL = "sqlite:///./test_delete.db"
engine = create_engine(DATABASE_URL, echo=True)


@pytest.fixture(scope="module")
def setup_db():
    SQLModel.metadata.create_all(engine)
    with Session(engine) as session:
        session.add_all(
            [
                Equipo(id=1, nombre="Equipo Rocket", generacion=1),
                Equipo(id=2, nombre="Equipo Aqua", generacion=3),
                IntegrantesEquipo(
                    id=1, equipo_id=1, pokemon_id=1, move_id=1, naturaleza_id=1
                ),
                IntegrantesEquipo(
                    id=2, equipo_id=1, pokemon_id=2, move_id=2, naturaleza_id=2
                ),
                Estadisticas(
                    id=1,
                    member_id=1,
                    vida=31,
                    ataque=31,
                    defensa=31,
                    ataque_especial=20,
                    defensa_especial=30,
                    velocidad=50,
                ),
                Estadisticas(
                    id=2,
                    member_id=2,
                    vida=20,
                    ataque=25,
                    defensa=15,
                    ataque_especial=40,
                    defensa_especial=30,
                    velocidad=50,
                ),
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


def test_eliminar_equipo_existente(client, setup_db):
    equipo_id = 1
    response = client.delete(f"/equipos/eliminar/{equipo_id}")

    assert response.status_code == 200
    assert response.json() == {
        "detail": f"El equipo con id {equipo_id} y sus integrantes han sido eliminados"
    }

    with Session(engine) as session:
        equipo = session.get(Equipo, equipo_id)
        assert equipo is None

        integrantes = session.exec(
            select(IntegrantesEquipo).where(IntegrantesEquipo.equipo_id == equipo_id)
        ).all()
        assert len(integrantes) == 0

        evs = session.exec(
            select(Estadisticas).where(Estadisticas.member_id.in_([1, 2]))
        ).all()
        assert len(evs) == 0


def test_eliminar_equipo_inexistente(client, setup_db):
    equipo_id = 55
    response = client.delete(f"/equipos/eliminar/{equipo_id}")

    assert response.status_code == 404
    assert response.json() == {
        "detail": f"El equipo con id {equipo_id} no ha sido encontrado"
    }


def test_eliminar_equipo_sin_integrantes(client, setup_db):
    equipo_id = 2
    response = client.delete(f"/equipos/eliminar/{equipo_id}")

    assert response.status_code == 200
    assert response.json() == {
        "detail": f"El equipo con id {equipo_id} y sus integrantes han sido eliminados"
    }

    with Session(engine) as session:
        equipo = session.get(Equipo, equipo_id)
        assert equipo is None

        integrantes = session.exec(
            select(IntegrantesEquipo).where(IntegrantesEquipo.equipo_id == equipo_id)
        ).all()
        assert len(integrantes) == 0
