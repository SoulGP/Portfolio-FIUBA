from fastapi.testclient import TestClient
from app.main import app
from app.modelos import *
from sqlmodel import Session, select, create_engine
from app.database import SessionDep, engine, get_session
from app.main import app
import sys

client = TestClient(app)

engine = create_engine("sqlite:///app/tests/test_post_teams.db")

SQLModel.metadata.create_all(engine)

with Session(engine) as session:

    def get_session_override():
        return session


def test_crear_equipo_sin_pokemones() -> None:
    app.dependency_overrides[get_session] = get_session_override
    response = client.post(
        "/equipos/?id_equipo=11111&nombre_equipo=equipo_prueba&generacion_equipo=8"
    )
    app.dependency_overrides.clear()
    assert response.status_code == 201
    assert response.json()["id"] == 11111
    assert response.json()["nombre"] == "equipo_prueba"
    assert response.json()["integrantes"] == []
    assert response.json()["generacion"] == 8


def test_crear_equipo_con_id_existente() -> None:
    app.dependency_overrides[get_session] = get_session_override
    response = client.post(
        "/equipos/?id_equipo=11111&nombre_equipo=nuevo_equipo_prueba&generacion_equipo=8"
    )
    app.dependency_overrides.clear()
    sys.stderr.write(str(response.json()))
    assert response.status_code == 400
    assert response.json() == {"detail": "Ese equipo ya existe"}
    query = select(Equipo).where(Equipo.id == 11111)
    equipo_especifico = session.exec(query).first()
    session.delete(equipo_especifico)
    session.commit()


def test_crear_equipo_con_generacion_erronea() -> None:
    app.dependency_overrides[get_session] = get_session_override
    response = client.post(
        "/equipos/?id_equipo=2&nombre_equipo=equipo_prueba&generacion_equipo=9"
    )
    app.dependency_overrides.clear()
    assert response.status_code == 400
    assert response.json() == {"detail": "La generación del equipo no es válida"}


def test_crear_equipo_con_pokemones_movimientos_naturaleza_y_evs() -> None:
    app.dependency_overrides[get_session] = get_session_override
    response = client.post(
        "/equipos/?id_equipo=14111&nombre_equipo=equipo_prueba&generacion_equipo=8&id_pkm_1=25&id_naturaleza_1=1",
        json={
            "id_movimientos_pkm_1": [84, 85, 25, 87],
            "evs_pkm_1": {
                "vida": 10,
                "ataque": 10,
                "defensa": 10,
                "ataque_especial": 10,
                "defensa_especial": 10,
                "velocidad": 10,
            },
        },
    )
    app.dependency_overrides.clear()
    rta = response.json()
    assert response.status_code == 201
    query = select(Equipo).where(Equipo.id == 14111)
    equipo_especifico = session.exec(query).first()
    integrantes_especificos = session.exec(
        select(IntegrantesEquipo).where(IntegrantesEquipo.equipo_id == 14111)
    ).all()
    estadisticas_especificas = session.exec(
        select(Estadisticas)
        .join(IntegrantesEquipo)
        .where(IntegrantesEquipo.id == Estadisticas.member_id)
    ).all()
    for estadistica in estadisticas_especificas:
        session.delete(estadistica)
    for integrante in integrantes_especificos:
        session.delete(integrante)
    session.delete(equipo_especifico)
    session.commit()
    sys.stderr.write(str(rta["integrantes"][0]["movimientos"]))
    assert rta["id"] == 14111
    assert rta["nombre"] == "equipo_prueba"
    assert rta["integrantes"][0]["naturaleza"]["nombre"] == "hardy"
    for id_movim in rta["integrantes"][0]["movimientos"]:
        assert id_movim["id"] in [84, 85, 25, 87]
    assert rta["integrantes"][0]["evs"] == {
        "vida": 10,
        "ataque": 10,
        "defensa": 10,
        "ataque_especial": 10,
        "defensa_especial": 10,
        "velocidad": 10,
    }

    app.dependency_overrides[get_session] = get_session_override
    response = client.post(
        "/equipos/?id_equipo=1111&nombre_equipo=equipo_prueba&generacion_equipo=8&id_pkm_1=7777&id_naturaleza_1=1"
    )
    app.dependency_overrides.clear()
    sys.stderr.write(str(response.json()))
    assert response.status_code == 404
    assert response.json() == {"detail": "Pokemon no encontrado."}


def test_crear_equipo_con_pokemon_fuera_de_generacion() -> None:
    app.dependency_overrides[get_session] = get_session_override
    response = client.post(
        "/equipos/?id_equipo=1111&nombre_equipo=equipo_prueba&generacion_equipo=1&id_pkm_1=658&id_naturaleza_1=1"
    )  # Greninja no es de la generacion 1
    app.dependency_overrides.clear()
    sys.stderr.write(str(response.json()))
    assert response.status_code == 400
    assert response.json() == {
        "detail": "El pokemon no pertenece a la generación del equipo (tiene que ser menor o igual)"
    }


def test_crear_equipo_con_movimientos_inexistentes_no_aprendidos_o_fuera_de_rango() -> (
    None
):
    app.dependency_overrides[get_session] = get_session_override
    response = client.post(
        "/equipos/?id_equipo=1111&nombre_equipo=equipo_prueba&generacion_equipo=8&id_pkm_1=25&&id_naturaleza_1=1",
        json={"id_movimientos_pkm_1": [7777]},
    )  # Movimiento no existente
    app.dependency_overrides.clear()
    sys.stderr.write(str(response.json()))
    assert response.status_code == 404
    assert response.json() == {"detail": "Movimiento no encontrado."}
    response = client.post(
        "/equipos/?id_equipo=1111&nombre_equipo=equipo_prueba&generacion_equipo=8&id_pkm_1=25&id_naturaleza_1=1",
        json={"id_movimientos_pkm_1": [813]},
    )  # Pikachu no aprende Triple Axel
    sys.stderr.write(str(response.json()))
    assert response.status_code == 400
    assert response.json() == {"detail": "El pokemon no puede aprender ese movimiento"}
    response = client.post(
        "/equipos/?id_equipo=1111&nombre_equipo=equipo_prueba&generacion_equipo=8&id_pkm_1=25&id_naturaleza_1=1",
        json={"id_movimientos_pkm_1": [84, 85, 25, 87, 113]},
    )  # Todos los movimientos son validos pero son mas de 4
    sys.stderr.write(str(response.json()))
    assert response.status_code == 400
    assert response.json() == {
        "detail": "El pokemon no puede tener más de 4 movimientos"
    }


def test_crear_equipo_con_naturaleza_inexistente_o_sin_naturaleza() -> None:
    app.dependency_overrides[get_session] = get_session_override
    response = client.post(
        "/equipos/?id_equipo=1111&nombre_equipo=equipo_prueba&generacion_equipo=8&id_pkm_1=25&id_naturaleza_1=7777"
    )
    app.dependency_overrides.clear()
    sys.stderr.write(str(response.json()))
    assert response.status_code == 404
    assert response.json() == {"detail": "Naturaleza no encontrada."}

    response = client.post(
        "/equipos/?id_equipo=1111&nombre_equipo=equipo_prueba&generacion_equipo=8&id_pkm_1=25"
    )
    assert response.status_code == 404
    assert response.json() == {"detail": "Naturaleza no encontrada."}


def test_crear_equipo_con_evs_incorrectos() -> None:
    app.dependency_overrides[get_session] = get_session_override
    response = client.post(
        "/equipos/?id_equipo=1111&nombre_equipo=equipo_prueba&generacion_equipo=8&id_pkm_1=25&id_naturaleza_1=1",
        json={
            "id_movimientos_pkm_1": [84, 85, 25, 87],
            "evs_pkm_1": {
                "vida": -1,
                "ataque": 10,
                "defensa": 10,
                "ataque_especial": 10,
                "defensa_especial": 10,
                "velocidad": 10,
            },
        },
    )  # EVs negativos
    app.dependency_overrides.clear()
    sys.stderr.write(str(response.json()))
    assert response.status_code == 400
    assert response.json() == {"detail": "Los EVs no pueden ser menores a 0"}
    response = client.post(
        "/equipos/?id_equipo=1111&nombre_equipo=equipo_prueba&generacion_equipo=8&id_pkm_1=25&id_naturaleza_1=1",
        json={
            "id_movimientos_pkm_1": [84, 85, 25, 87],
            "evs_pkm_1": {
                "vida": 300,
                "ataque": 10,
                "defensa": 10,
                "ataque_especial": 10,
                "defensa_especial": 10,
                "velocidad": 10,
            },
        },
    )  # EVs mayores a 255
    assert response.status_code == 400
    assert response.json() == {"detail": "Los EVs no pueden ser mayores a 255"}
    response = client.post(
        "/equipos/?id_equipo=1111&nombre_equipo=equipo_prueba&generacion_equipo=8&id_pkm_1=25&id_naturaleza_1=1",
        json={
            "id_movimientos_pkm_1": [84, 85, 25, 87],
            "evs_pkm_1": {
                "vida": 100,
                "ataque": 100,
                "defensa": 100,
                "ataque_especial": 100,
                "defensa_especial": 100,
                "velocidad": 100,
            },
        },
    )  # Suma de EVs mayor a 510
    sys.stderr.write(str(response.json()))
    assert response.status_code == 400
    assert response.json() == {"detail": "La suma de los EVs no puede ser mayor a 510"}
