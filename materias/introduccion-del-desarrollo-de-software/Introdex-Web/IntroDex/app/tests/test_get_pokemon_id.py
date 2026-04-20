from fastapi.testclient import TestClient
from app.main import app
from app.modelos import *

client = TestClient(app)


def test_get_pokemon_por_id_existente() -> None:
    # verificar si puede obtener a ditto por pokemon_id.
    response = client.get("/pokemons/id/132")
    assert response.status_code == 200


def test_obtener_pokemon_nombre_existente() -> None:
    response = client.get("/pokemons/nombre/ditto")
    assert response.status_code == 200


def test_no_existe_movimiento() -> None:
    # verificar si no existe un pokemon.
    response = client.get("/pokemons/id/9999999999999999")
    assert response.status_code == 404

    response = client.get("/pokemons/nombre/dadasd")
    assert response.status_code == 404


def test_verificar_datos_por_nombre_existente() -> None:
    response = client.get("/pokemons/nombre/ditto")
    data = response.json()

    assert data["nombre"] == "ditto"
    assert (
        data["imagen"]
        == "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/132.png"
    )
    assert data["altura"] == 3
    assert data["peso"] == 40
    assert data["generacion"] == 1

    assert len(data["tipos"]) == 1
    assert data["tipos"][0]["nombre"] == "Normal"

    assert len(data["habilidades"]) == 2
    assert sorted([hab["nombre"] for hab in data["habilidades"]]) == [
        "Flexibilidad",
        "Impostor",
    ]

    assert len(data["grupo_huevo"]) == 1
    assert data["grupo_huevo"][0]["nombre"] == "Ditto"

    for stat in data["stats"]:
        assert stat["base_stat"] == 48

    assert len(data["movimientos"]) == 1
    assert data["movimientos"][0]["movimientos"]["nombre"] == "Transformación"
