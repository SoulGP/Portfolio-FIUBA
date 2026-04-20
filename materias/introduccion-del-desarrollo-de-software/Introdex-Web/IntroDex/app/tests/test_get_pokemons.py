from fastapi.testclient import TestClient
from app.main import app

client = TestClient(app)


def test_get_pokemons():
    response = client.get("/pokemons")
    assert response.status_code == 200
    assert len(response.json()) == 1092
