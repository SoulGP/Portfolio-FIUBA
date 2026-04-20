from fastapi.testclient import TestClient
from app.main import app
from app.modelos import *

client = TestClient(app)


def test_obtener_movimiento_id_pokemons_existente() -> None:
    response = client.get("/movimientos/144/pokemon")
    assert response.status_code == 200


def test_movimiento_id_pokemons_no_existe() -> None:
    response = client.get("/movimientos/8888888888/pokemon")
    assert response.status_code == 404
    assert response.json()["detail"] == "Movimiento not found"


def test_verificar_datos_movimiento_id_pokemons() -> None:
    response = client.get("/movimientos/144/pokemon")
    assert response.status_code == 200

    data = response.json()
    assert data[0]["nombre"] == "ditto"
    assert data[0]["movimientos"][0]["movimientos"]["nombre"] == "Transformación"


def test_show_pokemones_por_id_sin_pokemon():
    response = client.get("/movimientos/698/pokemon")

    assert response.status_code == 404
    assert (
        response.json()["detail"]
        == "No Pokémon were found that learn that movement through pre-established methods"
    )
