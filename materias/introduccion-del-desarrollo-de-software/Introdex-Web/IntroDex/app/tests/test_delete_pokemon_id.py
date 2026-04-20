from fastapi.testclient import TestClient


def test_borrar_pokemon_existente(client: TestClient) -> None:
    response = client.delete("/pokemons/delete/132")
    assert response.status_code == 200


def test_borrar_pokemon_no_existente(client: TestClient) -> None:
    response = client.delete("/pokemons/delete/10000")
    assert response.status_code == 404
    assert response.json() == {"detail": "Pokemon not found"}
