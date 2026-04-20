from app.main import app
from fastapi.testclient import TestClient

client = TestClient(app)


def test_obtener_pokemon_movimientos():
    response = client.get("/pokemons/25/movimientos")
    assert response.status_code == 200


def test_obtener_pokemon_movimientos_inexistente():
    response = client.get("/pokemons/7777/movimientos")
    assert response.status_code == 404
    assert response.json() == {"detail": "Pokemon no encontrado."}
